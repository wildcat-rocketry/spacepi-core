#include <array>
#include <cerrno>
#include <chrono>
#include <cstddef>
#include <cstring>
#include <sstream>
#include <stack>
#include <string>
#include <utility>
#include <vector>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <boost/filesystem.hpp>
#include <git2.h>
#include <SpacePi.hpp>
#include <spacepi/liblinux/steps/CloneGitRepoStep.hpp>
#include <spacepi/liblinux/Config.hpp>
#include <spacepi/liblinux/InstallationConfig.hpp>
#include <spacepi/liblinux/InstallationData.hpp>
#include <spacepi/liblinux/InstallationOptions.hpp>
#include <spacepi/liblinux/SharedTempDir.hpp>
#include <spacepi/liblinux/SystemCaller.hpp>
#include <spacepi/liblinux/UniqueChroot.hpp>

using namespace std;
using namespace std::chrono;
using namespace boost::filesystem;
using namespace spacepi::log;
using namespace spacepi::util;
using namespace spacepi::liblinux;
using namespace spacepi::liblinux::steps;

CloneGitRepoStep::CloneGitRepoStep() noexcept {
    fetchOpt = GIT_FETCH_OPTIONS_INIT;
    fetchOpt.prune = GIT_FETCH_NO_PRUNE;
    fetchOpt.download_tags = GIT_REMOTE_DOWNLOAD_TAGS_NONE;
    fetchOpt.callbacks.sideband_progress = sidebandProgress;
    fetchOpt.callbacks.transfer_progress = transferProgress;
    fetchOpt.callbacks.payload = this;
    checkoutOpt = GIT_CHECKOUT_OPTIONS_INIT;
    checkoutOpt.progress_cb = checkoutProgress;
    checkoutOpt.progress_payload = this;
}

void CloneGitRepoStep::run(InstallationData &data) {
    InstallationConfig &config = data.getData<InstallationConfig>();
    const string &rootPath = data.getData<SharedTempDir>().getPath();
    path remoteRepoPath = rootPath / config.sourceDir;
    {
        GitContext gitCtx;
        {
            UniqueGitPtr<git_repository *, git_repository_free> localRepo;
            handle("opening local repo", git_repository_open(&localRepo, CMAKE_SOURCE_DIR));
            localRepo.take();

            create_directories(remoteRepoPath);
            UniqueGitPtr<git_repository *, git_repository_free> remoteRepo;
            handle("creating remote repo", git_repository_init(&remoteRepo, remoteRepoPath.c_str(), 0));
            remoteRepo.take();

            nameStack.emplace(config.sourceDir);
            localStack.emplace(move(localRepo));
            remoteStack.emplace(move(remoteRepo));
        }
        while (!nameStack.empty()) {
            string &name = nameStack.top();
            UniqueGitPtr<git_repository *, git_repository_free> &localRepo = localStack.top();
            UniqueGitPtr<git_repository *, git_repository_free> &remoteRepo = remoteStack.top();

            if (!git_repository_head_unborn(remoteRepo)) {
                log(LogLevel::Info) << "Done cloning " << name << ".";
                nameStack.pop();
                localStack.pop();
                remoteStack.pop();
                continue;
            }
            log(LogLevel::Info) << "Cloning into " << name << "...";

            UniqueGitPtr<git_config *, git_config_free> config;
            handle("getting repo config", git_repository_config(&config, remoteRepo));
            config.take();

            handle("setting repo config", git_config_set_string(config, "core.sharedRepository", "group"));

            UniqueGitPtr<git_reference *, git_reference_free> localHead;
            handle("getting local head", git_repository_head(&localHead, localRepo));
            localHead.take();

            UniqueGitPtr<git_object *, git_object_free> headCommit;
            handle("peeling local head", git_reference_peel(&headCommit, localHead, GIT_OBJECT_COMMIT));
            headCommit.take();

            string refSpec;
            bool headIsBranch = git_reference_is_branch(localHead);
            UniqueGitPtr<git_reference *, git_reference_free> tempBranch;
            string upstreamRemote;
            if (headIsBranch) {
                refSpec = git_reference_name(localHead);

                UniqueGitPtr<git_buf, git_buf_dispose> upstream;
                memset(&upstream, 0, sizeof(git_buf));
                handle("getting local upstream info", git_branch_upstream_remote(&upstream, localRepo, refSpec.c_str()));
                upstream.take();

                upstreamRemote = string(upstream->ptr, upstream->size);
            } else {
                UniqueGitPtr<git_commit *, git_commit_free> localHeadCommit;
                handle("looking up local head commit", git_commit_lookup(&localHeadCommit, localRepo, git_object_id(headCommit)));
                localHeadCommit.take();

                handle("creating temporary branch", git_branch_create(&tempBranch, localRepo, "spacepi-installer-temporary", localHeadCommit, 1));
                tempBranch.take();

                refSpec = git_reference_name(tempBranch);

                upstreamRemote = "spacepi-installer-tmp";
            }

            string remoteURL = ("file://" / absolute(git_repository_path(localRepo))).native();
            UniqueGitPtr<git_remote *, git_remote_free> remote;
            int res = git_remote_create(&remote, remoteRepo, upstreamRemote.c_str(), remoteURL.c_str());
            if (res == GIT_EEXISTS) {
                handle("resetting temporary remote", git_remote_set_url(remoteRepo, upstreamRemote.c_str(), remoteURL.c_str()));

                handle("looking up temporary remote", git_remote_lookup(&remote, remoteRepo, upstreamRemote.c_str()));
            } else {
                handle("adding temporary remote", res);
            }
            remote.take();

            array<char *, 1> refSpecs;
            vector<char> refSpecData;
            refSpecData.resize(refSpec.size() + 1);
            strcpy(refSpecData.data(), refSpec.data());
            refSpecs[0] = refSpecData.data();
            git_strarray refSpecArr;
            refSpecArr.strings = refSpecs.data();
            refSpecArr.count = 1;
            log(LogLevel::Debug) << "Fetching from " << refSpecArr.strings[0];
            handle("fetching changes", git_remote_fetch(remote, &refSpecArr, &fetchOpt, "spacepi"));
            logLastProgress();

            if (!headIsBranch) {
                handle("deleting temporary branch", git_branch_delete(tempBranch));
                tempBranch.reset();
            }

            remote.reset();
            if (!headIsBranch) {
                handle("deleting temporary remote", git_remote_delete(remoteRepo, upstreamRemote.c_str()));
            }

            UniqueGitPtr<git_strarray, git_strarray_dispose> remoteList;
            handle("listing local remotes", git_remote_list(&remoteList, localRepo));
            remoteList.take();
            for (int i = 0; i < remoteList->count; ++i) {
                if (string(remoteList->strings[i]) == "flash") {
                    continue;
                }

                handle("looking up local remote", git_remote_lookup(&remote, localRepo, remoteList->strings[i]));
                remote.take();

                string url = git_remote_url(remote);
                remote.reset();

                if (upstreamRemote == string(remoteList->strings[i])) {
                    string flashURL = "file:///media/flash" + url.substr(url.find_last_not_of('/'));
                    res = git_remote_create(&remote, remoteRepo, "flash", flashURL.c_str());
                    if (res == GIT_EEXISTS) {
                        handle("resetting flash remote", git_remote_set_url(remoteRepo, "flash", flashURL.c_str()));
                    } else {
                        handle("creating flash remote", res);
                        remote.take();
                        remote.reset();
                    }
                }

                res = git_remote_create(&remote, remoteRepo, remoteList->strings[i], url.c_str());
                if (res == GIT_EEXISTS) {
                    handle("resetting remote", git_remote_set_url(remoteRepo, remoteList->strings[i], url.c_str()));
                } else {
                    handle("copying remote", res);
                    remote.take();
                    remote.reset();
                }
            }

            UniqueGitPtr<git_object *, git_object_free> remoteHeadObject;
            handle("looking up remote head object", git_object_lookup(&remoteHeadObject, remoteRepo, git_object_id(headCommit), GIT_OBJECT_COMMIT));
            remoteHeadObject.take();

            handle("checking out remote head", git_reset(remoteRepo, remoteHeadObject, GIT_RESET_HARD, &checkoutOpt));
            logLastProgress();

            handle("setting remote detached head", git_repository_set_head_detached(remoteRepo, git_object_id(headCommit)));

            if (headIsBranch) {
                UniqueGitPtr<git_commit *, git_commit_free> remoteHeadCommit;
                handle("looking up remote head commit", git_commit_lookup(&remoteHeadCommit, remoteRepo, git_object_id(headCommit)));
                remoteHeadCommit.take();

                string branchName = git_reference_shorthand(localHead);
                UniqueGitPtr<git_reference *, git_reference_free> branch;
                handle("creating remote head branch", git_branch_create(&branch, remoteRepo, branchName.c_str(), remoteHeadCommit, 1));
                branch.take();

                handle("setting remote head", git_repository_set_head(remoteRepo, refSpec.c_str()));

                UniqueGitPtr<git_reference *, git_reference_free> upstream;
                handle("getting local upstream info", git_branch_upstream(&upstream, localHead));
                upstream.take();

                string upstreamStr = git_reference_shorthand(upstream);
                handle("setting remote upstream info", git_branch_set_upstream(branch, upstreamStr.c_str()));
            }

            this->remoteRepo = remoteRepo;
            this->name = &name;
            handle("recursing into submodules", git_submodule_foreach(localRepo, submoduleRecurse, this));
            this->remoteRepo = nullptr;
            this->name = nullptr;
        }
    }
    for (recursive_directory_iterator it(remoteRepoPath); it != recursive_directory_iterator(); ++it) {
        struct stat buf;
        SystemCaller::handle(stat(it->path().c_str(), &buf))
            << "Failed to stat " << it->path() << ": " << SyscallErrorString;
        if (((buf.st_mode & 0700) >> 3) != (buf.st_mode & 0070)) {
            SystemCaller::handle(chmod(it->path().c_str(), (buf.st_mode & ~0070) | ((buf.st_mode & 0700) >> 3)))
                << "Failed to set permissions on " << it->path() << ": " << SyscallErrorString;
        }
        SystemCaller::handle(chown(it->path().c_str(), config.sourceUid, config.sourceGid))
            << "Failed to set ownership on " << it->path() << ": " << SyscallErrorString;
    }
    SystemCaller::handle(chmod(remoteRepoPath.c_str(), 0775))
        << "Failed to set permissions on " << remoteRepoPath << ": " << SyscallErrorString;
    SystemCaller::handle(chown(remoteRepoPath.c_str(), config.sourceUid, config.sourceGid))
        << "Failed to set ownership on " << remoteRepoPath << ": " << SyscallErrorString;
}

CloneGitRepoStep::GitContext::GitContext() noexcept {
    git_libgit2_init();
}

CloneGitRepoStep::GitContext::~GitContext() noexcept {
    git_libgit2_shutdown();
}

int CloneGitRepoStep::sidebandProgress(const char *str, int len, void *payload) noexcept {
    ((CloneGitRepoStep *) payload)->logProgress(string(str, len));
    return 0;
}

int CloneGitRepoStep::transferProgress(const git_indexer_progress *stats, void *payload) noexcept {
    if (stats->total_objects > 0) {
        ostringstream progress;
        progress
            << "Copying objects: " << (stats->received_objects * 100 / stats->total_objects)
            << "% (" << stats->received_objects << "/" << stats->total_objects << ")";
        if (stats->received_objects == stats->total_objects) {
            progress << ", done";
        }
        ((CloneGitRepoStep *) payload)->logProgress(progress.str());
    }
    return 0;
}

void CloneGitRepoStep::checkoutProgress(const char *path, size_t completedSteps, size_t totalSteps, void *payload) noexcept {
    if (totalSteps > 0) {
        ostringstream progress;
        progress
            << "Checking out files: " << (completedSteps * 100 / totalSteps)
            << "% (" << completedSteps << "/" << totalSteps << ")";
        if (completedSteps == totalSteps) {
            progress << ", done";
        }
        ((CloneGitRepoStep *) payload)->logProgress(progress.str());
    }
}

int CloneGitRepoStep::submoduleRecurse(git_submodule *localMod, const char *name, void *payload) noexcept {
    CloneGitRepoStep *self = (CloneGitRepoStep *) payload;

    UniqueGitPtr<git_submodule *, git_submodule_free> remoteMod;
    switch (git_submodule_lookup(&remoteMod, self->remoteRepo, name)) {
        case GIT_OK:
            break;
        case GIT_ERROR:
            return -1;
        case GIT_ENOTFOUND:
            git_error_set_str(GIT_ERROR_SUBMODULE, "submodule does not exist in remote repo");
            return -1;
        case GIT_EEXISTS:
            git_error_set_str(GIT_ERROR_SUBMODULE, "repo is only in working directory");
            return -1;
        default:
            git_error_set_str(GIT_ERROR_SUBMODULE, "unknown error");
            return -1;
    }
    remoteMod.take();

    UniqueGitPtr<git_repository *, git_repository_free> localSubRepo;
    if (git_submodule_open(&localSubRepo, localMod)) {
        return -1;
    }
    localSubRepo.take();

    if (git_submodule_init(remoteMod, 0)) {
        return -1;
    }

    UniqueGitPtr<git_repository *, git_repository_free> remoteSubRepo;
    if (git_submodule_repo_init(&remoteSubRepo, remoteMod, 1)) {
        return -1;
    }
    remoteSubRepo.take();

    self->nameStack.emplace(*self->name + "/" + string(name));
    self->localStack.emplace(move(localSubRepo));
    self->remoteStack.emplace(move(remoteSubRepo));
    return 0;
}

void CloneGitRepoStep::handle(const string &op, int err) {
    if (err) {
        const git_error *e = git_error_last();
        if (e) {
            throw EXCEPTION(ResourceException("Failed " + op + ": " + string(e->message)));
        } else {
            throw EXCEPTION(ResourceException("Unknown error while " + op + "."));
        }
    } else {
        log(LogLevel::Debug) << "Finished " << op;
    }
    git_error_clear();
}

void CloneGitRepoStep::logProgress(const string &progress) noexcept {
    steady_clock::time_point now = steady_clock::now();
    if (!lastProgress.empty() && lastProgress.substr(0, 4) != progress.substr(0, 4)) {
        log(LogLevel::Info) << lastProgress;
        lastProgress = progress;
        lastLog = now;
    } else if (now - lastLog > seconds(1)) {
        log(LogLevel::Info) << progress;
        lastProgress.clear();
        lastLog = now;
    } else {
        lastProgress = progress;
    }
}

void CloneGitRepoStep::logLastProgress() noexcept {
    if (!lastProgress.empty()) {
        log(LogLevel::Info) << lastProgress;
        lastProgress.clear();
    }
}
