#pragma once

#include "AvlTreeHelper.hpp"
#include "AvlTreeNode.hpp"

#include <iterator>

/**
 * @brief An iterator for AVL tree.
 * @tparam ValueType The type of the value stored in the tree node.
 */
template <typename ValueType>
class AvlTreeIterator : public std::iterator<std::bidirectional_iterator_tag,
                                             ValueType,
                                             std::ptrdiff_t,
                                             const ValueType *,
                                             const ValueType &> {
public:
    template <typename T, class C>
    friend class avl_tree;

public:
    using value_type = ValueType;
    using pointer_type = AvlTreeNode<value_type>::pointer_type;
    using self = AvlTreeIterator<value_type>;
    using reference = const value_type &;
    using pointer = const value_type *;

    /**
     * @brief Default constructor.
     */
    AvlTreeIterator();

    /**
     * @brief Constructor with a node pointer.
     * @param node The node pointer.
     */
    explicit AvlTreeIterator(const pointer_type &node) noexcept;

    /**
     * @brief Dereference operator.
     * @return The value of the node.
     */
    reference operator*() const &noexcept;

    /**
     * @brief Arrow operator.
     * @return The pointer to the value of the node.
     */
    pointer operator->() const &noexcept;

    /**
     * @brief Prefix increment operator.
     * @return Reference to the incremented iterator.
     */
    self &operator++() &noexcept;

    /**
     * @brief Postfix increment operator.
     * @return The iterator before increment.
     */
    self operator++(int) const &noexcept;

    /**
     * @brief Prefix decrement operator.
     * @return Reference to the decremented iterator.
     */
    self &operator--() &noexcept;

    /**
     * @brief Postfix decrement operator.
     * @return The iterator before decrement.
     */
    self operator--(int) const &noexcept;

    /**
     * @brief Inequality operator.
     * @param other The other iterator.
     * @return True if the two iterators are not equal.
     */
    bool operator!=(const AvlTreeIterator<value_type> &other) const &noexcept;

    /**
     * @brief Equality operator.
     * @param other The other iterator.
     * @return True if the two iterators are equal.
     */
    bool operator==(const AvlTreeIterator<value_type> &other) const &noexcept;

private:
    pointer_type node_;
};

template <typename ValueType>
AvlTreeIterator<ValueType>::AvlTreeIterator() : node_(nullptr)
{}

template <typename ValueType>
AvlTreeIterator<ValueType>::AvlTreeIterator(const pointer_type &node) noexcept : node_(node)
{}

template <typename ValueType>
auto AvlTreeIterator<ValueType>::operator*() const &noexcept -> reference
{
    return node_->value_;
}

template <typename ValueType>
auto AvlTreeIterator<ValueType>::operator->() const &noexcept -> pointer
{
    return &node_->value_;
}

template <typename ValueType>
auto AvlTreeIterator<ValueType>::operator++() &noexcept -> self &
{
    node_ = avl_tree_increment<ValueType>(node_);
    return *this;
}

template <typename ValueType>
auto AvlTreeIterator<ValueType>::operator++(int) const &noexcept -> self
{
    self temp = *this;
    ++(*this);
    return temp;
}

template <typename ValueType>
auto AvlTreeIterator<ValueType>::operator--() &noexcept -> self &
{
    node_ = avl_tree_decrement<ValueType>(node_);
    return *this;
}

template <typename ValueType>
auto AvlTreeIterator<ValueType>::operator--(int) const &noexcept -> self
{
    self temp = *this;
    --(*this);
    return temp;
}

template <typename ValueType>
bool AvlTreeIterator<ValueType>::operator!=(
    const AvlTreeIterator<value_type> &other) const &noexcept
{
    return node_ != other.node_;
}

template <typename ValueType>
bool AvlTreeIterator<ValueType>::operator==(
    const AvlTreeIterator<value_type> &other) const &noexcept
{
    return node_ == other.node_;
}
