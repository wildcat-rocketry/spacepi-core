#ifndef SPACEPI_CORE_RESOURCE_RESOURCEFACTORY_HPP
#define SPACEPI_CORE_RESOURCE_RESOURCEFACTORY_HPP

#include <memory>
#include <string>
#include <unordered_map>
#include <spacepi/util/Exception.hpp>

namespace spacepi {
    namespace resource {
        template <typename Type>
        class ResourceFactory {
            public:
                ResourceFactory(const std::string &prefix) {
                    std::unordered_map<std::string, ResourceFactory<Type> *> &factories = getFactories();
                    typename std::unordered_map<std::string, ResourceFactory<Type> *>::iterator it = factories.find(prefix);
                    if (it != factories.end()) {
                        throw EXCEPTION(spacepi::util::ResourceException("Resource already registered."));
                    }
                    factories[prefix] = this;
                }

                ResourceFactory(ResourceFactory<Type> &) = delete;
                ResourceFactory<Type> &operator =(ResourceFactory<Type> &) = delete;

                static std::shared_ptr<Type> get(const std::string &name) {
                    std::unordered_map<std::string, ResourceFactory<Type> *> &factories = getFactories();
                    size_t colon = name.find_first_of(':');
                    std::string factoryName;
                    std::string resourceName;
                    if (colon == std::string::npos) {
                        factoryName = name;
                    } else {
                        factoryName = name.substr(0, colon);
                        resourceName = name.substr(colon + 1);
                    }
                    typename std::unordered_map<std::string, ResourceFactory<Type> *>::iterator it = factories.find(factoryName);
                    if (it == factories.end()) {
                        return std::shared_ptr<Type>();
                    }
                    return it->second->create(resourceName);
                }

            private:
                virtual std::shared_ptr<Type> create(const std::string &name) = 0;

                static std::unordered_map<std::string, ResourceFactory<Type> *> &getFactories() noexcept {
                    // Can't use a static member variable for this due to initialization order
                    static std::unordered_map<std::string, ResourceFactory<Type> *> factories;
                    return factories;
                }
        };
    }
}

#endif
