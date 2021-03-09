#include <bitset>
#include <functional>
#include <memory>
#include <utility>
#include <gpiod.hpp>
#include <SpacePi.hpp>
#include <spacepi/target/extension/Config.hpp>
#include <spacepi/target/extension/DigitalIO.hpp>
#include <spacepi/target/extension/DigitalIOChip.hpp>
#include <spacepi/target/extension/DigitalIOFactory.hpp>

using namespace std;
using namespace gpiod;
using namespace spacepi;
using namespace spacepi::util;
using namespace spacepi::target;
using namespace spacepi::target::extension;

extension::DigitalIO::DigitalIO(shared_ptr<DigitalIOChip> &&chip, gpiod::line &&line) : chip(chip), line(line), edge(resource::DigitalIO::Disable), isr(nullptr) {
    line_request req;
    req.consumer = chip->getFactory().getConsumerName();
    switch (line.direction()) {
        case gpiod::line::DIRECTION_OUTPUT:
            mode = Output;
            req.request_type = line_request::DIRECTION_OUTPUT;
            value = line.get_value();
            if (line.is_open_drain()) {
                mode |= OpenDrain;
                req.flags |= line_request::FLAG_OPEN_DRAIN;
            } else if (line.is_open_source()) {
                mode |= OpenSource;
                req.flags |= line_request::FLAG_OPEN_SOURCE;
            }
            break;
        case gpiod::line::DIRECTION_INPUT:
            mode = Input;
            req.request_type = line_request::DIRECTION_INPUT;
#ifdef HAVE_LIBGPIOD_BIAS
            switch (line.bias()) {
                case gpiod::line::BIAS_PULL_UP:
                    mode |= PullUp;
                    req.flags |= line_request::FLAG_BIAS_PULL_UP;
                    break;
                case gpiod::line::BIAS_PULL_DOWN:
                    mode |= PullDown;
                    req.flags |= line_request::FLAG_BIAS_PULL_DOWN;
                    break;
                default:
                    req.flags |= line_request::FLAG_BIAS_DISABLE;
                    break;
            }
#endif
            break;
        default:
            mode = Input;
            req.request_type = line_request::DIRECTION_INPUT;
#ifdef HAVE_LIBGPIOD_BIAS
            req.flags |= line_request::FLAG_BIAS_DISABLE;
#endif
            break;
    }
    line.request(req, value);
}

extension::DigitalIO::~DigitalIO() {
    if (edge != Disable) {
        chip->removeInterrupt(line);
    }
    chip->removeLine(line);
}

extension::DigitalIO::operator bool() {
    if (mode & Input) {
        return line.get_value() != 0;
    } else {
        return value != 0;
    }
}

resource::DigitalIO &extension::DigitalIO::operator =(bool value) {
    if (mode & Output){
	if (value) {
	    this->value = 1;
	} else {
	    this->value = 0;
	}
	line.set_value(this->value);
    }

    return *this;
}

enum resource::DigitalIO::Mode extension::DigitalIO::getMode() const noexcept {
    return mode;
}

void extension::DigitalIO::setMode(enum resource::DigitalIO::Mode mode) {
    if (mode != this->mode) {
        line_request req;
        req.consumer = chip->getFactory().getConsumerName();
        switch (mode & (Input | Output)) {
            case Output:
                req.request_type = line_request::DIRECTION_OUTPUT;
                switch (mode & (OpenDrain | OpenSource)) {
                    case 0:
                        break;
                    case OpenDrain:
                        req.flags |= line_request::FLAG_OPEN_DRAIN;
                        break;
                    case OpenSource:
                        req.flags |= line_request::FLAG_OPEN_SOURCE;
                        break;
                    default:
                        throw EXCEPTION(ResourceException("Unknown mode."));
                }
                break;
            case Input:
                req.request_type = line_request::DIRECTION_INPUT;
#ifdef HAVE_LIBGPIOD_BIAS
                switch (mode & (PullUp | PullDown)) {
                    case 0:
                        break;
                    case PullUp:
                        req.flags |= line_request::FLAG_BIAS_PULL_UP;
                        break;
                    case PullDown:
                        req.flags |= line_request::FLAG_BIAS_PULL_DOWN;
                        break;
                    default:
                        throw EXCEPTION(ResourceException("Unknown mode."));
                }
#endif
                break;
            default:
                throw EXCEPTION(ResourceException("Unknown mode."));
        }
        line.request(req, value);
    }
}

const function<void(void)> &extension::DigitalIO::getISR() const noexcept {
    return *isr;
}

enum resource::DigitalIO::Edge extension::DigitalIO::getISREdge() const noexcept {
    return edge;
}

void extension::DigitalIO::setISR(const function<void(void)> &isr, enum resource::DigitalIO::Edge edge) {
    enum Edge oldEdge = this->edge;
    this->isr = &isr;
    this->edge = edge;
    line_request req;
    req.consumer = chip->getFactory().getConsumerName();
    if ((mode & (Input | Output)) == Output) {
        setMode(Input);
    }
    switch (edge) {
        case Disable:
            if (oldEdge == Disable) {
                return;
            }
            req.request_type = line_request::DIRECTION_INPUT;
            chip->removeInterrupt(line);
            break;
        case Rising:
            req.request_type = line_request::EVENT_RISING_EDGE;
            break;
        case Falling:
            req.request_type = line_request::EVENT_FALLING_EDGE;
            break;
        case Transition:
            req.request_type = line_request::EVENT_BOTH_EDGES;
            break;
        default:
            throw EXCEPTION(ResourceException("Unknown edge."));
    }
    line.request(req);
    if (oldEdge == Disable) {
        chip->addInterrupt(line);
    }
}
