#ifndef SPACEPI_UTIL_COMMANDINTERNALS_HPP
#define SPACEPI_UTIL_COMMANDINTERNALS_HPP

#include <memory>
#include <string>
#include <vector>
#include <utility>

namespace spacepi {
    namespace util {
        class Command;
        class CommandConfigurable;

        namespace detail {
            class GenericCommandParser {
                friend class spacepi::util::Command;

                public:
                    operator bool() const noexcept;

                    virtual std::pair<std::vector<std::string>::const_iterator, std::string> parse(const std::vector<std::string> &args, const std::vector<std::string>::const_iterator &start) noexcept = 0;
                    virtual std::string example() const noexcept = 0;

                protected:
                    GenericCommandParser(const std::string &name, const std::string &desc, bool valid);

                    std::pair<std::string, std::pair<std::vector<std::string>::const_iterator, std::string>> parseNormal(const std::vector<std::string> &args, const std::vector<std::string>::const_iterator &start) noexcept;

                    template <typename Element>
                    std::pair<std::vector<std::string>::const_iterator, std::string> parseVector(const std::vector<std::string> &args, const std::vector<std::string>::const_iterator &start, std::vector<Element> &var) noexcept;

                    std::string name;
                    std::string desc;
                    bool valid;
            };

            template <typename Type>
            class CommandParser : public GenericCommandParser {
                public:
                    CommandParser(Type &var, const std::string &name, const std::string &desc) : GenericCommandParser(name, desc, false), var(var) {
                    }

                    CommandParser(Type &var, const Type &def, const std::string &name, const std::string &desc) : GenericCommandParser(name, desc, true), var(var) {
                        var = def;
                    }

                    CommandParser(CommandParser<Type> &) = delete;
                    CommandParser<Type> &operator =(CommandParser<Type> &) = delete;

                    std::pair<std::vector<std::string>::const_iterator, std::string> parse(const std::vector<std::string> &args, const std::vector<std::string>::const_iterator &start) noexcept;
                    std::string example() const noexcept;

                private:
                    Type &var;
            };

            template <typename Element>
            std::pair<std::vector<std::string>::const_iterator, std::string> GenericCommandParser::parseVector(const std::vector<std::string> &args, const std::vector<std::string>::const_iterator &start, std::vector<Element> &var) noexcept {
                Element el;
                CommandParser<Element> parser(el, name, "");
                std::pair<std::vector<std::string>::const_iterator, std::string> res = parser.parse(args, start);
                if (res.first != start && res.second.empty()) {
                    var.push_back(std::move(el));
                    valid = true;
                }
                return res;
            }

            class CommandGroup {
                friend class spacepi::util::Command;

                public:
                    CommandGroup(const std::string &caption, CommandConfigurable &cmd) noexcept;

                    CommandGroup(const CommandGroup &) = default;
                    CommandGroup &operator =(const CommandGroup &copy);

                    void add(std::shared_ptr<GenericCommandParser> &&option);

                private:
                    std::string caption;
                    std::vector<std::shared_ptr<GenericCommandParser>> options;
                    CommandConfigurable &cmd;
            };
        }
    }
}

#endif
