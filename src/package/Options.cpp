#include <memory>
#include <stack>
#include <string>
#include <unordered_map>
#include <spacepi/package/Options.hpp>

using namespace std;
using namespace spacepi::package;

const string &Options::Node::getKey() const noexcept {
    return key;
}

void Options::Node::setKey(const string &key) noexcept {
    this->key = key;
}

const string &Options::Node::getValue() const noexcept {
    return value;
}

void Options::Node::setValue(const string &value) noexcept {
    this->value = value;
}

Options::ConstIterator::ConstIterator(const shared_ptr<Options::Node> &node) noexcept : node(node) {
}

Options::ConstIterator &Options::ConstIterator::operator ++() noexcept {
    shared_ptr<Node> n = node.lock();
    if (n) {
        node = n->next;
    }
    return *this;
}

const Options::Node &Options::ConstIterator::operator *() const noexcept {
    return *node.lock();
}

const Options::Node *Options::ConstIterator::operator ->() const noexcept {
    return node.lock().get();
}

const shared_ptr<Options::Node> Options::ConstIterator::get() const noexcept {
    return node.lock();
}

bool Options::ConstIterator::operator ==(const Options::ConstIterator &other) const noexcept {
    return node.lock() == other.node.lock();
}

bool Options::ConstIterator::operator !=(const Options::ConstIterator &other) const noexcept {
    return node.lock() != other.node.lock();
}

Options::Iterator::Iterator(const shared_ptr<Options::Node> &node) noexcept : Options::ConstIterator(node) {
}

Options::Node &Options::Iterator::operator *() noexcept {
    return *node.lock();
}

Options::Node *Options::Iterator::operator ->() noexcept {
    return node.lock().get();
}

const string &Options::getOption(const string &key) const noexcept {
    static string empty = "";
    unordered_map<string, stack<shared_ptr<Node>>>::const_iterator it = sortedData.find(key);
    if (it == sortedData.end()) {
        return empty;
    }
    const stack<shared_ptr<Node>> &stack = it->second;
    if (stack.empty()) {
        return empty;
    }
    return stack.top()->getValue();
}

void Options::setOption(const string &key, const string &value) noexcept {
    unordered_map<string, stack<shared_ptr<Node>>>::iterator it = sortedData.find(key);
    if (it == sortedData.end()) {
        addOption(key, value);
    } else {
        it->second.top()->setValue(value);
    }
}

void Options::addOption(const string &key, const string &value, const ConstIterator &position) noexcept {
    unordered_map<string, stack<shared_ptr<Node>>>::iterator it = sortedData.emplace(key, stack<shared_ptr<Node>>()).first;
    shared_ptr<Node> node(new Node());
    node->setKey(key);
    node->setValue(value);
    it->second.push(node);
    if (position == end()) {
        node->prev = tail;
        tail = node;
        if (!head) {
            head = tail;
        }
    } else if (position == begin()) {
        node->next = head;
        head = node;
    } else {
        node->prev = position->prev;
        node->next = position.get();
    }
    if (node->prev) {
        node->prev->next = node;
    }
    if (node->next) {
        node->next->prev = node;
    }
}

void Options::removeOption(const string &key) noexcept {
    unordered_map<string, stack<shared_ptr<Node>>>::iterator it = sortedData.find(key);
    if (it != sortedData.end() && !it->second.empty()) {
        shared_ptr<Node> node = it->second.top();
        if (node->prev) {
            node->prev->next = node->next;
        } else {
            head = node->next;
        }
        if (node->next) {
            node->next->prev = node->prev;
        } else {
            tail = node->prev;
        }
        it->second.pop();
    }
}

void Options::clearOption(const std::string &key) noexcept {
    unordered_map<string, stack<shared_ptr<Node>>>::iterator it = sortedData.find(key);
    if (it != sortedData.end()) {
        while (!it->second.empty()) {
            removeOption(key);
        }
    }
}

void Options::clear() noexcept {
    head.reset();
    tail.reset();
    sortedData.clear();
}

Options::ConstIterator Options::begin() const noexcept {
    return ConstIterator(head);
}

Options::Iterator Options::begin() noexcept {
    return Iterator(head);
}

Options::ConstIterator Options::end() const noexcept {
    return ConstIterator();
}

Options::Iterator Options::end() noexcept {
    return Iterator();
}
