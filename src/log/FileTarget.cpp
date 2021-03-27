#include <ctime>
#include <chrono>
#include <string>
#include <spacepi/log/Entry.hpp>
#include <spacepi/log/FileTarget.hpp>
#include <spacepi/resource/Filesystem.hpp>

using namespace std;
using namespace std::chrono;
using namespace spacepi::log;
using namespace spacepi::resource;

FileTarget::FileTarget(Filesystem &fs) noexcept : stream(fs.open("", true)) {
}

void FileTarget::operator <<(const Entry &entry) {
    char buffer[20]; // "2020-10-12 09:45:12\0"
    time_t time = system_clock::to_time_t(entry.getTime());
    strftime(buffer, sizeof(buffer), "%F %T", localtime(&time));
    const string &msg = entry.getMessage();
    for (string::const_iterator it = msg.begin(); it != msg.end(); ) {
        *stream << "[" << buffer << "] [" << entry.getLevel() << "] [" << entry.getTag() << "] ";
        for (; it != msg.end() && *it != '\n'; ++it) {
            *stream << *it;
        }
        if (it != msg.end()) {
            ++it;
        }
        *stream << '\n';
    }
    stream->flush();
}
