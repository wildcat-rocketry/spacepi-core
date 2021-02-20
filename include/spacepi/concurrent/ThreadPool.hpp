#ifndef SPACEPI_CORE_CONCURRENT_THREADPOOL_HPP
#define SPACEPI_CORE_CONCURRENT_THREADPOOL_HPP

#include <initializer_list>
#include <memory>
#include <thread>
#include <vector>
#include <utility>
#include <boost/fiber/all.hpp>
#include <spacepi/util/Command.hpp>
#include <spacepi/util/CommandConfigurable.hpp>

namespace spacepi {
    namespace concurrent {
        namespace detail {
            class GenericFiber {
                public:
                    virtual ~GenericFiber() = default;

                    virtual void start(std::vector<boost::fibers::fiber> &vec) const = 0;
            };

            template <typename Func>
            class WrappedFiber final : public GenericFiber {
                public:
                    explicit WrappedFiber(Func fn) noexcept : fn(fn) {
                    }

                    void start(std::vector<boost::fibers::fiber> &vec) const {
                        vec.emplace_back(fn);
                    }

                private:
                    const Func fn;
            };
        }

        /**
         * \brief Allows creating a pool of semi-concurrent tasks
         * 
         * This differs from normal threading because it actually runs all of the subroutines on the same thread, but
         * allows context-switching between them.  In this way, the code should never run into race conditions, as the
         * locations which are non-critical sections can be marked clearly.  However, this also means that
         * computation-intense operations should not be using this class.
         * 
         * \code{.cpp}
         * #include <spacepi/concurrent/ThreadPool.hpp>
         * #include <spacepi/util/Command.hpp>
         * 
         * void task1(int arg1, int arg2);
         * void task2();
         * 
         * int main(int argc, const char **argv) {
         *     Command cmd(argc, argv);
         *     ThreadPool pool(cmd);
         *     pool.add(task1, 42, 7);
         *     pool.add(task2);
         *     cmd.run(); // Starts task1 and task2 semi-concurrently
         *     return 0;
         * }
         * \endcode
         */
        class ThreadPool : public spacepi::util::CommandConfigurable {
            public:
                /**
                 * \brief Initializes a new ThreadPool
                 * 
                 * All tasks which are added to this ThreadPool will be executed when the \c cmd is run.
                 * 
                 * \param[in,out] cmd The spacepi::util::Command which determines when this ThreadPool starts
                 */
                ThreadPool(spacepi::util::Command &cmd);
                
                /**
                 * \brief Initializes a new ThreadPool with an initial list of tasks
                 * 
                 * All tasks which are added to this ThreadPool will be executed when the \c cmd is run.
                 * 
                 * \param[in,out] cmd The spacepi::util::Command which determines when this ThreadPool starts
                 * \param[in] fns A list of tasks to immediately add to this ThreadPool
                 */
                template <typename Fn>
                explicit ThreadPool(spacepi::util::Command &cmd, std::initializer_list<Fn> fns) : spacepi::util::CommandConfigurable("", cmd) {
                    for (typename std::initializer_list<Fn>::iterator it = fns.begin(); it != fns.end(); ++it) {
                        add(*it);
                    }
                }

                ThreadPool(const ThreadPool &) = delete;
                ThreadPool &operator =(const ThreadPool &) = delete;

                /**
                 * \brief Blocks until all tasks in this ThreadPool have finished
                 * 
                 * This is required because each task's stack is owned by this object, so they must be stopped before
                 * freeing the memory.
                 */
                virtual ~ThreadPool();

                /**
                 * \brief Add another task to this ThreadPool
                 * 
                 * \param[in] fn The function to execute
                 * \param[in,out] arg The arguments to pass to the function when it is started
                 */
                template <typename Fn, typename... Arg>
                void add(Fn &&fn, Arg &&...arg) {
                    auto func = std::bind(fn, std::forward<Arg>(arg)...);
                    functions.emplace_back(new detail::WrappedFiber<decltype(func)>(func));
                }

                void runCommand();

            private:
                void run();

                std::vector<std::unique_ptr<detail::GenericFiber>> functions;
                std::thread thread;
        };
    }
}

#endif
