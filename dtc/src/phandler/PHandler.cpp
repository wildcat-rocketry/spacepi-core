#include <memory>
#include <spacepi/dtc/phandler/PHandler.hpp>
#include <spacepi/dtc/phandler/PHandlerImpl.hpp>

using namespace std;
using namespace spacepi::dtc::phandler;

shared_ptr<PHandler> PHandler::instance(new PHandlerImpl());
