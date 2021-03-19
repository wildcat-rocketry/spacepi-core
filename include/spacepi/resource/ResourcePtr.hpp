#ifndef SPACEPI_CORE_RESOURCE_RESOURCEPTR_HPP
#define SPACEPI_CORE_RESOURCE_RESOURCEPTR_HPP

#include <exception>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <spacepi/resource/ResourceFactory.hpp>
#include <spacepi/util/CommandInternals.hpp>
#include <spacepi/util/Exception.hpp>

namespace spacepi {
    namespace resource {
        /**
         * \brief A pointer to a hardware resource which can be parsed out of command-line arguments
         * 
         * \tparam Type The type of resource
         */
        template <typename Type>
        class ResourcePtr final {
            public:
                /**
                 * \brief Copy-assign a ResourcePtr from a \c std::shared_ptr
                 * 
                 * \param[in] ptr The \c std::shared_ptr containing the resource
                 * 
                 * \return \c this
                 */
                ResourcePtr &operator =(const std::shared_ptr<Type> &ptr) {
                    res = ptr;
                    return *this;
                }

                /**
                 * \brief Move-assign a ResourcePtr from a \c std::shared_ptr
                 * 
                 * \param[in,out] ptr The \c std::shared_ptr containing the resource
                 * 
                 * \return \c this
                 */
                ResourcePtr &operator =(std::shared_ptr<Type> &&ptr) {
                    res = std::move(ptr);
                    return *this;
                }

                /**
                 * \brief Get a reference to the underlying resource object
                 * 
                 * \return The reference
                 */
                Type &operator *() {
                    return *res;
                }

                /**
                 * \copydoc operator*
                 */
                const Type &operator *() const {
                    return *res;
                }

                /**
                 * \brief Call a function on the underlying resource object
                 */
                Type *operator ->() {
                    return res.get();
                }

                /**
                 * \copydoc operator->
                 */
                Type *operator ->() const {
                    return res.get();
                }

                /**
                 * \brief Get a pointer to the underlying resource object
                 * 
                 * \return The pointer
                 */
                operator Type *() {
                    return res.get();
                }

                /**
                 * \brief Get a pointer to the underlying resource object
                 * 
                 * \return The pointer
                 */
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
                                    throw EXCEPTION(ResourceException("Resource factory not found"));
                                }
                                valid = true;
                            } catch (const std::exception &e) {
                                return std::make_pair(res.second.first, "Unknown resource " + res.first + ": " + e.what());
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
