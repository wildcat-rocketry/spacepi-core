#ifndef SPACEPI_TARGETLIB_LINUX_STEPS_CLONEGITREPOSTEP_HPP
#define SPACEPI_TARGETLIB_LINUX_STEPS_CLONEGITREPOSTEP_HPP

#include <chrono>
#include <cstddef>
#include <stack>
#include <string>
#include <type_traits>
#include <git2.h>
#include <SpacePi.hpp>
#include <spacepi/liblinux/InstallationData.hpp>
#include <spacepi/liblinux/InstallationStep.hpp>
#include <spacepi/liblinux/SystemCaller.hpp>

namespace spacepi {
    namespace liblinux {
        namespace steps {
            class CloneGitRepoStep : public InstallationStep, private SystemCaller, private spacepi::log::AutoLog<decltype("spacepi:liblinux:git"_autolog)> {
                public:
                    CloneGitRepoStep() noexcept;

                    void run(InstallationData &data);

                private:
                    class GitContext {
                        public:
                            GitContext() noexcept;
                            ~GitContext() noexcept;

                            GitContext(GitContext &) = delete;
                            GitContext &operator =(GitContext &) = delete;
                    };

                    template <typename Type, void (*Free)(typename std::remove_pointer<Type>::type *)>
                    class UniqueGitPtr {
                        public:
                            UniqueGitPtr() noexcept : valid(false) {
                            }

                            UniqueGitPtr(Type ptr) noexcept : ptr(ptr), valid(true) {
                            }

                            UniqueGitPtr &operator =(Type ptr) noexcept {
                                reset();
                                this->ptr = ptr;
                                this->valid = true;
                                return *this;
                            }

                            UniqueGitPtr(UniqueGitPtr &&move) noexcept : ptr(move.ptr), valid(move.valid) {
                                move.valid = false;
                            }

                            UniqueGitPtr &operator =(UniqueGitPtr &&move) noexcept {
                                this->ptr = move.ptr;
                                this->valid = move.valid;
                                move.valid = false;
                                return *this;
                            }

                            UniqueGitPtr(UniqueGitPtr &) = delete;
                            UniqueGitPtr &operator =(UniqueGitPtr &) = delete;

                            ~UniqueGitPtr() noexcept {
                                reset();
                            }

                            void take() noexcept {
                                valid = true;
                            }

                            void reset() noexcept {
                                if (valid) {
                                    Free(*this);
                                    valid = false;
                                }
                            }

                            operator bool() const noexcept {
                                return valid;
                            }

                            operator Type() noexcept {
                                return ptr;
                            }

                            operator Type *() noexcept {
                                return &ptr;
                            }

                            Type *operator ->() noexcept {
                                return &ptr;
                            }

                            Type *operator &() noexcept {
                                return &ptr;
                            }

                        private:
                            Type ptr;
                            bool valid;
                    };

                    static int sidebandProgress(const char *str, int len, void *payload) noexcept;
                    static int transferProgress(const git_indexer_progress *stats, void *payload) noexcept;
                    static void checkoutProgress(const char *path, size_t completedSteps, size_t totalSteps, void *payload) noexcept;
                    static int submoduleRecurse(git_submodule *localMod, const char *name, void *payload) noexcept;

                    void handle(const std::string &op, int err);
                    void logProgress(const std::string &progress) noexcept;
                    void logLastProgress() noexcept;

                    git_fetch_options fetchOpt;
                    git_checkout_options checkoutOpt;
                    std::string lastProgress;
                    std::chrono::steady_clock::time_point lastLog;
                    git_repository *remoteRepo;
                    std::string *name;
                    std::stack<std::string> nameStack;
                    std::stack<UniqueGitPtr<git_repository *, git_repository_free>> localStack;
                    std::stack<UniqueGitPtr<git_repository *, git_repository_free>> remoteStack;
            };
        }
    }
}

#endif
