#include <memory>
#include <string>
#include <vector>
#include <spacepi/dtc/includer/IncludeFinder.hpp>
#include <spacepi/dtc/includer/IncludeFinderImpl.hpp>

using namespace std;
using namespace spacepi::dtc::includer;

shared_ptr<IncludeFinder> IncludeFinder::instance(new IncludeFinderImpl());
