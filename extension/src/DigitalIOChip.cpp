#include <chrono>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <utility>
#include <gpiod.hpp>
#include <SpacePi.hpp>
#include <spacepi/target/extension/DigitalIO.hpp>
#include <spacepi/target/extension/DigitalIOChip.hpp>
#include <spacepi/target/extension/DigitalIOFactory.hpp>

using namespace std;
using namespace std::chrono;
using namespace gpiod;
using namespace spacepi::util;
using namespace spacepi::target::extension;

DigitalIOChip::DigitalIOChip(DigitalIOFactory &factory, const std::string &devName) : factory(factory), devName(devName), chip(this->devName, gpiod::chip::OPEN_BY_NAME) {
    int numLines = chip.num_lines();
    resources.reserve(numLines);
    while (resources.size() < numLines) {
        resources.emplace_back();
    }
}

DigitalIOChip::~DigitalIOChip() {
    factory.destroyChip(devName);
    if (irqThread.joinable()) {
        irqThread.join();
    }
}

DigitalIOFactory &DigitalIOChip::getFactory() noexcept {
    return factory;
}

shared_ptr<DigitalIO> DigitalIOChip::addLine(const string &name) {
    unordered_map<string, int>::iterator it = resourceIDs.find(name);
    if (it != resourceIDs.end()) {
        shared_ptr<DigitalIO> ptr = resources[it->second].lock();
        if (ptr) {
            return ptr;
        }
    }

    line l;
    int off;
    if(name.substr(0,7) == "OFFSET="){
        // Get by offset instead of name
        off = stoi(name.substr(7));
        if (off >= resources.size()) {
            throw EXCEPTION(ResourceException("Offset is larger than the number of lines"));
        }
        l = chip.get_line(off);
        if (!l) {
            throw EXCEPTION(ResourceException("Unknown line offset"));
        }
    } else {
        l = chip.find_line(name);
        if (!l) {
            throw EXCEPTION(ResourceException("Unknown line name"));
        }
        off = l.offset();
        if (off >= resources.size()) {
            throw EXCEPTION(ResourceException("Got a line with an offset larger than the number of lines"));
        }
    }

    shared_ptr<DigitalIO> ptr(new DigitalIO(shared_from_this(), move(l)));
    resources[off] = weak_ptr<DigitalIO>(ptr);
    resourceIDs.emplace_hint(it, name, off);
    return ptr;
}

void DigitalIOChip::removeLine(const gpiod::line &line) {
    resources[line.offset()].reset();
    resourceIDs.erase(line.name());
}

void DigitalIOChip::addInterrupt(const gpiod::line &line) {
    unique_lock<mutex> lck(mtx);
    lines.append(line);
    if (lines.size() == 1) {
        irqThread = thread(bind(&DigitalIOChip::irqPoll, this));
    }
}

void DigitalIOChip::removeInterrupt(const gpiod::line &line) {
    int off = line.offset();
    unique_lock<mutex> lck(mtx);
    vector<int> ids = lines.get_values();
    for (vector<int>::iterator it = ids.begin(); it != ids.end(); ) {
        if (*it == off) {
            it = ids.erase(it);
        } else {
            ++it;
        }
    }
    lines.set_values(ids);
    if (lines.size() == 0) {
        lck.unlock();
        irqThread.join();
    }
}

void DigitalIOChip::irqPoll() {
    unique_lock<mutex> lck(mtx);
    while (lines.size() > 0) {
        line_bulk fired = lines.event_wait(milliseconds(10));
        for (line_bulk::iterator it = fired.begin(); it != fired.end(); ++it) {
            shared_ptr<DigitalIO> res = resources[it->offset()].lock();
            if (res) {
                const function<void(void)> &func = res->getISR();
                if (func) {
                    func();
                }
            }
        }
    }
}
