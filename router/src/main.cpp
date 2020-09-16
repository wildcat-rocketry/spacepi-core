#include <functional>
#include <iostream>
#include <spacepi/concurrent/Sleep.hpp>
#include <spacepi/concurrent/ThreadPool.hpp>
using namespace std;
using namespace spacepi::concurrent;

static void threadFunc(const char *name);

int main(int argc, const char **argv) {
    ThreadPool {
        bind(threadFunc, "Thread A"),
        bind(threadFunc, "Thread B"),
        bind(threadFunc, "Thread C")
    };
    return 0;
}

void threadFunc(const char *name) {
    for (int i = 0; i < 10; ++i) {
        cout << name << " (" << i << ")" << endl;
        Sleep::momentarily();
    }
}
