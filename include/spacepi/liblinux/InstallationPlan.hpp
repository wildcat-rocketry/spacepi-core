#ifndef SPACEPI_TARGETLIB_LINUX_INSTALLATIONPLAN_HPP
#define SPACEPI_TARGETLIB_LINUX_INSTALLATIONPLAN_HPP

#include <list>
#include <memory>
#include <type_traits>
#include <utility>
#include <spacepi/liblinux/Image.hpp>
#include <spacepi/liblinux/InstallationData.hpp>
#include <spacepi/liblinux/InstallationStep.hpp>

namespace spacepi {
    namespace liblinux {
        namespace detail {
            class GenericTypedInstaller : public InstallationStep {
                public:
                    virtual int getID() const noexcept = 0;
                    virtual void *getData() noexcept = 0;
                    virtual const void *getData() const noexcept = 0;

                protected:
                    static int getNextID();
            };

            template <typename Type, typename std::enable_if<std::is_base_of<InstallationStep, Type>::value>::type * = nullptr>
            class TypedInstaller : public GenericTypedInstaller {
                public:
                    template <typename... Args>
                    TypedInstaller(Args... args) : obj(std::forward<Args>(args)...) {
                    }

                    void run(InstallationData &data) {
                        obj.run(data);
                    }

                    int getID() const noexcept {
                        return id;
                    }

                    static int getID2() noexcept {
                        return id;
                    }

                    void *getData() noexcept {
                        return &obj;
                    }

                    const void *getData() const noexcept {
                        return &obj;
                    }

                private:
                    static int id;
                    Type obj;
            };

            template <typename Type, typename std::enable_if<std::is_base_of<InstallationStep, Type>::value>::type *Dummy>
            int TypedInstaller<Type, Dummy>::id = getNextID();
        }

        class InstallationPlan {
            public:
                template <typename Type, typename... Args>
                void prependStep(Args... args) {
                    int id = detail::TypedInstaller<Type>::getID2();
                    throwIfContains(id);
                    steps.emplace_front(std::make_unique<detail::TypedInstaller<Type>>(std::forward<Args>(args)...));
                }

                template <typename Type, typename... Args>
                void appendStep(Args... args) {
                    int id = detail::TypedInstaller<Type>::getID2();
                    throwIfContains(id);
                    steps.emplace_back(std::make_unique<detail::TypedInstaller<Type>>(std::forward<Args>(args)...));
                }

                template <typename Before, typename Type, typename... Args>
                void insertStepAfter(Args... args) {
                    int id = detail::TypedInstaller<Type>::getID2();
                    int bid = detail::TypedInstaller<Before>::getID2();
                    throwIfContains(id);
                    steps.emplace(++findOrThrow(bid), std::make_unique<detail::TypedInstaller<Type>>(std::forward<Args>(args)...));
                }

                template <typename After, typename Type, typename... Args>
                void insertStepBefore(Args... args) {
                    int id = detail::TypedInstaller<Type>::getID2();
                    int aid = detail::TypedInstaller<After>::getID2();
                    throwIfContains(id);
                    steps.emplace(findOrThrow(aid), std::make_unique<detail::TypedInstaller<Type>>(std::forward<Args>(args)...));
                }

                template <typename Type>
                void removeStep() {
                    int id = detail::TypedInstaller<Type>::getID2();
                    std::list<std::unique_ptr<detail::GenericTypedInstaller>>::const_iterator it = find(id);
                    if (it != steps.end()) {
                        steps.erase(it);
                    }
                }

                template <typename Type>
                const Type &getStep() const {
                    int id = detail::TypedInstaller<Type>::getID2();
                    return *(Type *) (*findOrThrow(id))->getData();
                }

                template <typename Type>
                inline Type &getStep() {
                    return const_cast<Type &>(((const InstallationPlan &) *this).getStep<Type>());
                }

                void install(const InstallationData &data);

            private:
                std::list<std::unique_ptr<detail::GenericTypedInstaller>>::const_iterator find(int id) const;
                std::list<std::unique_ptr<detail::GenericTypedInstaller>>::const_iterator findOrThrow(int id) const;
                void throwIfContains(int id) const;

                std::list<std::unique_ptr<detail::GenericTypedInstaller>> steps;
        };
    }
}

#endif
