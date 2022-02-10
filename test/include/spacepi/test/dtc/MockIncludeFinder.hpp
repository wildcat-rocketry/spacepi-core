#ifndef SPACEPI_CORE_TEST_DTC_MOCKINCLUDEFINDER_HPP
#define SPACEPI_CORE_TEST_DTC_MOCKINCLUDEFINDER_HPP

#include <spacepi/dtc/includer/IncludeFinder.hpp>
#include <string>
#include <unordered_map>
#include <vector>

namespace spacepi {
    namespace test {
        namespace dtc {
            class MockIncludeFinder {
                public:
                    MockIncludeFinder() noexcept;
                    ~MockIncludeFinder() noexcept;

                private:
                    class Impl : public spacepi::dtc::includer::IncludeFinder {
                        public:
                            const std::string &locate(const std::string &filename, const std::vector<std::string> &includeDirs) const noexcept;
                    };

                    std::shared_ptr<spacepi::dtc::includer::IncludeFinder> old;
            };
        }
    }
}

#endif
