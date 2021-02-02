#include <cstddef>
#include <fstream>
#include <istream>
#include <ostream>
#include <stack>
#include <string>
#include <unordered_map>
#include <utility>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <spacepi/package/Module.hpp>
#include <spacepi/package/Options.hpp>
#include <spacepi/package/PackageConfig.hpp>
#include <spacepi/package/User.hpp>
#include <spacepi/util/Exception.hpp>

using namespace std;
using namespace boost::property_tree;
using namespace spacepi::package;
using namespace spacepi::util;

PackageConfig::PackageConfig(const string &filename) {
    load(filename);
}

void PackageConfig::load(const string &filename) {
    reset();
    stack<string> includes;
    includes.push(filename);
    while (!includes.empty()) {
        ptree pt;
        const string &file = includes.top();
        ifstream is(file);
        if (!is) {
            throw EXCEPTION(ArgumentException("Unable to find config XML file '" + file + "'."));
        }
        read_xml(is, pt);
        is.close();
        size_t slash = file.find_last_of('/');
        string dir = "";
        if (slash != string::npos) {
            dir = file.substr(0, slash + 1);
        }
        includes.pop();
        for (ptree::const_iterator rit = pt.begin(); rit != pt.end(); ++rit) {
            if (rit->first == "config") {
                for (ptree::const_iterator cit = rit->second.begin(); cit != rit->second.end(); ++cit) {
                    if (cit->first == "include") {
                        includes.push(dir + cit->second.data());
                    } else if (cit->first == "target") {
                        for (ptree::const_iterator tit = cit->second.begin(); tit != cit->second.end(); ++tit) {
                            if (tit->first == "type") {
                                setTargetType(tit->second.data());
                            } else if (tit->first == "name") {
                                setTargetName(tit->second.data());
                            } else if (tit->first == "options") {
                                for (ptree::const_iterator oit = tit->second.begin(); oit != tit->second.end(); ++oit) {
                                    getTargetOptions().addOption(oit->first, oit->second.data());
                                }
                            } else {
                                throw EXCEPTION(ArgumentException("Unknown target XML tag '" + tit->first + "'."));
                            }
                        }
                    } else if (cit->first == "users") {
                        for (ptree::const_iterator lit = cit->second.begin(); lit != cit->second.end(); ++lit) {
                            if (lit->first == "user") {
                                User user;
                                for (ptree::const_iterator uit = lit->second.begin(); uit != lit->second.end(); ++uit) {
                                    if (uit->first == "name") {
                                        user.setName(uit->second.data());
                                    } else if (uit->first == "uname") {
                                        user.setUsername(uit->second.data());
                                    } else if (uit->first == "email") {
                                        user.setEmail(uit->second.data());
                                    } else if (uit->first == "key") {
                                        user.getKeys().push_back(uit->second.data());
                                    } else {
                                        throw EXCEPTION(ArgumentException("Unknown user XML tag '" + cit->first + "'."));
                                    }
                                }
                                getUsers()[user.getUsername()] = user;
                            } else {
                                throw EXCEPTION(ArgumentException("Unknown users XML tag '" + cit->first + "'."));
                            }
                        }
                    } else if (cit->first == "modules") {
                        for (ptree::const_iterator lit = cit->second.begin(); lit != cit->second.end(); ++lit) {
                            if (lit->first == "module") {
                                Module mod;
                                mod.setAutomaticStart(true);
                                for (ptree::const_iterator mit = lit->second.begin(); mit != lit->second.end(); ++mit) {
                                    if (mit->first == "name") {
                                        mod.setName(mit->second.data());
                                    } else if (mit->first == "type") {
                                        mod.setName(mit->second.data());
                                    } else if (mit->first == "start") {
                                        if (mit->second.data() == "manual") {
                                            mod.setAutomaticStart(false);
                                        } else {
                                            throw EXCEPTION(ArgumentException("Unknown start mode '" + cit->first + "'."));
                                        }
                                    } else if (mit->first == "options") {
                                        for (ptree::const_iterator oit = mit->second.begin(); oit != mit->second.end(); ++oit) {
                                            mod.getOptions().addOption(oit->first, oit->second.data());
                                        }
                                    } else {
                                        throw EXCEPTION(ArgumentException("Unknown module XML tag '" + cit->first + "'."));
                                    }
                                }
                                getModules()[mod.getName()] = mod;
                            } else {
                                throw EXCEPTION(ArgumentException("Unknown modules XML tag '" + cit->first + "'."));
                            }
                        }
                    } else {
                        throw EXCEPTION(ArgumentException("Unknown top-level XML tag '" + cit->first + "'."));
                    }
                }
            } else {
                throw EXCEPTION(ArgumentException("Unknown XML root tag '" + rit->first + "'."));
            }
        }
    }
}

void PackageConfig::save(ostream &os) {
    ptree pt;
    pt.put("config.target.type", getTargetType());
    pt.put("config.target.name", getTargetName());
    for (Options::ConstIterator it = getTargetOptions().begin(); it != getTargetOptions().end(); ++it) {
        pt.put("config.target.options." + it->getKey(), it->getValue());
    }
    ptree users;
    for (unordered_map<string, User>::const_iterator uit = getUsers().begin(); uit != getUsers().end(); ++uit) {
        ptree user;
        user.put("name", uit->second.getName());
        user.put("uname", uit->second.getUsername());
        user.put("email", uit->second.getEmail());
        for (vector<string>::const_iterator kit = uit->second.getKeys().begin(); kit != uit->second.getKeys().end(); ++kit) {
            ptree key;
            key.data() = *kit;
            user.push_back(make_pair("key", key));
        }
        users.push_back(make_pair("user", users));
    }
    pt.put_child("config.users", users);
    ptree modules;
    for (unordered_map<string, Module>::const_iterator mit = getModules().begin(); mit != getModules().end(); ++mit) {
        ptree mod;
        mod.put("name", mit->second.getName());
        mod.put("type", mit->second.getType());
        if (!mit->second.hasAutomaticStart()) {
            mod.put("start", "manual");
        }
        ptree options;
        for (Options::ConstIterator oit = mit->second.getOptions().begin(); oit != mit->second.getOptions().end(); ++oit) {
            ptree opt;
            opt.data() = oit->getValue();
            options.push_back(make_pair(oit->getKey(), opt));
        }
        mod.put_child("options", options);
        modules.push_back(make_pair("module", mod));
    }
    pt.put_child("config.modules", modules);
    write_xml(os, pt);
}

void PackageConfig::save(const string &filename) {
    ofstream os(filename);
    save(os);
}

void PackageConfig::reset() noexcept {
    targetType = "";
    targetName = "";
    targetOptions.clear();
    users.clear();
    modules.clear();
}

const string &PackageConfig::getTargetType() const noexcept {
    return targetType;
}

void PackageConfig::setTargetType(const string &targetType) noexcept {
    this->targetType = targetType;
}

const string &PackageConfig::getTargetName() const noexcept {
    return targetName;
}

void PackageConfig::setTargetName(const string &targetName) noexcept {
    this->targetName = targetName;
}

const Options &PackageConfig::getTargetOptions() const noexcept {
    return targetOptions;
}

Options &PackageConfig::getTargetOptions() noexcept {
    return targetOptions;
}

const unordered_map<string, User> &PackageConfig::getUsers() const noexcept {
    return users;
}

unordered_map<string, User> &PackageConfig::getUsers() noexcept {
    return users;
}

const unordered_map<string, Module> &PackageConfig::getModules() const noexcept {
    return modules;
}

unordered_map<string, Module> &PackageConfig::getModules() noexcept {
    return modules;
}
