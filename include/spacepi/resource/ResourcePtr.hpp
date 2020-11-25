#ifndef SPACEPI_CORE_RESOURCE_RESOURCEPTR_HPP
#define SPACEPI_CORE_RESOURCE_RESOURCEPTR_HPP

#include <exception>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <spacepi/resource/ResourceFactory.hpp>
#include <spacepi/util/CommandInternals.hpp>

namespace spacepi {
    namespace resource {
        template <typename Type>
        class ResourcePtr {
            public:
                ResourcePtr &operator =(const std::shared_ptr<Type> &ptr) {
                    res = ptr;
                    return *this;
                }

                ResourcePtr &operator =(std::shared_ptr<Type> &&ptr) {
                    res = std::move(ptr);
                    return *this;
                }

                Type &operator *() {
                    return *res;
                }

                const Type &operator *() const {
                    return *res;
                }

                Type *operator ->() {
                    return res.get();
                }

                Type *operator ->() const {
                    return res.get();
                }

                operator Type *() {
                    return res.get();
                }

                operator const Type *() const {
                    return res.get();
                }

            private:
                std::shared_ptr<Type> res;
        };
    }

    namespace util {
        namespace detail {
            template <typename Type>
            class CommandParser<spacepi::resource::ResourcePtr<Type>> : public GenericCommandParser {
                public:
                    CommandParser(spacepi::resource::ResourcePtr<Type> &var, const std::string &name, const std::string &desc) : GenericCommandParser(name, desc, false), var(var) {
                    }

                    CommandParser(spacepi::resource::ResourcePtr<Type> &var, const Type &def, const std::string &name, const std::string &desc) : GenericCommandParser(name, desc, true), var(var) {
                        var = def;
                    }

                    CommandParser(CommandParser<spacepi::resource::ResourcePtr<Type>> &) = delete;
                    CommandParser<spacepi::resource::ResourcePtr<Type>> &operator =(CommandParser<spacepi::resource::ResourcePtr<Type>> &) = delete;

                    std::pair<std::vector<std::string>::const_iterator, std::string> parse(const std::vector<std::string> &args, const std::vector<std::string>::const_iterator &start) noexcept {
                        std::pair<std::string, std::pair<std::vector<std::string>::const_iterator, std::string>> res = parseNormal(args, start);
                        if (!res.first.empty()) {
                            try {
                                var = std::move(spacepi::resource::ResourceFactory<Type>::get(res.first));
                                if (!var) {
                                    throw std::exception();
                                }
                                valid = true;
                            } catch (const std::exception &) {
                                return std::make_pair(res.second.first, "Unknown resource " + res.first);
                            }
                        }
                        return res.second;
                    }

                    std::string example() const noexcept {
                        return "--" + name + "=factory:instance";
                    }

                private:
                    spacepi::resource::ResourcePtr<Type> &var;
            };
        }
    }
}

#endif
