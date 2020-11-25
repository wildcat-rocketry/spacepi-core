#ifndef SPACEPI_CORE_UTIL_TRIE_HPP
#define SPACEPI_CORE_UTIL_TRIE_HPP

#include <exception>
#include <memory>
#include <string>
#include <utility>
#include <spacepi/util/Exception.hpp>

namespace spacepi {
    namespace util {
        template <typename Type, typename KeyElement = char, KeyElement Min = ' ', KeyElement Max = '~'>
        class Trie {
            public:
                class ConstFindResult {
                    public:
                        ConstFindResult() : constData(nullptr), constChild(nullptr) {
                        }

                        ConstFindResult(const Type &data) : constData(&data), constChild(nullptr) {
                        }

                        ConstFindResult(const Trie<Type, KeyElement, Min, Max> &child) : constData(nullptr), constChild(&child) {
                        }

                        const Type &data() const {
                            if (!constData) {
                                throw EXCEPTION(StateException("Find result does not contain data"));
                            }
                            return *constData;
                        }

                        bool hasData() const noexcept {
                            return constData;
                        }

                        const Trie<Type, KeyElement, Min, Max> &child() const {
                            if (!constChild) {
                                throw EXCEPTION(StateException("Find result does not contain a child"));
                            }
                            return *constChild;
                        }

                        bool hasChild() const noexcept {
                            return constChild;
                        }

                    private:
                        const Type *constData;
                        const Trie<Type, KeyElement, Min, Max> *constChild;
                };

                class FindResult : public ConstFindResult {
                    public:
                        FindResult() : mutableData(nullptr), mutableChild(nullptr) {
                        }

                        FindResult(Type &data) : ConstFindResult(data), mutableData(&data), mutableChild(nullptr) {
                        }

                        FindResult(Trie<Type, KeyElement, Min, Max> &child) : ConstFindResult(child), mutableData(nullptr), mutableChild(&child) {
                        }

                        Type &data() {
                            if (!mutableData) {
                                throw EXCEPTION(StateException("Find result does not contain data"));
                            }
                            return *mutableData;
                        }

                        Trie<Type, KeyElement, Min, Max> &child() {
                            if (!mutableChild) {
                                throw EXCEPTION(StateException("Find result does not contain a child"));
                            }
                            return *mutableChild;
                        }

                    private:
                        Type *mutableData;
                        Trie<Type, KeyElement, Min, Max> *mutableChild;
                };

                Trie() noexcept = default;

                Trie(const Trie<Type, KeyElement, Min, Max> &copy) {
                    for (int i = 0; i <= Max - Min; ++i) {
                        if (copy.data[i]) {
                            data[i].reset(new Type(copy.data[i]));
                        } else if (copy.children[i]) {
                            children[i].reset(new Trie<Type, KeyElement, Min, Max>(copy.children[i]));
                        }
                    }
                }

                Trie(Trie<Type, KeyElement, Min, Max> &&) = default;

                Trie &operator =(const Trie<Type, KeyElement, Min, Max> &copy) {
                    for (int i = 0; i <= Max - Min; ++i) {
                        if (copy.data[i]) {
                            data[i].reset(new Type(copy.data[i]));
                            children[i].reset();
                        } else if (copy.children[i]) {
                            data[i].reset();
                            children[i].reset(new Trie<Type, KeyElement, Min, Max>(copy.children[i]));
                        } else {
                            data[i].reset();
                            children[i].reset();
                        }
                    }
                }

                Trie &operator =(Trie<Type, KeyElement, Min, Max> &&) = default;

                bool empty() const noexcept {
                    for (int i = 0; i <= Max - Min; ++i) {
                        if (data[i] || (children[i] && !children[i]->empty())) {
                            return false;
                        }
                    }
                    return true;
                }

                int size() const noexcept {
                    int res = 0;
                    for (int i = 0; i <= Max - Min; ++i) {
                        if (data[i]) {
                            ++res;
                        } else if (children[i]) {
                            res += children[i]->size();
                        }
                    }
                    return res;
                }

                Type &dataAt(KeyElement key) {
                    throwChild(key);
                    std::unique_ptr<Type> &d = data[key - Min];
                    if (!d) {
                        d.reset(new Type());
                    }
                    return *d;
                }

                const Type &dataAt(KeyElement key) const {
                    throwChild(key);
                    return *data[key - Min];
                }

                Type &dataAt(const KeyElement *keys) {
                    return dataAt(keys, countKeys(keys));
                }

                const Type &dataAt(const KeyElement *keys) const {
                    return dataAt(keys, countKeys(keys));
                }

                Type &dataAt(const KeyElement *keys, int keyCount) {
                    Trie<Type, KeyElement, Min, Max> *trie = this;
                    for (int i = 0; i < keyCount - 1; ++i) {
                        if (trie->hasDataAt(keys[i])) {
                            return trie->dataAt(keys[i]);
                        }
                        trie = &trie->childAt(keys[i]);
                    }
                    return trie->dataAt(keys[keyCount - 1]);
                }

                const Type &dataAt(const KeyElement *keys, int keyCount) const {
                    const Trie<Type, KeyElement, Min, Max> *trie = this;
                    for (int i = 0; i < keyCount - 1; ++i) {
                        if (trie->hasDataAt(keys[i])) {
                            return trie->dataAt(keys[i]);
                        }
                        trie = &trie->childAt(keys[i]);
                    }
                    return trie->dataAt(keys[keyCount - 1]);
                }

                Type &dataAt(const std::basic_string<KeyElement> &keys) {
                    return dataAt(keys.data(), keys.size());
                }

                const Type &dataAt(const std::basic_string<KeyElement> &keys) const {
                    return dataAt(keys.data(), keys.size());
                }

                bool hasDataAt(KeyElement key) const noexcept {
                    return validateData(key);
                }

                bool hasDataAt(const KeyElement *keys) const noexcept {
                    return hasDataAt(keys, countKeys(keys));
                }

                bool hasDataAt(const KeyElement *keys, int keyCount) const noexcept {
                    try {
                        const Trie<Type, KeyElement, Min, Max> *trie = this;
                        for (int i = 0; i < keyCount - 1; ++i) {
                            if (trie->hasDataAt(keys[i])) {
                                return true;
                            }
                            trie = &trie->childAt(keys[i]);
                        }
                        return trie->hasDataAt(keys[keyCount - 1]);
                    } catch (std::exception &) {
                        return false;
                    }
                }

                bool hasDataAt(const std::basic_string<KeyElement> &keys) const noexcept {
                    return hasDataAt(keys.data(), keys.size());
                }

                Trie<Type, KeyElement, Min, Max> &childAt(KeyElement key) {
                    throwData(key);
                    std::unique_ptr<Trie<Type, KeyElement, Min, Max>> &c = children[key - Min];
                    if (!c) {
                        c.reset(new Trie<Type, KeyElement, Min, Max>());
                    }
                    return *c;
                }

                const Trie<Type, KeyElement, Min, Max> &childAt(KeyElement key) const {
                    throwData(key);
                    return *children[key - Min];
                }

                Trie<Type, KeyElement, Min, Max> &childAt(const KeyElement *keys) {
                    return childAt(keys, countKeys(keys));
                }

                const Trie<Type, KeyElement, Min, Max> &childAt(const KeyElement *keys) const {
                    return childAt(keys, countKeys(keys));
                }

                Trie<Type, KeyElement, Min, Max> &childAt(const KeyElement *keys, int keyCount) {
                    Trie<Type, KeyElement, Min, Max> *trie = this;
                    for (int i = 0; i < keyCount; ++i) {
                        trie = &trie->childAt(keys[i]);
                    }
                    return *trie;
                }

                const Trie<Type, KeyElement, Min, Max> &childAt(const KeyElement *keys, int keyCount) const {
                    const Trie<Type, KeyElement, Min, Max> *trie = this;
                    for (int i = 0; i < keyCount; ++i) {
                        trie = &trie->childAt(keys[i]);
                    }
                    return *trie;
                }


                Trie<Type, KeyElement, Min, Max> &childAt(const std::basic_string<KeyElement> &keys) {
                    return childAt(keys.data(), keys.size());
                }

                const Trie<Type, KeyElement, Min, Max> &childAt(const std::basic_string<KeyElement> &keys) const {
                    return childAt(keys.data(), keys.size());
                }


                bool hasChildAt(KeyElement key) const noexcept {
                    return validateChild(key);
                }

                bool hasChildAt(const KeyElement *keys) const noexcept {
                    return hasChildAt(keys, countKeys(keys));
                }

                bool hasChildAt(const KeyElement *keys, int keyCount) const noexcept {
                    try {
                        const Trie<Type, KeyElement, Min, Max> *trie = this;
                        for (int i = 0; i < keyCount; ++i) {
                            if (!trie->hasChildAt(keys[i])) {
                                return false;
                            }
                            trie = &trie->childAt(keys[i]);
                        }
                        return true;
                    } catch (std::exception &) {
                        return false;
                    }
                }

                bool hasChildAt(const std::basic_string<KeyElement> &keys) const noexcept {
                    return hasChildAt(keys.data(), keys.size());
                }

                Type &insert(KeyElement key, const Type &val) {
                    return emplace(key, val);
                }

                Type &insert(KeyElement key, Type &&val) {
                    return emplace(key, std::move(val));
                }

                Type &insert(const KeyElement *keys, const Type &val) {
                    return emplace<const Type &>(keys, countKeys(keys), val);
                }

                Type &insert(const KeyElement *keys, Type &&val) {
                    return emplace<Type &&>(keys, countKeys(keys), std::move(val));
                }

                Type &insert(const KeyElement *keys, int keyCount, const Type &val) {
                    return emplace<const Type &>(keys, keyCount, val);
                }

                Type &insert(const KeyElement *keys, int keyCount, Type &&val) {
                    return emplace<Type &&>(keys, keyCount, std::move(val));
                }

                Type &insert(const std::basic_string<KeyElement> &keys, const Type &val) {
                    return emplace<const Type &>(keys.data(), keys.size(), val);
                }

                Type &insert(const std::basic_string<KeyElement> &keys, Type &&val) {
                    return emplace<Type &&>(keys.data(), keys.size(), std::move(val));
                }

                void erase(KeyElement key) {
                    throwInvalidRange(key);
                    if (hasDataAt(key)) {
                        data[key - Min].reset();
                    } else if (hasChildAt(key)) {
                        children[key - Min].reset();
                    }
                }

                void erase(const KeyElement *keys) {
                    erase(keys, countKeys(keys));
                }

                void erase(const KeyElement *keys, int keyCount) {
                    Trie<Type, KeyElement, Min, Max> *trie = this;
                    for (int i = 0; i < keyCount - 1; ++i) {
                        if (trie->hasDataAt(keys[i])) {
                            trie->erase(keys[i]);
                            return;
                        }
                        trie = &trie->childAt(keys[i]);
                    }
                    trie->erase(keys[keyCount - 1]);
                }

                void erase(const std::basic_string<KeyElement> &keys) {
                    erase(keys.data(), keys.size());
                }

                template <typename... Args>
                Type &emplace(KeyElement key, Args&&... args) {
                    throwChild(key);
                    data[key - Min].reset(new Type(std::forward<Args...>(args...)));
                    return *data[key - Min];
                }

                template <typename... Args>
                Type &emplace(const KeyElement *keys, Args&&... args) {
                    return emplace<Args...>(keys, countKeys(keys), std::forward<Args...>(args...));
                }

                template <typename... Args>
                Type &emplace(const KeyElement *keys, int keyCount, Args&&... args) {
                    Trie<Type, KeyElement, Min, Max> *trie = this;
                    for (int i = 0; i < keyCount - 1; ++i) {
                        trie = &trie->childAt(keys[i]);
                    }
                    return trie->emplace(keys[keyCount - 1], std::forward<Args...>(args...));
                }

                template <typename... Args>
                Type &emplace(const std::basic_string<KeyElement> &keys, Args&&... args) {
                    return emplace<Args...>(keys.data(), keys.size(), std::forward<Args...>(args...));
                }

                FindResult find(KeyElement key) {
                    if (hasDataAt(key)) {
                        return FindResult(dataAt(key));
                    } else if (hasChildAt(key)) {
                        return FindResult(childAt(key));
                    } else {
                        return FindResult();
                    }
                }

                ConstFindResult find(KeyElement key) const {
                    if (hasDataAt(key)) {
                        return ConstFindResult(dataAt(key));
                    } else if (hasChildAt(key)) {
                        return ConstFindResult(childAt(key));
                    } else {
                        return ConstFindResult();
                    }
                }

                FindResult find(const KeyElement *keys) {
                    return find(keys, countKeys(keys));
                }

                ConstFindResult find(const KeyElement *keys) const {
                    return find(keys, countKeys(keys));
                }

                FindResult find(const KeyElement *keys, int keyCount) {
                    Trie<Type, KeyElement, Min, Max> *trie = this;
                    for (int i = 0; i < keyCount; ++i) {
                        if (trie->hasDataAt(keys[i])) {
                            return FindResult(trie->dataAt(keys[i]));
                        } else if (trie->hasChildAt(keys[i])) {
                            trie = &trie->childAt(keys[i]);
                        } else {
                            return FindResult();
                        }
                    }
                    return FindResult(trie->childAt(keys[keyCount - 1]));
                }

                ConstFindResult find(const KeyElement *keys, int keyCount) const {
                    const Trie<Type, KeyElement, Min, Max> *trie = this;
                    for (int i = 0; i < keyCount; ++i) {
                        if (trie->hasDataAt(keys[i])) {
                            return ConstFindResult(trie->dataAt(keys[i]));
                        } else if (trie->hasChildAt(keys[i])) {
                            trie = &trie->childAt(keys[i]);
                        } else {
                            return ConstFindResult();
                        }
                    }
                    return ConstFindResult(trie->childAt(keys[keyCount - 1]));
                }

                FindResult find(const std::basic_string<KeyElement> &keys) {
                    return find(keys.data(), keys.size());
                }

                ConstFindResult find(const std::basic_string<KeyElement> &keys) const {
                    return find(keys.data(), keys.size());
                }

            private:
                bool validateRange(KeyElement key) const {
                    return key >= Min && key <= Max;
                }

                void throwInvalidRange(KeyElement key) const {
                    if (!validateRange(key)) {
                        throw EXCEPTION(ArgumentException("Index out of range"));
                    }
                }

                bool validateChild(KeyElement key) const {
                    if (!validateRange(key)) {
                        return false;
                    }
                    return (bool) children[key - Min];
                }

                void throwChild(KeyElement key) const {
                    throwInvalidRange(key);
                    if (validateChild(key)) {
                        throw EXCEPTION(StateException("Key represents a child"));
                    }
                }

                bool validateData(KeyElement key) const {
                    if (!validateRange(key)) {
                        return false;
                    }
                    return (bool) data[key - Min];
                }

                void throwData(KeyElement key) const {
                    throwInvalidRange(key);
                    if (validateData(key)) {
                        throw EXCEPTION(StateException("Key represents a data element"));
                    }
                }

                int countKeys(const KeyElement *keys) const noexcept {
                    int count;
                    for (count = 0; validateRange(keys[count]); ++count);
                    return count - 1;
                }

                std::unique_ptr<Trie<Type, KeyElement, Min, Max>> children[Max - Min + 1];
                std::unique_ptr<Type> data[Max - Min + 1];
        };
    }
}

#endif
