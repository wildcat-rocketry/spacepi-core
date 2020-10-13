// spacepi-config
// This configs the pi on boot from the /etc/spacepi.xml file

// Way to many includes needed
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/foreach.hpp>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mount.h>
#include <sys/sendfile.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>
#include <shadow.h>
#include <string.h>
#include <errno.h>

#define CONFIG_PATH "/etc/spacepi.xml"

using namespace std;

using boost::property_tree::ptree;
using boost::optional;

typedef struct {
    struct passwd pw;
    struct spwd sh;
} PWD_PAIR;

// Linked list of passwords
struct pwd_ll {
    PWD_PAIR* data;
    struct pwd_ll* next; // Next in the list
};

// All the info for a single user
typedef struct {
    string uname;
    optional<string> name;
    optional<string> email;
    optional<string> shell;
    optional<string> keys;
    PWD_PAIR* entry;
} USER;

// Linked list of users
struct users_ll {
   USER* data;
  struct users_ll* next; 
};

// Functions to Copy pwd and spwd structs because of static pointer returns
void cp_pwd(struct passwd* clone, const struct passwd* old);
void cp_spwd(struct spwd* clone, const struct spwd* old);

// Take users from the config and passwd file and update them. Return updated user linked lists if updates are needed
// Otherwise both ll are NULL
int set_users(ptree & users, struct pwd_ll ** pwd_update, struct users_ll ** users_update);

// Locate all users in the config
struct users_ll *load_users_ll(ptree & users);

// Find if there is an existing user with same uname
USER* find_user_by_uname(struct users_ll *top_user, char *uname);

// Determine what users are system users or config users, associate password entries
struct pwd_ll *sort_passwds(struct users_ll *top_user);

// Go through the process of adding a user and setting up the home dir. Should have similar result to useradd
int add_user(USER *user, uid_t uid, gid_t gid);

// Update the home dir of a user
int update_home(USER *user);

// Find the next available UID within a set range
uid_t next_uid(struct users_ll *top, uid_t start);

// Write users to disk
int write_users(struct pwd_ll * passwords_first, struct users_ll * users_first);

int main( int argc, char **argv ){
    ptree pt;
    read_xml(CONFIG_PATH, pt);    
    fstream file;
    string old_hostname;
    string old_ip;
    int update_hostname, update_ip;

    optional<ptree &> options = pt.get_child_optional("config.target.options");
    if(!options){
        cerr << "config.target.options does not exist in " CONFIG_PATH "\n";
        return 1;
    }

    optional<string> ip = (*options).get_optional<string>("ip");
    optional<string> hostname = (*options).get_optional<string>("hostname");

    // Loop through users and determine if 1. User needs to be added, 2. User needs to be updated, 3. User needs to be deleted
    optional<ptree &> users = (*options).get_child_optional("users");
    struct pwd_ll ** updated_pwd = new struct pwd_ll *;
    struct users_ll ** updated_users = new struct users_ll *;
    if(users){
        if(int retval = set_users(*users, updated_pwd, updated_users)){ return retval; }
    } else {
        cerr << "No users in config, will not change users\n";
    }

    if(!ip || !hostname){
        cerr << "Either hostname or ip are not defined, not changing those\n";
    } else {
        file.open("/etc/hostname", ios::in);
        if(file.is_open()){
            getline(file, old_hostname);
            cout << "Old hostname is (" << old_hostname << ")\n";
            file.close();

            if(old_hostname.compare(*hostname)){
                update_hostname = 1;
            }
        } else {
            update_hostname = 1;
        }

        file.open("/etc/ip", ios::in);
        if(file.is_open()){
            getline(file, old_ip);
            cout << "Old ip is (" << old_ip << ")\n";
            file.close();

            if(old_ip.compare(*ip)){
                update_ip = 1;
            }
        } else {
            update_ip = 1;
        }

        cout << "NEW IP: " << *ip << " HOSTNAME: " << *hostname << "\n";
    }

    FILE * fp_pwd;
    int remount_root;
    if(*updated_users || update_hostname || update_ip){
        fp_pwd = fopen("/etc/passwd", "a");
        if(!fp_pwd) {
            // Try remounting / as rw
            remount_root = 1;
            mount("", "/", "", MS_REMOUNT, NULL);
        }
	close((int)fp_pwd);
    }

    if(*updated_users != NULL){
        if(int retval = write_users(*updated_pwd, *updated_users)){ return retval; }
    }
    
    if(update_hostname){
        file.open("/etc/hostname", ios::out);
        file << *hostname << "\n";
        file.close();

        int cid = fork();
        if(cid == 0){
            execlp("sed", "-e", string("s|" + old_hostname + "|" + *hostname + "|").c_str(), "-i", "/etc/hosts", (char*)NULL);
        } else if (cid == -1){
            cerr << "Fork for sed failed somehow\n";
            return 1;
        } else {
            wait(NULL);
        }

	// Set hostname used by systemd because systemd-hostnamed would have already been executed
        cid = fork();
        if(cid == 0){
            execlp("hostnamectl", "--transient", "set-hostname", (*hostname).c_str(), (char*)NULL);
        } else if (cid == -1){
            cerr << "Fork for hostname somehow\n";
            return 1;
        } else {
            wait(NULL);
        }
    }

    if(update_ip){
        file.open("/etc/ip", ios::out);
        file << *ip << "\n";
        file.close();
    }

    if(remount_root){
        // Go back to ro for safety
        mount("", "/", "", MS_REMOUNT | MS_RDONLY, NULL);
    }
    
    return 0;
}


void cp_pwd(struct passwd* clone, const struct passwd* old){
    clone->pw_uid = old->pw_uid;
    clone->pw_gid = old->pw_gid;
    clone->pw_name = (char *)malloc(sizeof(char) * (strlen(old->pw_name) + 1));
    clone->pw_passwd = (char *)malloc(sizeof(char) * (strlen(old->pw_passwd) + 1));
    clone->pw_gecos = (char *)malloc(sizeof(char) * (strlen(old->pw_gecos) + 1));
    clone->pw_dir = (char *)malloc(sizeof(char) * (strlen(old->pw_dir) + 1));
    clone->pw_shell = (char *)malloc(sizeof(char) * (strlen(old->pw_shell) + 1));

    strcpy(clone->pw_name, old->pw_name);
    strcpy(clone->pw_passwd, old->pw_passwd);
    strcpy(clone->pw_gecos, old->pw_gecos);
    strcpy(clone->pw_dir, old->pw_dir);
    strcpy(clone->pw_shell, old->pw_shell);
}

void cp_spwd(struct spwd* clone, const struct spwd* old){
    clone->sp_lstchg = old->sp_lstchg;
    clone->sp_min = old->sp_min;
    clone->sp_max = old->sp_max;
    clone->sp_warn = old->sp_warn;
    clone->sp_inact = old->sp_inact;
    clone->sp_expire = old->sp_expire;
    clone->sp_namp = (char *)malloc(sizeof(char) * (strlen(old->sp_namp) + 1));
    clone->sp_pwdp = (char *)malloc(sizeof(char) * (strlen(old->sp_pwdp) + 1));

    strcpy(clone->sp_namp, old->sp_namp);
    strcpy(clone->sp_pwdp, old->sp_pwdp);
}

int set_users(ptree & users, struct pwd_ll ** pwd_update, struct users_ll ** users_update){
    struct users_ll * users_first = load_users_ll(users);
    struct pwd_ll * passwords_first = sort_passwds(users_first);
    int ret;
    bool update_pwd = false;

    cout << "Start setting up users\n";

    struct group* sudo_grp = getgrnam("sudo");
    if(!sudo_grp){
        cerr << "No sudo group found\n";
        return 1;
    }

    // Update or add user
    for(struct users_ll *user_i = users_first; user_i; user_i = user_i->next){
        if(user_i->data->entry){
            // Update existing entry
            if(user_i->data->shell){
                if(user_i->data->entry->pw.pw_shell &&
                   strcmp(user_i->data->entry->pw.pw_shell, (*(user_i->data->shell)).c_str()) != 0){
                } else {
                    update_pwd = true;
                    user_i->data->entry->pw.pw_shell = (char*)malloc(sizeof(char) * 64);
                    strcpy(user_i->data->entry->pw.pw_shell, (*(user_i->data->shell)).c_str());
                }
            }
        } else {
            // Add user entry
            update_pwd = true;
            if( (ret = add_user(user_i->data, next_uid(users_first, 1000), sudo_grp->gr_gid)) ){
                return ret;
            }
        }
    }

    cout << "Done updating or adding users\n";

    // Add users to pwd
    if(update_pwd){
        *pwd_update = passwords_first;
        *users_update = users_first;
    }else{
        *pwd_update = NULL;
        *users_update = NULL;
    }

    return 0;
}

int write_users(struct pwd_ll * passwords_first, struct users_ll * users_first){
    FILE *fp_pwd;

    fp_pwd = fopen("/etc/passwd", "w");
    if(!fp_pwd){
        cerr << "Unable to open /etc/passwd for editing\n";
        return 1;
    }

    FILE *fp_sh;
    fp_sh = fopen("/etc/shadow", "w");
    if(!fp_pwd){
        cerr << "Unable to open /etc/shadow for editing\n";
        return 1;
    }

    for(struct pwd_ll * pwd_i = passwords_first; pwd_i; pwd_i = pwd_i->next){
        putpwent(&(pwd_i->data->pw), fp_pwd);
        putspent(&(pwd_i->data->sh), fp_sh);
    }

    for(struct users_ll *user_i = users_first; user_i; user_i = user_i->next){
        putpwent(&(user_i->data->entry->pw), fp_pwd);
        putspent(&(user_i->data->entry->sh), fp_sh);
    }

    cout << "Done writing users\n";

    close((int)fp_pwd);
    close((int)fp_sh);

    // Update user homes
    for(struct users_ll *user_i = users_first; user_i; user_i = user_i->next){
        update_home(user_i->data);
    }

    return 0;
}

struct users_ll *load_users_ll(ptree & users){
    cout << "Start loading users\n";
    struct users_ll * users_first = NULL;
    struct users_ll * cur_user = users_first;
    struct users_ll * new_user;
    optional<string> username;
    BOOST_FOREACH(const ptree::value_type &pair, users.get_child("")){
        const ptree & user = pair.second;
        new_user = new struct users_ll;
        new_user->data = new USER;
        username = user.get_optional<string>("uname");
        if(!username){
            cerr << "User without username\n";
            free(new_user->data);
            free(new_user);
            continue;
        }
        new_user->data->uname = *username;
        new_user->data->name = user.get_optional<string>("name");
        new_user->data->email = user.get_optional<string>("email");
        new_user->data->shell = user.get_optional<string>("shell");
        new_user->data->keys = user.get_optional<string>("keys");
        new_user->data->entry = NULL;
        new_user->next = NULL;

        if(!users_first){
            users_first = new_user;
            cur_user = new_user;
        } else {
            cur_user->next = new_user;
            cur_user = new_user;
        }
    }

    cout << "Done loading users\n";
    return users_first;
}

USER* find_user_by_uname(struct users_ll *top, char *uname){
    for(struct users_ll *cur_user = top; cur_user; cur_user = cur_user->next){
        if(strcmp(cur_user->data->uname.c_str(), uname) == 0){
            return cur_user->data;
        }
    }

    return NULL;
}

// Sort passwords into either system passwords or associated with a user, others are deleted
struct pwd_ll *sort_passwds(struct users_ll *top_user){
    struct passwd* cur_pwd;
    struct spwd* cur_spwd;
    struct pwd_ll* first_ll = NULL;
    struct pwd_ll* cur_ll = first_ll;
    struct pwd_ll* new_ll = first_ll;
    PWD_PAIR *new_pair;
    USER *user_to_amend;

    while( (cur_pwd = getpwent()) ){
        new_pair = new PWD_PAIR;
        cp_pwd(&(new_pair->pw), cur_pwd);
        cur_spwd = getspnam(cur_pwd->pw_name);
        if(cur_spwd){
            cp_spwd(&(new_pair->sh), cur_spwd);
        }

        user_to_amend = find_user_by_uname(top_user, cur_pwd->pw_name);
        if(user_to_amend){
            user_to_amend->entry = new_pair;
        } else if(cur_pwd->pw_uid < 1000 || cur_pwd->pw_uid > 5000){ 
            // Delete users between 1000 and 5000 unless they are associated with a user in the config
            new_ll = new struct pwd_ll;
            new_ll->data = new_pair;
            new_ll->next = NULL;
            if(!first_ll){
                first_ll = new_ll;
                cur_ll = first_ll;
            } else {
                cur_ll->next = new_ll;
                cur_ll = cur_ll->next;
            }
        } else free(new_pair); // Delete user pair if not saved as system user or in config
    }
    endpwent();

    return first_ll;
}

int add_user(USER *user, uid_t uid, gid_t gid){
    cout << "Start adding user: " << user->uname << "\n";
    PWD_PAIR* new_pwd = new PWD_PAIR;
    new_pwd->pw.pw_uid = uid;
    new_pwd->pw.pw_gid = gid;
    new_pwd->pw.pw_passwd = (char*)malloc(sizeof(char) * 2);
    strcpy(new_pwd->pw.pw_passwd, "x");

    new_pwd->pw.pw_gecos = (char*)malloc(sizeof(char) * 25);
    strcpy(new_pwd->pw.pw_gecos, "From spacepi-config");

    new_pwd->pw.pw_name = (char*)malloc(sizeof(char) * 25);
    strcpy(new_pwd->pw.pw_name, user->uname.c_str());

    new_pwd->pw.pw_dir = (char*)malloc(sizeof(char) * 64);
    strcpy(new_pwd->pw.pw_dir, string("/home/" + user->uname).c_str());

    new_pwd->pw.pw_shell = (char*)malloc(sizeof(char) * 64);
    if(user->shell)
        strcpy(new_pwd->pw.pw_shell, (*user->shell).c_str());
    else
        strcpy(new_pwd->pw.pw_shell, "/bin/bash");

    new_pwd->sh.sp_namp = new_pwd->pw.pw_name;
    new_pwd->sh.sp_pwdp = (char*)malloc(sizeof(char) * 2);
    strcpy(new_pwd->sh.sp_pwdp, "*");
    new_pwd->sh.sp_lstchg = 18509;
    new_pwd->sh.sp_min = 0;
    new_pwd->sh.sp_max = 99999;
    new_pwd->sh.sp_warn = 7;
    new_pwd->sh.sp_inact = -1;
    new_pwd->sh.sp_expire = -1;
    new_pwd->sh.sp_flag = -1;

    user->entry = new_pwd;

    if(mkdir(new_pwd->pw.pw_dir, 0755)){
        if(errno == EEXIST){
            chown(new_pwd->pw.pw_dir, new_pwd->pw.pw_uid, new_pwd->pw.pw_gid);
            cout << "Home for user " << new_pwd->pw.pw_name << " already exists\n";
            return 0;
        }
        cerr << "Error occured adding directory: " << new_pwd->pw.pw_dir << "\n";
        return 1;
    }

    int home_fd = open(new_pwd->pw.pw_dir, O_DIRECTORY);
    if(!home_fd){
        cerr << "Error opening directory: " << new_pwd->pw.pw_dir << "\n";
        return 1;
    }

    chown(new_pwd->pw.pw_dir, new_pwd->pw.pw_uid, new_pwd->pw.pw_gid);

    // Copy files from skeleton dir into new home dir
    DIR *skel_dir = opendir("/etc/skel");
    int skel_fd = dirfd(skel_dir);
    struct dirent *dir_entry;
    int skel_file, home_file;


    while( (dir_entry = readdir(skel_dir)) ){
        if(dir_entry->d_type == DT_REG){
            if((skel_file = openat(skel_fd, dir_entry->d_name, O_RDONLY)) == -1){
                cerr << "Error opening skel file: " << dir_entry->d_name << strerror(errno) << "\n";
                return -1;
            }
            if((home_file = openat(home_fd, dir_entry->d_name, O_CREAT | O_EXCL | O_WRONLY, 0660)) == -1){
                if(errno == EEXIST){
                    closedir(skel_dir);
                    close(skel_file);
                    continue;
                }
                else {
                    cerr << "Error opening file in home: " << dir_entry->d_name << ": " << strerror(errno) << "\n";
                    close(skel_file);
                    return -1;
                }
            }

            struct stat fileinfo = {0};
            fstat(skel_file, &fileinfo);
            int count;
            if( (count = sendfile(home_file, skel_file, NULL, fileinfo.st_size)) == -1){
                cerr << "Error copying file: " << strerror(errno) << "\n";
                return -1;
            }

            cout << "Coppied " << count << " bytes of " << fileinfo.st_size << "\n";

            close(skel_file);
            close(home_file);
            fchownat(home_fd, dir_entry->d_name, new_pwd->pw.pw_uid, new_pwd->pw.pw_gid, 0);
        }
    }

    mkdirat(home_fd, ".ssh", 0755);
    int ssh_fd = openat(home_fd, ".ssh", O_DIRECTORY);
    if(!ssh_fd){
        cerr << "Error adding .ssh directory";
        return 1;
    }

    symlinkat("/spacepi/deploy-keys/id_rsa", ssh_fd, "id_rsa");
    symlinkat("/spacepi/deploy-keys/id_rsa.pub", ssh_fd, "id_rsa.pub");

    closedir(skel_dir);
    close(home_fd);
    cout << "Done adding user: " << user->uname << "\n";

    return 0;
}

uid_t next_uid(struct users_ll *top, uid_t start){
    for(struct users_ll *cur = top; cur; cur = cur->next){
        if(cur->data->entry)
            if(cur->data->entry->pw.pw_uid == start) return next_uid(top, ++start);
    }
    return start;
}

int update_home(USER *user){

    fstream file;
    if(user->keys){
        file.open(string(string(user->entry->pw.pw_dir) + "/.ssh/authorized_keys").c_str(), ios::out);
        file << *(user->keys) << "\n";
        file.close();
    }
       
    // Become user and update GIT
    int pid = fork();
    if(pid == 0){ // Am child
        // Become child
        setuid(user->entry->pw.pw_uid);
        setgid(user->entry->pw.pw_gid);

        setenv("HOME", user->entry->pw.pw_dir, 1); // Set home to child home

        if(user->name){
            if( (pid = fork()) == 0 ){ // I have no clue why passing an extra config arg is needed
                execlp("git", "config", "config", "--global", "user.name", (*(user->name)).c_str(), (char*)NULL);
            } else if (pid == -1){
            } else {
                wait(NULL);
            }
        }

        if(user->email){
            if( (pid = fork()) == 0 ){
                execlp("git", "config", "config", "--global", "user.email", (*(user->email)).c_str(), (char*)NULL);
            } else if (pid == -1){
            } else {
                wait(NULL);
            }
        }

        exit(0);

    } else if (pid == -1) { // Is error
        cerr << "Error forking to update user: " << strerror(errno) << "\n";
        return 1;
    } else { // Is parent
        wait(NULL);
    }

    return 0;
}
