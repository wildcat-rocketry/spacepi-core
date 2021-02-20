#ifndef SPACEPI_CORE_PACKAGE_OPTIONS_HPP
#define SPACEPI_CORE_PACKAGE_OPTIONS_HPP

#include <memory>
#include <stack>
#include <string>
#include <unordered_map>

namespace spacepi {
    namespace package {
        /**
         * \brief Data class to represent a list of repeatable, ordered, string-keyed, string-valued options
         */
        class Options final {
            public:
                /**
                 * \brief Node which represents a single option
                 */
                class Node final {
                    friend class Options;

                    public:
                        /**
                         * \brief Get the string key for the option
                         * 
                         * \return The key
                         */
                        const std::string &getKey() const noexcept;

                        /**
                         * \brief Set the string key for the option
                         * 
                         * \param[in] key The key
                         */
                        void setKey(const std::string &key) noexcept;

                        /**
                         * \brief Get the string value for the option
                         * 
                         * \return The value
                         */
                        const std::string &getValue() const noexcept;
                        
                        /**
                         * \brief Set the string value for the option
                         * 
                         * \param[in] value The value
                         */
                        void setValue(const std::string &value) noexcept;

                    private:
                        std::shared_ptr<Node> next;
                        std::shared_ptr<Node> prev;
                        std::string key;
                        std::string value;
                };

                /**
                 * \brief An iterator to iterate over the list of options
                 */
                class ConstIterator {
                    public:
                        /**
                         * \brief Initialize an invalid iterator
                         */
                        ConstIterator() noexcept = default;

                        /**
                         * \brief Initialize an iterator
                         * 
                         * \param[in] node The head Node of the list
                         */
                        ConstIterator(const std::shared_ptr<Node> &node) noexcept;

                        /**
                         * \brief Destroy this ConstIterator
                         */
                        virtual ~ConstIterator() = default;

                        /**
                         * \brief Advance the iterator
                         * 
                         * \return \c this
                         */
                        ConstIterator &operator ++() noexcept;

                        /**
                         * \brief Get the node at the current iterator position
                         * 
                         * \return The node
                         */
                        const Node &operator *() const noexcept;

                        /**
                         * \brief Call a function on the node at the current iterator position
                         */
                        const Node *operator ->() const noexcept;

                        /**
                         * \brief Get the node at the current iterator position
                         * 
                         * \return The node
                         */
                        const std::shared_ptr<Node> get() const noexcept;

                        /**
                         * \brief Check iterator equality
                         * 
                         * \param[in] other The other iterator to compare against
                         * 
                         * \return The comparison result
                         */
                        bool operator ==(const ConstIterator &other) const noexcept;

                        /**
                         * \copydoc operator==
                         */
                        bool operator !=(const ConstIterator &other) const noexcept;

                    protected:
                        /**
                         * \brief The current node
                         */
                        std::weak_ptr<Node> node;
                };

                /**
                 * \brief An iterator to iterate over the list of options
                 */
                class Iterator final : public ConstIterator {
                    public:
                        /**
                         * \copydoc ConstIterator::ConstIterator()
                         */
                        Iterator() noexcept = default;

                        /**
                         * \brief Initialize an iterator
                         * 
                         * \param[in] node The head Node of the list
                         */
                        Iterator(const std::shared_ptr<Node> &node) noexcept;

                        /**
                         * \copydoc ConstIterator::operator*
                         */
                        Node &operator *() noexcept;

                        /**
                         * \copydoc ConstIterator::operator->
                         */
                        Node *operator ->() noexcept;
                };

                /**
                 * \brief Gets the last instance of an option in the list
                 * 
                 * \param[in] key The key to search for
                 * 
                 * \return The value of the last option node with the given \c key
                 */
                const std::string &getOption(const std::string &key) const noexcept;

                /**
                 * \brief Sets the last instance of an option in the list (or create a new node if no node exists)
                 * 
                 * \param[in] key The key to search for
                 * \param[in] value The new value for the last option node with the given \c key
                 */
                void setOption(const std::string &key, const std::string &value) noexcept;

                /**
                 * \brief Append a new option node to the list
                 * 
                 * \param[in] key The key for the node to insert
                 * \param[in] value The value for the node to insert
                 * \param[in] position Where to insert the new node (default to the end of the list)
                 */
                void addOption(const std::string &key, const std::string &value, const ConstIterator &position = ConstIterator()) noexcept;

                /**
                 * \brief Remove the last instance of an option in the list
                 * 
                 * \param[in] key The key to search for
                 */
                void removeOption(const std::string &key) noexcept;

                /**
                 * \brief Remove all instances of an option in the list
                 * 
                 * \param[in] key The key to search for
                 */
                void clearOption(const std::string &key) noexcept;

                /**
                 * \brief Remove all options in the list
                 */
                void clear() noexcept;

                /**
                 * \copydoc begin()
                 */
                ConstIterator begin() const noexcept;

                /**
                 * \brief Get an iterator to the first node in the list
                 * 
                 * \return The iterator
                 */
                Iterator begin() noexcept;

                /**
                 * \copydoc end()
                 */
                ConstIterator end() const noexcept;

                /**
                 * \brief Get an iterator past the last node in the list
                 * 
                 * \return The iterator
                 */
                Iterator end() noexcept;

            private:
                std::shared_ptr<Node> head;
                std::shared_ptr<Node> tail;
                std::unordered_map<std::string, std::stack<std::shared_ptr<Node>>> sortedData;
        };
    }
}

#endif
