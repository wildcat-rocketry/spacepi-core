#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h> 

#define LSIZ 40
#define RSIZ 40

int main(int argc, char *argv[]) {

	char line[RSIZ][LSIZ];
	FILE *fp;
	int numLines = 0;
	fp = fopen ("program_list", "r");
	int i;
	int opt;
	int status;
	pid_t p;
	char buf[0x100];

	//arguments for running on the pi or not- creates paths on the program list	
	while((opt = getopt(argc, argv, "pn")) != -1)  //"pi" and "not pi"
    	{  
		switch(opt)  
		{    
			case 'p':  
				system("find ../space-pi/bin/ -name main.c >> program_list.txt");  //these paths are wrong
		        	break;  
		    	case 'n':  
				system("find ../space-pi/modules/drivers/ -name main.c >> program_list.txt"); //these paths are wrong
		        	break;
		}  
	}  

	//reads the program list
	while (fgets(line[numLines], LSIZ, fp)) {
		line[numLines][strlen(line[numLines]) - 1] = '\0';
		numLines++;
	}
	
	//store pid and file descriptor for each program
	pid_t pids[numLines];
	char file_descs[numLines];

	//set up the log files
	for (i = 0; i < numLines; i++) {
		snprintf(buf, sizeof(buf), "%s_logFile", line[i]);
		fp = fopen(buf, "w");
		file_descs[i] = open(buf, O_WRONLY | O_APPEND); 

	}


	for (i = 0; i < numLines; i++) {
		p = fork();	 
		if (p == 0) { //child
			
			dup2(file_descs[i], 1);

			pids[i] = getpid();
			char *args[] = {line[i], NULL};
			execvp(args[0], args);
		}
		pids[i] = p;
	}


	if (p > 0) { //parent
		while (1) {

			for (i = 0; i < numLines; i++) {
				
				if(waitpid(pids[i], &status, WNOHANG) > 0){
					if (WIFEXITED(status)) {
						printf("restarting program %d\n", i);
						p = fork();

						if (p == 0) { //child

							dup2(file_descs[i], 1);

							pids[i] = getpid();
							char *args[] = {line[i], NULL};
							execvp(args[0], args);
						}
						pids[i] = p;
					}
				}

			}


		}
	}
	

	return 0;
}