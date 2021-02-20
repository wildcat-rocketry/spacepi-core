#include <cstddef>
#include <cstring>
#include <initializer_list>
#include <istream>
#include <memory>
#include <ostream>
#include <string>
#include <utility>
#include <vector>
#include <boost/asio.hpp>
#include <boost/process.hpp>
#include <boost/system/error_code.hpp>
#include <SpacePi.hpp>
#include <spacepi/liblinux/UniqueProcess.hpp>

using namespace std;
using namespace boost;
using namespace boost::asio;
using namespace boost::process;
using namespace spacepi::concurrent;
using namespace spacepi::log;
using namespace spacepi::messaging::network;
using namespace spacepi::liblinux;
using namespace spacepi::liblinux::detail;

OutputStreamCallbacks::OutputStreamCallbacks(const std::shared_ptr<OutputStream> &os) noexcept : os(os) {
}

pair<buffers_iterator<asio::streambuf::const_buffers_type>, bool> OutputStreamCallbacks::operator ()(buffers_iterator<asio::streambuf::const_buffers_type> begin, buffers_iterator<asio::streambuf::const_buffers_type> end) {
    for (buffers_iterator<asio::streambuf::const_buffers_type> it = begin; it != end; ++it) {
        if (*it == '\n') {
            return make_pair(it + 1, true);
        }
    }
    return make_pair(end, false);
}

void OutputStreamCallbacks::operator ()(const system::error_code &err, size_t count) {
    std::shared_ptr<OutputStream> s = os.lock();
    if (s) {
        s->handleRead(count);
    }
}

OutputStream::OutputStream(bool use, Logger &log, LogLevel level) : use(use), log(log), level(level), pipe(NetworkThread::instance.getContext()), fail(false) {
}

async_pipe &OutputStream::getPipe() noexcept {
    return pipe;
}

void OutputStream::handleRead(size_t count) {
    if (count > 0) {
        const char *payload = (const char *) buf.data().data();
        size_t newline;
        for (newline = 0; newline < count; ++newline) {
            if (payload[newline] == '\n') {
                ++newline;
                break;
            }
        }
        log(level) << string(payload, newline - 1);
        if (use) {
            UniqueConditionVariableLock lck(cond);
            readQueue.emplace(payload, newline);
            cond.notify_all();
        }
        buf.consume(count);
        start();
    } else {
        UniqueConditionVariableLock lck(cond);
        fail = true;
        cond.notify_all();
    }
}

void OutputStream::start() {
    OutputStreamCallbacks cb(shared_from_this());
    async_read_until(pipe, buf, cb, cb);
}

void OutputStream::join() {
    UniqueConditionVariableLock lck(cond);
    while (!fail) {
        cond.wait(lck);
    }
}

streamsize OutputStream::showmanyc() {
    UniqueConditionVariableLock lck(cond);
    if (fail) {
        return -1;
    }
    return readQueue.front().size();
}

streamsize OutputStream::xsgetn(char *s, std::streamsize n) {
    UniqueConditionVariableLock lck(cond);
    while (!fail && readQueue.empty()) {
        cond.wait(lck);
    }
    if (fail) {
        return 0;
    }
    string &line = readQueue.front();
    if (line.size() < n) {
        strncpy(s, line.c_str(), line.size());
        readQueue.pop();
        return line.size();
    } else {
        strncpy(s, line.c_str(), n);
        line = line.substr(n);
        return n;
    }
}

int OutputStream::underflow() {
    UniqueConditionVariableLock lck(cond);
    while (!fail && readQueue.empty()) {
        cond.wait(lck);
    }
    if (fail) {
        return EOF;
    }
    return readQueue.front().front();
}

int OutputStream::uflow() {
    char c;
    if (xsgetn(&c, 1) == 1) {
        return c;
    }
    return EOF;
}

UniqueProcess::UniqueProcess(bool useInput, bool useOutput, bool useError, const string &exe, const initializer_list<string> &args) : UniqueProcess(useInput, useOutput, useError, exe, vector<string>(args)) {
}

UniqueProcess::UniqueProcess(bool useInput, bool useOutput, bool useError, const string &exe, const vector<string> &args) :
    log(getLogName(exe)),
    stdoutBuf(new OutputStream(useOutput, log, LogLevel::Info)), stderrBuf(new OutputStream(useError, log, LogLevel::Warning)),
    stdoutStream(stdoutBuf.get()), stderrStream(stderrBuf.get()),
    proc(exe, argv = args, std_in < stdinStream, std_out > stdoutBuf->getPipe(), std_err > stderrBuf->getPipe()) {
    if (!useInput) {
        closeInput();
    }
    stdoutBuf->start();
    stderrBuf->start();
    NetworkThread::instance.start();
}

ostream &UniqueProcess::input() noexcept {
    return stdinStream;
}

istream &UniqueProcess::output() noexcept {
    return stdoutStream;
}

istream &UniqueProcess::error() noexcept {
    return stderrStream;
}

void UniqueProcess::closeInput() {
    stdinStream.flush();
    stdinStream.pipe().close();
}

bool UniqueProcess::running() {
    return proc.running();
}

void UniqueProcess::wait() {
    proc.join();
    stdoutBuf->join();
    stderrBuf->join();
}

int UniqueProcess::getExitCode() const {
    return proc.exit_code();
}

string UniqueProcess::getLogName(const string &exe) noexcept {
    size_t slash = exe.find_last_of('/');
    if (slash == string::npos) {
        return "spacepi:liblinux:proc:" + exe;
    } else {
        return "spacepi:liblinux:proc:" + exe.substr(slash + 1);
    }
}
