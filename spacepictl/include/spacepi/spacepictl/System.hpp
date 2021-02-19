#ifndef SPACEPI_TARGETLIB_LINUX_SPACEPICTL_SYSTEM_HPP
#define SPACEPI_TARGETLIB_LINUX_SPACEPICTL_SYSTEM_HPP

#include <string>
#include <vector>
#include <pwd.h>
#include <grp.h>
#include <shadow.h>
#include <SpacePi.hpp>
#include <spacepi/spacepictl/FSTransaction.hpp>

namespace spacepi {
    namespace spacepictl {
        class System : private spacepi::log::AutoLog<decltype("spacepictl:System"_autolog)> {
            public:
                System(spacepi::spacepictl::FSTransaction &fs, const spacepi::package::Options &options, const std::unordered_map<std::string, spacepi::package::Module> &modules);

                void write_updates();

                static std::string moduleBin(spacepi::package::Module &module);
                static std::string moduleServiceName(spacepi::package::Module &module);
                static bool isServiceName(const std::string &name);

            protected:

            private:
                const std::unordered_map<std::string, spacepi::package::Module> &modules;

                bool update_hostname;
                bool update_ip;

                std::string ip;
                std::string hostname;

                std::string old_ip;
                std::string old_hostname;

                spacepi::spacepictl::FSTransaction &fs;

                void fetch_ip();
                void fetch_hostname();

                void write_ip();
                void write_hostname();
                void write_services();
                void delete_service_from_dir(std::string dir);
        };
    }
}

#endif
