#ifndef SPACEPI_CORE_CONCURRENT_THREADPOOL_HPP
#define SPACEPI_CORE_CONCURRENT_THREADPOOL_HPP

#include <initializer_list>
#include <vector>
#include <utility>
#include <boost/context/all.hpp>
#include <boost/fiber/all.hpp>

namespace spacepi {
    namespace concurrent {
        class ThreadPool {
            public:
                ThreadPool();
                
                template <typename Fn>
                ThreadPool(std::initializer_list<Fn> fns) {
                    for (typename std::initializer_list<Fn>::iterator it = fns.begin(); it != fns.end(); ++it) {
                        add(*it);
                    }
                }

                ThreadPool(const ThreadPool &) = delete;

                virtual ~ThreadPool();

                ThreadPool &operator =(const ThreadPool &) = delete;

                template <typename Fn,
                          typename ... Arg,
                          typename = boost::context::detail::disable_overload<boost::fibers::fiber, Fn>,
                          typename = boost::context::detail::disable_overload<boost::fibers::launch, Fn>,
                          typename = boost::context::detail::disable_overload<std::allocator_arg_t, Fn>>
                void add(Fn &&fn, Arg &&...arg) {
                    functions.emplace_back(std::forward<Fn>(fn), std::forward<Arg>(arg)...);
                }

            private:
                std::vector<boost::fibers::fiber> functions;
        };
    }
}

#endif
