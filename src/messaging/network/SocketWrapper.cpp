#include <cstddef>
#include <mutex>
#include <stdexcept>
#include <string>
#include <utility>
#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <spacepi/log/LogLevel.hpp>
#include <spacepi/messaging/network/NetworkThread.hpp>
#include <spacepi/messaging/network/SocketWrapper.hpp>
#include <spacepi/util/Exception.hpp>

using namespace std;
using namespace boost;
using namespace boost::asio;
using namespace boost::asio::ip;
using namespace boost::asio::local;
using namespace spacepi::log;
using namespace spacepi::messaging::network;
using namespace spacepi::util;

template class SocketWrapper<tcp>;
template class SocketWrapper<stream_protocol>;

void DefaultSocketWrapperCallback::handleError(Exception::pointer err) {
    log(LogLevel::Error) << "SocketWrapper unhandled error: " << err;
}

pair<buffers_iterator<asio::streambuf::const_buffers_type>, bool> GenericSocketReader::operator ()(buffers_iterator<asio::streambuf::const_buffers_type> begin, buffers_iterator<asio::streambuf::const_buffers_type> end) {
    try {
        // Require at least one byte in order to determine the kind/size of packet
        if (end - begin < 1) {
            return make_pair(end, false);
        }
        // Compute the length of the packet
        int length = 0;
        int header = -1;
        int shift = 0;
        do {
            ++header;
            if (end - begin <= header) {
                return make_pair(end, false);
            }
            length |= ((begin[header] & 0x7F) << shift);
            shift += 7;
        } while ((begin[header] & 0x80) != 0);
        ++header;
        // Make sure there is enough space left
        if (header + length <= end - begin) {
            return make_pair(begin + header + length, true);
        }
    } catch (std::exception &) {
        wrapper->callback->handleError(Exception::createPointer(EXCEPTION(MessagingException("Error while determining if a packet is complete")) << errinfo_nested_exception(Exception::getPointer())));
    }
    return make_pair(end, false);
}

void GenericSocketReader::operator ()(const system::error_code &err, size_t length) {
    try {
        if (err) {
            wrapper->callback->handleError(Exception::createPointer(EXCEPTION(MessagingException("Error while reading socket")) << errinfo_nested_exception(Exception::createPointer(system::system_error(err)))));
        } else if (length != 0) {
            // Last byte of length is first byte without MSB set
            const char *payload = (const char *) wrapper->readBuffer.data().data();
            int payloadlen = length;
            while ((*payload & 0x80) != 0) {
                ++payload;
                --payloadlen;
            }
            // Move past the last length byte
            ++payload;
            --payloadlen;
            // Pass to handler
            string msg(payload, payloadlen);
            wrapper->readBuffer.consume(length);
            wrapper->callback->handlePacket(msg);
        }
        wrapper->startRead();
    } catch (const std::exception &) {
        wrapper->callback->handleError(Exception::createPointer(EXCEPTION(MessagingException("Error while executing read handler")) << errinfo_nested_exception(Exception::getPointer())));
    }
}

GenericSocketReader::GenericSocketReader(GenericSocketWrapper *wrapper) : wrapper(wrapper) {
}

void GenericSocketWriter::operator ()(const system::error_code &err, size_t length) {
    try {
        if (err) {
            wrapper->callback->handleError(Exception::createPointer(EXCEPTION(MessagingException("Error while writing socket")) << errinfo_nested_exception(Exception::createPointer(system::system_error(err)))));
        }
        std::unique_lock<mutex> lck(wrapper->writeMutex);
        if (!err) {
            wrapper->writeBuffer.consume(length);
            wrapper->writeCommitted -= length;
        }
        if (wrapper->writeUncommitted > 0) {
            wrapper->writeBuffer.commit(wrapper->writeUncommitted);
            wrapper->writeCommitted += wrapper->writeUncommitted;
            wrapper->writeUncommitted = 0;
        }
        if (wrapper->writeCommitted > 0) {
            wrapper->startWrite();
        } else {
            wrapper->isWriting = false;
        }
    } catch (const std::exception &) {
        wrapper->callback->handleError(Exception::createPointer(EXCEPTION(MessagingException("Error while executing read handler")) << errinfo_nested_exception(Exception::getPointer())));
    }
}

GenericSocketWriter::GenericSocketWriter(GenericSocketWrapper *wrapper) : wrapper(wrapper) {
}

GenericSocketWrapper::GenericSocketWrapper(SocketWrapperCallback *callback) : reader(this), writer(this), callback(callback), inited(false), writeCommitted(0), writeUncommitted(0), isWriting(false) {
    readBuffer.prepare(1024);
}

void GenericSocketWrapper::sendPacket(const string &pkt) {
    int length = pkt.length();
    int numLengthBytes = 0;
    for (int lengthToRepresent = length; lengthToRepresent > 0; lengthToRepresent >>= 7) {
        ++numLengthBytes;
    }
    std::unique_lock<mutex> lck(writeMutex);
    writeBuffer.prepare(length + numLengthBytes);
    for (int lengthToRepresent = length; lengthToRepresent > 0; lengthToRepresent >>= 7) {
        if (lengthToRepresent >= 0x80) {
            writeBuffer.sputc(0x80 | (lengthToRepresent & 0x7F));
        } else {
            writeBuffer.sputc(lengthToRepresent);
        }
    }
    writeBuffer.sputn(pkt.data(), length);
    writeUncommitted += length;
    if (!isWriting) {
        writeBuffer.commit(writeUncommitted);
        writeCommitted += writeUncommitted;
        writeUncommitted = 0;
        isWriting = true;
        startWrite();
    }
}

template <>
SocketWrapper<tcp>::SocketWrapper(SocketWrapperCallback *callback) : GenericSocketWrapper(callback), socket(new tcp::socket(NetworkThread::instance.getContext())) {
}

template <>
SocketWrapper<stream_protocol>::SocketWrapper(SocketWrapperCallback *callback) : GenericSocketWrapper(callback), socket(new stream_protocol::socket(NetworkThread::instance.getContext())) {
}

template <typename Proto>
basic_stream_socket<Proto> &SocketWrapper<Proto>::getSocket() {
    return *socket;
}

template <typename Proto>
void SocketWrapper<Proto>::startRead() {
    async_read_until(*socket, readBuffer, reader, reader);
}

template <typename Proto>
void SocketWrapper<Proto>::startWrite() {
    async_write(*socket, writeBuffer, writer);
}
