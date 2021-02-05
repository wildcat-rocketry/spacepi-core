#include <string>
#include <vector>
#include <spacepi/package/User.hpp>

using namespace std;
using namespace spacepi::package;

const string &User::getName() const noexcept {
    return name;
}

void User::setName(const string &name) noexcept {
    this->name = name;
}

const string &User::getUsername() const noexcept {
    return username;

}

void User::setUsername(const string &username) noexcept {
    this->username = username;
}

const string &User::getEmail() const noexcept {
    return email;
}

void User::setEmail(const string &email) noexcept {
    this->email = email;
}

const vector<string> &User::getKeys() const noexcept {
    return keys;
}

vector<string> &User::getKeys() noexcept {
    return keys;
}
