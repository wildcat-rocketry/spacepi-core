#include <memory>
#include <spacepi/dtc/emitter/Emitter.hpp>
#include <spacepi/dtc/emitter/EmitterImpl.hpp>

using namespace std;
using namespace spacepi::dtc::emitter;

shared_ptr<Emitter> Emitter::instance(new EmitterImpl());
