#include <memory>
#include <spacepi/dtc/main/Main.hpp>
#include <spacepi/dtc/main/MainImpl.hpp>

using namespace std;
using namespace spacepi::dtc::main;

shared_ptr<Main> Main::instance(new MainImpl());
