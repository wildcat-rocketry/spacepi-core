#ifndef SPACEPI_TARGETLIB_LINUX_INSTALLATIONDATA_HPP
#define SPACEPI_TARGETLIB_LINUX_INSTALLATIONDATA_HPP

#include <memory>
#include <SpacePi.hpp>
#include <utility>
#include <vector>

namespace spacepi {
    namespace liblinux {
        namespace detail {
            class GenericInstallationDataAccessor {
                public:
                    virtual void *getData() noexcept = 0;
                    virtual const void *getData() const noexcept = 0;

                protected:
                    static int getNextID();
            };

            template <typename Type>
            class InstallationDataAccessor : public GenericInstallationDataAccessor {
                public:
                    template <typename... Args>
                    InstallationDataAccessor(Args... args) : obj(std::forward<Args>(args)...) {
                    }

                    void *getData() noexcept {
                        return &obj;
                    }

                    const void *getData() const noexcept {
                        return &obj;
                    }

                    static int getID() noexcept {
                        return id;
                    }

                private:
                    static int id;
                    Type obj;
            };

            template <typename Type>
            int InstallationDataAccessor<Type>::id = getNextID();
        }

        class InstallationData {
            public:
                template <typename Type, typename... Args>
                void initData(Args... args) {
                    int id = detail::InstallationDataAccessor<Type>::getID();
                    if (id >= data.size()) {
                        data.resize(id + 1);
                    }
                    data[id].reset(new detail::InstallationDataAccessor<Type>(std::forward<Args>(args)...));
                }

                template <typename Type>
                Type &getData() {
                    int id = detail::InstallationDataAccessor<Type>::getID();
                    if (id >= data.size()) {
                        throw EXCEPTION(spacepi::util::StateException("Cannot get data that has not been initialized."));
                    }
                    return *(Type *) data[id]->getData();
                }

                template <typename Type>
                const Type &getData() const {
                    int id = detail::InstallationDataAccessor<Type>::getID();
                    if (id >= data.size()) {
                        throw EXCEPTION(spacepi::util::StateException("Cannot get data that has not been initialized."));
                    }
                    return *(const Type *) data[id]->getData();
                }

                template <typename Type>
                void deleteData() {
                    int id = detail::InstallationDataAccessor<Type>::getID();
                    if (id >= data.size()) {
                        data.resize(id + 1);
                    }
                    data[id].reset();
                }

            private:
                std::vector<std::shared_ptr<detail::GenericInstallationDataAccessor>> data;
        };
    }
}

#endif
