#include <ctime>
#include <chrono>
#include <ostream>
#include <spacepi/log/ConsoleTarget.hpp>
#include <spacepi/log/Entry.hpp>

using namespace std;
using namespace std::chrono;
using namespace spacepi::log;

ConsoleTarget::ConsoleTarget(ostream &os) : os(os) {
}

void ConsoleTarget::operator <<(const Entry &entry) {
    char buffer[20]; // "2020-10-12 09:45:12\0"
    time_t time = system_clock::to_time_t(entry.getTime());
    strftime(buffer, sizeof(buffer), "%F %T", localtime(&time));
    const string &msg = entry.getMessage();
    for (string::const_iterator it = msg.begin(); it != msg.end(); ) {
        os << "[" << buffer << "] [" << entry.getLevel() << "] [" << entry.getTag() << "] ";
        for (; it != msg.end() && *it != '\n'; ++it) {
            os << *it;
        }
        if (it != msg.end()) {
            ++it;
        }
        os << endl;
    }
    os.flush();
}
