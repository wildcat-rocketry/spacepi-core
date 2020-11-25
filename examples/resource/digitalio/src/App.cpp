#include <chrono>
#include <spacepi/concurrent/Sleep.hpp>
#include <spacepi/example/App.hpp>
#include <spacepi/resource/DigitalIO.hpp>
#include <spacepi/util/Command.hpp>
#include <spacepi/util/CommandConfigurable.hpp>

using namespace std::chrono;
using namespace spacepi::concurrent;
using namespace spacepi::example;
using namespace spacepi::resource;
using namespace spacepi::util;

App::App(Command &cmd) noexcept : CommandConfigurable("App Options", cmd) {
    fromCommand(pin, "pin", "The pin to send a pulse on");
}

void App::runCommand() {
    (*pin) = false;
    pin->setMode(DigitalIO::Output);
    Sleep::duration(seconds(1));
    (*pin) = true;
    Sleep::duration(seconds(1));
    (*pin) = false;
    pin->setMode(DigitalIO::Input);
}
