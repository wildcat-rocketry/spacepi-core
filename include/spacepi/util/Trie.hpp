#ifndef SPACEPI_CORE_UTIL_TRIE_HPP
#define SPACEPI_CORE_UTIL_TRIE_HPP

#include <exception>
#include <memory>
#include <string>
#include <utility>
#include <spacepi/util/Exception.hpp>

namespace spacepi {
    namespace util {
        /**
         * \brief Data type representing a trie
         * 
         * A trie is good for quickly accessing and modifying data which contains a string-valued key
         * 
         * \tparam Type The type of value stored for each string-valued key
         * \tparam KeyElement The type of character which makes up the string-valued key
         * \tparam Min The minimum \c KeyElement which can be stored in the Trie
         * \tparam Max The maximum \c KeyElement which can be stored in the Trie
         */
        template <typename Type, typename KeyElement = char, KeyElement Min = ' ', KeyElement Max = '~'>
        class Trie {
            public:
                /**
                 * \brief The return value of find()
                 * 
                 * This may either point to a data node (if the key matches exactly), a child node (if the key matches a
                 * prefix), or nothing if the key was not found.
                 */
                class ConstFindResult {
                    public:
                        /**
                         * \brief Initialize an empty ConstFindResult
                         */
                        ConstFindResult() : constData(nullptr), constChild(nullptr) {
                        }

                        /**
                         * \brief Initialize a ConstFindResult which points to a data node
                         * 
                         * \param[in] data The data node
                         */
                        ConstFindResult(const Type &data) : constData(&data), constChild(nullptr) {
                        }

                        /**
                         * \brief Initialize a ConstFindResult which points to a child node
                         * 
                         * \param[in] child The child node
                         */
                        ConstFindResult(const Trie<Type, KeyElement, Min, Max> &child) : constData(nullptr), constChild(&child) {
                        }

                        /**
                         * \brief Destroy the ConstFindResult
                         */
                        virtual ~ConstFindResult() = default;

                        /**
                         * \brief Get the value from the data node this result points to
                         * 
                         * Do not call this method if hasData() returns \c false
                         * 
                         * \return The value object
                         */
                        const Type &data() const {
                            if (!constData) {
                                throw EXCEPTION(StateException("Find result does not contain data"));
                            }
                            return *constData;
                        }

                        /**
                         * \brief Determine if this result points to a data node
                         * 
                         * \return If it does
                         */
                        bool hasData() const noexcept {
                            return constData;
                        }

                        /**
                         * \brief Get the child node this result points to
                         * 
                         * Do not call this method if hasChild() returns \c false
                         * 
                         * \return The child node
                         */
                        const Trie<Type, KeyElement, Min, Max> &child() const {
                            if (!constChild) {
                                throw EXCEPTION(StateException("Find result does not contain a child"));
                            }
                            return *constChild;
                        }

                        /**
                         * \brief Determine if this result points to a child node
                         * 
                         * \return If it does
                         */
                        bool hasChild() const noexcept {
                            return constChild;
                        }

                    private:
                        const Type *constData;
                        const Trie<Type, KeyElement, Min, Max> *constChild;
                };

                /**
                 * \brief The return value of find()
                 * 
                 * This may either point to a data node (if the key matches exactly), a child node (if the key matches a
                 * prefix), or nothing if the key was not found.
                 */
                class FindResult final : public ConstFindResult {
                    public:
                        /**
                         * \brief Initialize an empty ConstFindResult
                         */
                        FindResult() : mutableData(nullptr), mutableChild(nullptr) {
                        }

                        /**
                         * \brief Initialize a ConstFindResult which points to a data node
                         * 
                         * \param[in,out] data The data node
                         */
                        FindResult(Type &data) : ConstFindResult(data), mutableData(&data), mutableChild(nullptr) {
                        }

                        /**
                         * \brief Initialize a ConstFindResult which points to a child node
                         * 
                         * \param[in,out] child The child node
                         */
                        FindResult(Trie<Type, KeyElement, Min, Max> &child) : ConstFindResult(child), mutableData(nullptr), mutableChild(&child) {
                        }

                        /**
                         * \copydoc ConstFindResult::data()
                         */
                        Type &data() {
                            if (!mutableData) {
                                throw EXCEPTION(StateException("Find result does not contain data"));
                            }
                            return *mutableData;
                        }

                        /**
                         * \copydoc ConstFindResult::child()
                         */
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

                /**
                 * \brief Initialize an empty Trie
                 */
                Trie() noexcept = default;

                /**
                 * \brief Copy-construct a Trie
                 * 
                 * \param[in] copy The Trie to copy from
                 */
                Trie(const Trie<Type, KeyElement, Min, Max> &copy) {
                    for (int i = 0; i <= Max - Min; ++i) {
                        if (copy.data[i]) {
                            data[i].reset(new Type(copy.data[i]));
                        } else if (copy.children[i]) {
                            children[i].reset(new Trie<Type, KeyElement, Min, Max>(copy.children[i]));
                        }
                    }
                }

                /**
                 * \brief Move-construct a Trie
                 * 
                 * \param[in,out] move The Trie to move from
                 */
                Trie(Trie<Type, KeyElement, Min, Max> &&move) = default;

                /**
                 * \brief Destroy a Trie
                 */
                virtual ~Trie() = default;

                /**
                 * \brief Copy-assign a Trie
                 * 
                 * \param[in] copy The Trie to copy from
                 * 
                 * \return \c this
                 */
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

                /**
                 * \brief Move-assign a Trie
                 * 
                 * \param[in] move The Trie to move from
                 * 
                 * \return \c this
                 */
                Trie &operator =(Trie<Type, KeyElement, Min, Max> &&move) = default;

                /**
                 * \brief Determine if this Trie is empty
                 * 
                 * \return If it is empty
                 */
                bool empty() const noexcept {
                    for (int i = 0; i <= Max - Min; ++i) {
                        if (data[i] || (children[i] && !children[i]->empty())) {
                            return false;
                        }
                    }
                    return true;
                }

                /**
                 * \brief Determine the total number of data nodes contained within this Trie and all children
                 * 
                 * \return The total number of data nodes
                 */
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

                /**
                 * \brief Get the data node inside this Trie, and do not check child Trie's
                 * 
                 * \param[in] key The key for which to get the data node
                 * 
                 * \return The data node
                 */
                Type &dataAt(KeyElement key) {
                    throwChild(key);
                    std::unique_ptr<Type> &d = data[key - Min];
                    if (!d) {
                        d.reset(new Type());
                    }
                    return *d;
                }

                /**
                 * \copydoc dataAt(KeyElement)
                 */
                const Type &dataAt(KeyElement key) const {
                    throwChild(key);
                    return *data[key - Min];
                }

                /**
                 * \brief Get the data node inside this Trie or a child Trie
                 * 
                 * \param[in] keys A set of keys for which to traverse the Trie's, delimited by an out-of-range
                 * KeyElement
                 * 
                 * \return The data node
                 */
                Type &dataAt(const KeyElement *keys) {
                    return dataAt(keys, countKeys(keys));
                }

                /**
                 * \brief Get the data node inside this Trie or a child Trie
                 * 
                 * \param[in] keys A set of keys for which to traverse the Trie's, delimited by an out-of-range
                 * KeyElement
                 * 
                 * \return The data node
                 */
                const Type &dataAt(const KeyElement *keys) const {
                    return dataAt(keys, countKeys(keys));
                }

                /**
                 * \brief Get the data node inside this Trie or a child Trie
                 * 
                 * \param[in] keys A set of keys for which to traverse the Trie's
                 * \param[in] keyCount The number of keys in the \c keys array
                 * 
                 * \return The data node
                 */
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

                /**
                 * \brief Get the data node inside this Trie or a child Trie
                 * 
                 * \param[in] keys A set of keys for which to traverse the Trie's
                 * \param[in] keyCount The number of keys in the \c keys array
                 * 
                 * \return The data node
                 */
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

                /**
                 * \brief Get the data node inside this Trie or a child Trie
                 * 
                 * \param[in] keys The set of keys for which to traverse the Trie's
                 * 
                 * \return The data node
                 */
                Type &dataAt(const std::basic_string<KeyElement> &keys) {
                    return dataAt(keys.data(), keys.size());
                }

                /**
                 * \brief Get the data node inside this Trie or a child Trie
                 * 
                 * \param[in] keys The set of keys for which to traverse the Trie's
                 * 
                 * \return The data node
                 */
                const Type &dataAt(const std::basic_string<KeyElement> &keys) const {
                    return dataAt(keys.data(), keys.size());
                }

                /**
                 * \brief Determine if a data node exists inside this Trie, not checking child Trie's
                 * 
                 * \param[in] key The key for which to check for the data node
                 * 
                 * \return If the data node exists
                 */
                bool hasDataAt(KeyElement key) const noexcept {
                    return validateData(key);
                }

                /**
                 * \brief Determine if a data node exists inside this Trie or a child Trie
                 * 
                 * \param[in] keys A set of keys for which to traverse the Trie's, delimited by an out-of-range
                 * KeyElement
                 * 
                 * \return If the data node exists
                 */
                bool hasDataAt(const KeyElement *keys) const noexcept {
                    return hasDataAt(keys, countKeys(keys));
                }

                /**
                 * \brief Determine if a data node exists inside this Trie or a child Trie
                 * 
                 * \param[in] keys A set of keys for which to traverse the Trie's
                 * \param[in] keyCount The number of keys in the \c keys array
                 * 
                 * \return If the data node exists
                 */
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

                /**
                 * \brief Determine if a data node exists inside this Trie or a child Trie
                 * 
                 * \param[in] keys The set of keys for which to traverse the Trie's
                 * 
                 * \return If the data node exists
                 */
                bool hasDataAt(const std::basic_string<KeyElement> &keys) const noexcept {
                    return hasDataAt(keys.data(), keys.size());
                }

                /**
                 * \brief Get the child node inside this Trie, and do not check child Trie's
                 * 
                 * \param[in] key The key for which to get the child node
                 * 
                 * \return The child node
                 */
                Trie<Type, KeyElement, Min, Max> &childAt(KeyElement key) {
                    throwData(key);
                    std::unique_ptr<Trie<Type, KeyElement, Min, Max>> &c = children[key - Min];
                    if (!c) {
                        c.reset(new Trie<Type, KeyElement, Min, Max>());
                    }
                    return *c;
                }

                /**
                 * \copydoc childAt(KeyElement)
                 */
                const Trie<Type, KeyElement, Min, Max> &childAt(KeyElement key) const {
                    throwData(key);
                    return *children[key - Min];
                }

                /**
                 * \brief Get the child node inside this Trie or a child Trie
                 * 
                 * \param[in] keys A set of keys for which to traverse the Trie's, delimited by an out-of-range
                 * KeyElement
                 * 
                 * \return The child node
                 */
                Trie<Type, KeyElement, Min, Max> &childAt(const KeyElement *keys) {
                    return childAt(keys, countKeys(keys));
                }

                /**
                 * \brief Get the child node inside this Trie or a child Trie
                 * 
                 * \param[in] keys A set of keys for which to traverse the Trie's, delimited by an out-of-range
                 * KeyElement
                 * 
                 * \return The child node
                 */
                const Trie<Type, KeyElement, Min, Max> &childAt(const KeyElement *keys) const {
                    return childAt(keys, countKeys(keys));
                }

                /**
                 * \brief Get the child node inside this Trie or a child Trie
                 * 
                 * \param[in] keys A set of keys for which to traverse the Trie's
                 * \param[in] keyCount The number of keys in the \c keys array
                 * 
                 * \return The child node
                 */
                Trie<Type, KeyElement, Min, Max> &childAt(const KeyElement *keys, int keyCount) {
                    Trie<Type, KeyElement, Min, Max> *trie = this;
                    for (int i = 0; i < keyCount; ++i) {
                        trie = &trie->childAt(keys[i]);
                    }
                    return *trie;
                }

                /**
                 * \brief Get the child node inside this Trie or a child Trie
                 * 
                 * \param[in] keys A set of keys for which to traverse the Trie's
                 * \param[in] keyCount The number of keys in the \c keys array
                 * 
                 * \return The child node
                 */
                const Trie<Type, KeyElement, Min, Max> &childAt(const KeyElement *keys, int keyCount) const {
                    const Trie<Type, KeyElement, Min, Max> *trie = this;
                    for (int i = 0; i < keyCount; ++i) {
                        trie = &trie->childAt(keys[i]);
                    }
                    return *trie;
                }

                /**
                 * \brief Get the child node inside this Trie or a child Trie
                 * 
                 * \param[in] keys The set of keys for which to traverse the Trie's
                 * 
                 * \return The child node
                 */
                Trie<Type, KeyElement, Min, Max> &childAt(const std::basic_string<KeyElement> &keys) {
                    return childAt(keys.data(), keys.size());
                }

                /**
                 * \brief Get the child node inside this Trie or a child Trie
                 * 
                 * \param[in] keys The set of keys for which to traverse the Trie's
                 * 
                 * \return The child node
                 */
                const Trie<Type, KeyElement, Min, Max> &childAt(const std::basic_string<KeyElement> &keys) const {
                    return childAt(keys.data(), keys.size());
                }

                /**
                 * \brief Determine if a child node exists inside this Trie, not checking child Trie's
                 * 
                 * \param[in] key The key for which to check for the child node
                 * 
                 * \return If the child node exists
                 */
                bool hasChildAt(KeyElement key) const noexcept {
                    return validateChild(key);
                }

                /**
                 * \brief Determine if a child node exists inside this Trie or a child Trie
                 * 
                 * \param[in] keys A set of keys for which to traverse the Trie's, delimited by an out-of-range
                 * KeyElement
                 * 
                 * \return If the child node exists
                 */
                bool hasChildAt(const KeyElement *keys) const noexcept {
                    return hasChildAt(keys, countKeys(keys));
                }

                /**
                 * \brief Determine if a child node exists inside this Trie or a child Trie
                 * 
                 * \param[in] keys A set of keys for which to traverse the Trie's
                 * \param[in] keyCount The number of keys in the \c keys array
                 * 
                 * \return If the child node exists
                 */
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

                /**
                 * \brief Determine if a child node exists inside this Trie or a child Trie
                 * 
                 * \param[in] keys The set of keys for which to traverse the Trie's
                 * 
                 * \return If the child node exists
                 */
                bool hasChildAt(const std::basic_string<KeyElement> &keys) const noexcept {
                    return hasChildAt(keys.data(), keys.size());
                }

                /**
                 * \brief Insert a data node into this Trie
                 * 
                 * \param[in] key The key for which to insert the element
                 * \param[in] val The value to copy into the element
                 * 
                 * \return The value now stored in the Trie
                 */
                Type &insert(KeyElement key, const Type &val) {
                    return emplace(key, val);
                }

                /**
                 * \brief Insert a data node into this Trie
                 * 
                 * \param[in] key The key for which to insert the element
                 * \param[in] val The value to move into the element
                 * 
                 * \return The value now stored in the Trie
                 */
                Type &insert(KeyElement key, Type &&val) {
                    return emplace(key, std::move(val));
                }

                /**
                 * \brief Insert a data node into this Trie or a child Trie
                 * 
                 * \param[in] keys A set of keys for which to insert the element, delimited by an out-of-range
                 * KeyElement
                 * \param[in] val The value to copy into the element
                 * 
                 * \return The value now stored in the Trie
                 */
                Type &insert(const KeyElement *keys, const Type &val) {
                    return emplace<const Type &>(keys, countKeys(keys), val);
                }

                /**
                 * \brief Insert a data node into this Trie or a child Trie
                 * 
                 * \param[in] keys A set of keys for which to insert the element, delimited by an out-of-range
                 * KeyElement
                 * \param[in] val The value to move into the element
                 * 
                 * \return The value now stored in the Trie
                 */
                Type &insert(const KeyElement *keys, Type &&val) {
                    return emplace<Type &&>(keys, countKeys(keys), std::move(val));
                }

                /**
                 * \brief Insert a data node into this Trie or a child Trie
                 * 
                 * \param[in] keys A set of keys for which to insert the element
                 * \param[in] keyCount The number of keys in the \c keys array
                 * \param[in] val The value to copy into the element
                 * 
                 * \return The value now stored in the Trie
                 */
                Type &insert(const KeyElement *keys, int keyCount, const Type &val) {
                    return emplace<const Type &>(keys, keyCount, val);
                }

                /**
                 * \brief Insert a data node into this Trie or a child Trie
                 * 
                 * \param[in] keys A set of keys for which to insert the element
                 * \param[in] keyCount The number of keys in the \c keys array
                 * \param[in] val The value to move into the element
                 * 
                 * \return The value now stored in the Trie
                 */
                Type &insert(const KeyElement *keys, int keyCount, Type &&val) {
                    return emplace<Type &&>(keys, keyCount, std::move(val));
                }

                /**
                 * \brief Insert a data node into this Trie or a child Trie
                 * 
                 * \param[in] keys A set of keys for which to insert the element
                 * \param[in] val The value to copy into the element
                 * 
                 * \return The value now stored in the Trie
                 */
                Type &insert(const std::basic_string<KeyElement> &keys, const Type &val) {
                    return emplace<const Type &>(keys.data(), keys.size(), val);
                }

                /**
                 * \brief Insert a data node into this Trie or a child Trie
                 * 
                 * \param[in] keys A set of keys for which to insert the element
                 * \param[in] val The value to move into the element
                 * 
                 * \return The value now stored in the Trie
                 */
                Type &insert(const std::basic_string<KeyElement> &keys, Type &&val) {
                    return emplace<Type &&>(keys.data(), keys.size(), std::move(val));
                }

                /**
                 * \brief Remove a data node from this Trie
                 * 
                 * \param[in] key The key for which to remove the element
                 */
                void erase(KeyElement key) {
                    throwInvalidRange(key);
                    if (hasDataAt(key)) {
                        data[key - Min].reset();
                    } else if (hasChildAt(key)) {
                        children[key - Min].reset();
                    }
                }

                /**
                 * \brief Remove a data node from this Trie or a child Trie
                 * 
                 * \param[in] keys A set of keys for which to remove the element, delimited by an out-of-range
                 * KeyElement
                 */
                void erase(const KeyElement *keys) {
                    erase(keys, countKeys(keys));
                }

                /**
                 * \brief Remove a data node from this Trie or a child Trie
                 * 
                 * \param[in] keys A set of keys for which to remove the element
                 * \param[in] keyCount The number of keys in the \c keys array
                 */
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

                /**
                 * \brief Remove a data node from this Trie or a child Trie
                 * 
                 * \param[in] keys A set of keys for which to remove the element
                 */
                void erase(const std::basic_string<KeyElement> &keys) {
                    erase(keys.data(), keys.size());
                }

                /**
                 * \brief Construct a new data node inside this Trie
                 * 
                 * \param[in] key The key for which to insert the element
                 * \param[in,out] args The arguments to the constructor
                 * 
                 * \return The value now stored in the Trie
                 */
                template <typename... Args>
                Type &emplace(KeyElement key, Args&&... args) {
                    throwChild(key);
                    data[key - Min].reset(new Type(std::forward<Args...>(args...)));
                    return *data[key - Min];
                }

                /**
                 * \brief Construct a new data node inside this Trie or a child Trie
                 * 
                 * \param[in] keys A set of keys for which to insert the element, delimited by an out-of-range
                 * KeyElement
                 * \param[in,out] args The arguments to the constructor
                 * 
                 * \return The value now stored in the Trie
                 */
                template <typename... Args>
                Type &emplace(const KeyElement *keys, Args&&... args) {
                    return emplace<Args...>(keys, countKeys(keys), std::forward<Args...>(args...));
                }

                /**
                 * \brief Construct a new data node inside this Trie or a child Trie
                 * 
                 * \param[in] keys A set of keys for which to insert the element
                 * \param[in] keyCount The number of keys in the \c keys array
                 * \param[in,out] args The arguments to the constructor
                 * 
                 * \return The value now stored in the Trie
                 */
                template <typename... Args>
                Type &emplace(const KeyElement *keys, int keyCount, Args&&... args) {
                    Trie<Type, KeyElement, Min, Max> *trie = this;
                    for (int i = 0; i < keyCount - 1; ++i) {
                        trie = &trie->childAt(keys[i]);
                    }
                    return trie->emplace(keys[keyCount - 1], std::forward<Args...>(args...));
                }

                /**
                 * \brief Construct a new data node inside this Trie or a child Trie
                 * 
                 * \param[in] keys A set of keys for which to insert the element
                 * \param[in,out] args The arguments to the constructor
                 * 
                 * \return The value now stored in the Trie
                 */
                template <typename... Args>
                Type &emplace(const std::basic_string<KeyElement> &keys, Args&&... args) {
                    return emplace<Args...>(keys.data(), keys.size(), std::forward<Args...>(args...));
                }

                /**
                 * \brief Search this Trie for a key
                 * 
                 * \param[in] key The key for which to search
                 * 
                 * \return The results of the search
                 */
                FindResult find(KeyElement key) {
                    if (hasDataAt(key)) {
                        return FindResult(dataAt(key));
                    } else if (hasChildAt(key)) {
                        return FindResult(childAt(key));
                    } else {
                        return FindResult();
                    }
                }

                /**
                 * \copydoc find(KeyElement)
                 */
                ConstFindResult find(KeyElement key) const {
                    if (hasDataAt(key)) {
                        return ConstFindResult(dataAt(key));
                    } else if (hasChildAt(key)) {
                        return ConstFindResult(childAt(key));
                    } else {
                        return ConstFindResult();
                    }
                }

                /**
                 * \brief Search this Trie or a child Trie
                 * 
                 * \param[in] keys A set of keys for which to search, delimited by an out-of-range KeyElement
                 * 
                 * \return The results of the search
                 */
                FindResult find(const KeyElement *keys) {
                    return find(keys, countKeys(keys));
                }

                /**
                 * \brief Search this Trie or a child Trie
                 * 
                 * \param[in] keys A set of keys for which to search, delimited by an out-of-range KeyElement
                 * 
                 * \return The results of the search
                 */
                ConstFindResult find(const KeyElement *keys) const {
                    return find(keys, countKeys(keys));
                }

                /**
                 * \brief Search this Trie or a child Trie
                 * 
                 * \param[in] keys A set of keys for which to search
                 * \param[in] keyCount The number of keys in the \c keys array
                 * 
                 * \return The results of the search
                 */
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

                /**
                 * \brief Search this Trie or a child Trie
                 * 
                 * \param[in] keys A set of keys for which to search
                 * \param[in] keyCount The number of keys in the \c keys array
                 * 
                 * \return The results of the search
                 */
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

                /**
                 * \brief Search this Trie or a child Trie
                 * 
                 * \param[in] keys A set of keys for which to search
                 * 
                 * \return The results of the search
                 */
                FindResult find(const std::basic_string<KeyElement> &keys) {
                    return find(keys.data(), keys.size());
                }

                /**
                 * \brief Search this Trie or a child Trie
                 * 
                 * \param[in] keys A set of keys for which to search
                 * 
                 * \return The results of the search
                 */
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
