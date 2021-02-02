#include <istream>
#include <memory>
#include <ostream>
#include <spacepi/liblinux/SharedProcess.hpp>

using namespace std;
using namespace spacepi::liblinux;

ostream &SharedProcess::input() noexcept {
    return unique->input();
}

istream &SharedProcess::output() noexcept {
    return unique->output();
}

istream &SharedProcess::error() noexcept {
    return unique->error();
}

void SharedProcess::closeInput() {
    unique->closeInput();
}

bool SharedProcess::running() {
    return unique->running();
}

void SharedProcess::wait() {
    unique->wait();
}

int SharedProcess::getExitCode() const {
    return unique->getExitCode();
}
