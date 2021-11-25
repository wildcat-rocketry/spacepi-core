#ifndef SPACEPI_CORE_TEST_ELEMENTTYPE_HPP
#define SPACEPI_CORE_TEST_ELEMENTTYPE_HPP

namespace spacepi {
    namespace test {
        class ElementType {
            public:
                enum ConstructorType {
                    Default,
                    Init,
                    Tagged,
                    Copy,
                    Move,
                    Moved
                };

                class TagTag {
                } static constexpr Tag = TagTag();

                constexpr ElementType() noexcept
                    : type(Default), val(0) {
                }

                constexpr ElementType(int val) noexcept
                    : type(Init), val(val) {
                }

                constexpr ElementType(int val, const TagTag &tag) noexcept
                    : type(Tagged), val(val) {
                }

                constexpr ElementType(const ElementType &copy) noexcept
                    : type(Copy), val(copy.val) {
                }

                constexpr ElementType(ElementType &&move) noexcept
                    : type(Move), val(move.val) {
                    move.type = Moved;
                }

                constexpr ElementType &operator=(int val) noexcept {
                    type = Init;
                    this->val = val;
                    return *this;
                }

                constexpr ElementType &operator=(const ElementType &copy) noexcept {
                    type = Copy;
                    val = copy.val;
                    return *this;
                }

                constexpr ElementType &operator=(ElementType &&move) noexcept {
                    type = Move;
                    val = move.val;
                    move.type = Moved;
                    return *this;
                }

                constexpr ConstructorType getType() const noexcept {
                    return type;
                }

                constexpr int getValue() const noexcept {
                    return val;
                }

            private:
                ConstructorType type;
                int val;
        };
    }
}

#endif
