#include <initializer_list>
#include <istream>
#include <memory>
#include <ostream>
#include <string>
#include <vector>
#include <spacepi/liblinux/SharedProcess.hpp>

using namespace std;
using namespace spacepi::liblinux;

SharedProcess::SharedProcess(bool useInput, bool useOutput, bool useError, const string &exe, const initializer_list<string> &args) : unique(new UniqueProcess(useInput, useOutput, useError, exe, args)) {
}

SharedProcess::SharedProcess(bool useInput, bool useOutput, bool useError, const string &exe, const vector<string> &args) : unique(new UniqueProcess(useInput, useOutput, useError, exe, args)) {
}

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
