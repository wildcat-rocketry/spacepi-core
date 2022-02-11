#include <memory>
#include <vector>
#include <spacepi/test/dtc/MockIncludeFinder.hpp>

using namespace std;
using namespace spacepi::dtc::includer;
using namespace spacepi::test::dtc;

MockIncludeFinder::MockIncludeFinder() noexcept: old(IncludeFinder::instance)
{
    IncludeFinder::instance.reset(new Impl());
}

MockIncludeFinder::~MockIncludeFinder() noexcept
{
    IncludeFinder::instance = old;
}

string MockIncludeFinder::Impl::locate(const string &filename, const vector<string> &includeDirs) const noexcept {
    return filename;
}
