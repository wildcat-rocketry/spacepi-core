#ifndef SPACEPI_CORE_CONCURRENT_THREADPOOL_HPP
#define SPACEPI_CORE_CONCURRENT_THREADPOOL_HPP

#include <initializer_list>
#include <memory>
#include <vector>
#include <utility>
#include <boost/context/all.hpp>
#include <boost/fiber/all.hpp>

namespace spacepi {
    namespace concurrent {
        namespace detail {
            class GenericFiber {
                public:
                    virtual void start(std::vector<boost::fibers::fiber> &vec) const = 0;
            };

            template <typename Func>
            class WrappedFiber : public GenericFiber {
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

        class ThreadPool {
            public:
                ThreadPool();
                
                template <typename Fn>
                explicit ThreadPool(std::initializer_list<Fn> fns) {
                    for (typename std::initializer_list<Fn>::iterator it = fns.begin(); it != fns.end(); ++it) {
                        add(*it);
                    }
                }

                ThreadPool(const ThreadPool &) = delete;
                ThreadPool &operator =(const ThreadPool &) = delete;

                virtual ~ThreadPool();

                template <typename Fn,
                          typename... Arg,
                          typename = boost::context::detail::disable_overload<boost::fibers::fiber, Fn>,
                          typename = boost::context::detail::disable_overload<boost::fibers::launch, Fn>,
                          typename = boost::context::detail::disable_overload<std::allocator_arg_t, Fn>>
                void add(Fn &&fn, Arg &&...arg) {
                    auto func = bind(fn, std::forward<Arg>(arg)...);
                    functions.emplace_back(new detail::WrappedFiber<decltype(func)>(func));
                }

            private:
                void run();

                std::vector<std::unique_ptr<detail::GenericFiber>> functions;
        };
    }
}

#endif
