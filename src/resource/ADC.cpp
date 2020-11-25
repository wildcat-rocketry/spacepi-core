#include <exception>
#include <memory>
#include <string>
#include <spacepi/log/AutoLog.hpp>
#include <spacepi/log/LogLevel.hpp>
#include <spacepi/resource/ADC.hpp>
#include <spacepi/resource/ResourceFactory.hpp>

namespace spacepi {
    namespace resource {
        namespace detail {
            class MockADC : public ADC, private spacepi::log::AutoLog<decltype("core:resource:mock:adc"_autolog)> {
                public:
                    MockADC(double val, const std::string &name) noexcept;

                    operator double() const noexcept;

                private:
                    double val;
                    std::string name;
            };

            class MockADCFactory : public ResourceFactory<ADC> {
                public:
                    static MockADCFactory instance;

                    MockADCFactory();

                private:
                    std::shared_ptr<ADC> create(const std::string &name);
            };
        }
    }
}

using namespace std;
using namespace spacepi::log;
using namespace spacepi::resource;
using namespace spacepi::resource::detail;

MockADCFactory MockADCFactory::instance;

MockADC::MockADC(double val, const string &name) noexcept : val(val), name(name) {
    log(LogLevel::Debug) << "Created new ADC '" << name << "'.";
}

MockADC::operator double() const noexcept {
    log(LogLevel::Debug) << "Reading ADC '" << name << "'.";
    return val;
}

MockADCFactory::MockADCFactory() : ResourceFactory<ADC>("mock") {
}

shared_ptr<ADC> MockADCFactory::create(const string &name) {
    try {
        return shared_ptr<ADC>(new MockADC(stod(name), name));
    } catch (const exception &) {
    }
    return shared_ptr<ADC>(new MockADC(0.5, name));
}

shared_ptr<ADC> ADC::get(const string &name) {
    return ResourceFactory<ADC>::get(name);
}
