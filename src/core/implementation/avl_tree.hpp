#pragma once

#include "AvlTreeImplementation.hpp"
#include "AvlTreeIterator.hpp"
#include "traits.hpp"

#include <cstdint>

/**
 * @brief An AVL tree.
 * @tparam ValueType The type of the value stored in the tree.
 * @tparam Comparator The type of the comparator used to order the values in the
 * tree (default is std::less<ValueType>).
 */
template <typename ValueType,
          BinaryPredicate<ValueType, ValueType> Comparator = std::less<ValueType>>
class avl_tree {
public:
    using self = avl_tree;
    using value_type = ValueType;
    using _BaseImpl = AvlTreeBase<ValueType, Comparator>;
    using size_type = _BaseImpl::size_type;
    using node_type = _BaseImpl::node_type;
    using node_pointer = _BaseImpl::node_pointer;
    using iterator = AvlTreeIterator<value_type>;
    using reverse_iterator = std::reverse_iterator<iterator>;

    /**
     * @brief Default constructor.
     */
    avl_tree();

    /**
     * @brief Destructor.
     */
    ~avl_tree();

    /**
     * @brief Move constructor.
     * @param other The tree to be moved.
     */
    avl_tree(avl_tree &&other) noexcept;

    /**
     * @brief Copy constructor.
     * @param other The tree to be copied.
     */
    avl_tree(const avl_tree &other) noexcept;

    /**
     * @brief Move assignment operator.
     * @param other The tree to be moved.
     * @return Reference to the moved tree.
     */
    self &operator=(avl_tree &&other) noexcept;

    /**
     * @brief Copy assignment operator.
     * @param other The tree to be copied.
     * @return Reference to the copied tree.
     */
    self &operator=(const avl_tree &other) noexcept;

    /**
     * @brief Get the beginning iterator of the tree.
     * @return The beginning iterator of the tree.
     */
    iterator begin() const &noexcept;

    /**
     * @brief Get the ending iterator of the tree.
     * @return The ending iterator of the tree.
     */
    iterator end() const &noexcept;

    /**
     * @brief Get the reverse beginning iterator of the tree.
     * @return The reverse beginning iterator of the tree.
     */
    reverse_iterator rbegin() const &noexcept;

    /**
     * @brief Get the reverse ending iterator of the tree.
     * @return The reverse ending iterator of the tree.
     */
    reverse_iterator rend() const &noexcept;

    /**
     * @brief Get the size of the tree.
     * @return The size of the tree.
     */
    [[nodiscard]] size_type size() const &noexcept;

    /**
     * @brief Check if the tree is empty.
     * @return True if the tree is empty.
     */
    [[nodiscard]] bool empty() const &noexcept;

    /**
     * @brief Clear the tree.
     */
    void clear() &noexcept;

    /**
     * @brief Insert a value into the tree.
     * @param value The value to be inserted.
     * @return A pair consisting of an iterator to the inserted element (or to
     * the element that prevented the insertion) and a bool denoting whether the
     * insertion took place.
     */
    std::pair<iterator, bool> insert(const value_type &value) &noexcept;

    /**
     * @brief Insert a value into the tree.
     * @param value The value to be inserted.
     * @return A pair consisting of an iterator to the inserted element (or to
     * the element that prevented the insertion) and a bool denoting whether the
     * insertion took place.
     */
    std::pair<iterator, bool> insert(value_type &&value) &noexcept;

    /**
     * @brief Remove the node with the smallest value from the tree.
     */
    void erase_smallest() &noexcept;

    /**
     * @brief Remove the node with the largest value from the tree.
     */
    void erase_largest() &noexcept;

    /**
     * @brief Remove a value from the tree.
     * @param value The value to be removed.
     */
    void erase(const value_type &value) &noexcept;

    /**
     * @brief Remove an element from the tree.
     * @param pos An iterator pointing to the element to be removed.
     */
    void erase(const iterator &pos) &noexcept;

    /**
     * @brief Find an element with a specific value in the tree.
     * @param value The value of the element to be found.
     * @return An iterator pointing to the element with the specific value.
     */
    iterator find(const value_type &value) &noexcept;

    /**
     * @brief Find an element with a specific value in the tree.
     * @param value The value of the element to be found.
     * @return An iterator pointing to the element with the specific value.
     */
    iterator find(value_type &&value) &noexcept;

private:
    /**
     * @brief Clone the tree.
     * @param other The tree to be cloned.
     */
    void clone(const self &other) &noexcept;

    _BaseImpl base_;
    AvlTreeNode<value_type> header_;
};

template <typename ValueType, BinaryPredicate<ValueType, ValueType> Comparator>
void avl_tree<ValueType, Comparator>::clone(const self &other) &noexcept
{
    base_ = other.base_;

    const auto smallest = base_.root_->leftmost();
    const auto largest = base_.root_->rightmost();

    if (smallest != nullptr)
    {
        header_.left_ = smallest;
    }
    if (largest != nullptr)
    {
        header_.right_ = largest;
    }

    header_.restore_placeholder();
}

template <typename ValueType, BinaryPredicate<ValueType, ValueType> Comparator>
avl_tree<ValueType, Comparator>::avl_tree() : base_(), header_(0xffffff)
{
    clear();
}

template <typename ValueType, BinaryPredicate<ValueType, ValueType> Comparator>
avl_tree<ValueType, Comparator>::~avl_tree()
{
    clear();
}

template <typename ValueType, BinaryPredicate<ValueType, ValueType> Comparator>
avl_tree<ValueType, Comparator>::avl_tree(avl_tree &&other) noexcept
{
    base_ = std::move(other.base_);
    header_ = std::move(other.header_);
}

template <typename ValueType, BinaryPredicate<ValueType, ValueType> Comparator>
avl_tree<ValueType, Comparator>::avl_tree(const avl_tree &other) noexcept : avl_tree()
{
    clone(other);
}

template <typename ValueType, BinaryPredicate<ValueType, ValueType> Comparator>
auto avl_tree<ValueType, Comparator>::operator=(avl_tree &&other) noexcept -> self &
{
    if (this != &other)
    {
        base_ = std::move(other.base_);
        header_ = std::move(other.header_);
    }

    return *this;
}

template <typename ValueType, BinaryPredicate<ValueType, ValueType> Comparator>
auto avl_tree<ValueType, Comparator>::operator=(const avl_tree &other) noexcept -> self &
{
    if (this != &other)
    {
        clear();
        clone(other);
    }

    return *this;
}

template <typename ValueType, BinaryPredicate<ValueType, ValueType> Comparator>
auto avl_tree<ValueType, Comparator>::begin() const &noexcept -> iterator
{
    return iterator(header_.left_);
}

template <typename ValueType, BinaryPredicate<ValueType, ValueType> Comparator>
auto avl_tree<ValueType, Comparator>::end() const &noexcept -> iterator
{
    return iterator(header_.parent_);
}

template <typename ValueType, BinaryPredicate<ValueType, ValueType> Comparator>
auto avl_tree<ValueType, Comparator>::rbegin() const &noexcept -> reverse_iterator
{
    return reverse_iterator(end());
}

template <typename ValueType, BinaryPredicate<ValueType, ValueType> Comparator>
auto avl_tree<ValueType, Comparator>::rend() const &noexcept -> reverse_iterator
{
    return reverse_iterator(begin());
}

template <typename ValueType, BinaryPredicate<ValueType, ValueType> Comparator>
auto avl_tree<ValueType, Comparator>::size() const &noexcept -> size_type
{
    return base_.size();
}

template <typename ValueType, BinaryPredicate<ValueType, ValueType> Comparator>
bool avl_tree<ValueType, Comparator>::empty() const &noexcept
{
    return base_.empty();
}

template <typename ValueType, BinaryPredicate<ValueType, ValueType> Comparator>
void avl_tree<ValueType, Comparator>::clear() &noexcept
{
    header_.unlink_placeholder();
    header_.reset();
    base_.clear();
}

template <typename ValueType, BinaryPredicate<ValueType, ValueType> Comparator>
auto avl_tree<ValueType, Comparator>::insert(const value_type &value) &noexcept
    -> std::pair<iterator, bool>
{
    /*
     * If valus is already in the tree, return an iterator pointing to it.
     */
    if (const auto pos = find(value); pos != end())
    {
        return {pos, false};
    }

    header_.unlink_placeholder();

    auto node = new node_type(value);

    if (empty())
    {
        base_.insert(node);
        header_.left_ = base_.root_;
        header_.right_ = base_.root_;
    }
    else
    {
        /*
         * Keep track of the smallest and largest value in the tree.
         */
        const auto &smallest = header_.left_;
        const auto &greatest = header_.right_;

        if (base_.compare_(node->value_, smallest->value_))
        {
            header_.left_ = node;
        }
        else if (base_.compare_(greatest->value_, node->value_))
        {
            header_.right_ = node;
        }

        base_.insert(node);
    }

    header_.restore_placeholder();

    return {iterator(node), true};
}

template <typename ValueType, BinaryPredicate<ValueType, ValueType> Comparator>
auto avl_tree<ValueType, Comparator>::insert(value_type &&value) &noexcept
    -> std::pair<iterator, bool>
{
    return insert(static_cast<ValueType &>(value));
}

template <typename ValueType, BinaryPredicate<ValueType, ValueType> Comparator>
void avl_tree<ValueType, Comparator>::erase_smallest() &noexcept
{
    erase(iterator(header_.left_));
}

template <typename ValueType, BinaryPredicate<ValueType, ValueType> Comparator>
void avl_tree<ValueType, Comparator>::erase_largest() &noexcept
{
    erase(iterator(header_.right_));
}

template <typename ValueType, BinaryPredicate<ValueType, ValueType> Comparator>
void avl_tree<ValueType, Comparator>::erase(const value_type &value) &noexcept
{
    erase(find(value));
}

template <typename ValueType, BinaryPredicate<ValueType, ValueType> Comparator>
void avl_tree<ValueType, Comparator>::erase(const iterator &pos) &noexcept
{
    /*
     * Cannot erase placeholder node.
     */
    if (pos == end())
    {
        return;
    }
    if (size() == 1)
    {
        clear();
        return;
    }

    header_.unlink_placeholder();

    /*
     * Keep track of the smallest and largest value in the tree.
     */
    if (pos == begin())
    {
        header_.left_ = std::next(pos).node_;
    }
    else if (pos == std::prev(end()))
    {
        header_.right_ = std::prev(pos).node_;
    }

    base_.erase(pos.node_);

    header_.restore_placeholder();
}

template <typename ValueType, BinaryPredicate<ValueType, ValueType> Comparator>
auto avl_tree<ValueType, Comparator>::find(const value_type &value) &noexcept -> iterator
{
    header_.unlink_placeholder();

    auto ret = std::move(end());

    if (const auto node = base_.find(value); node != nullptr)
    {
        ret = std::move(iterator(node));
    }

    header_.restore_placeholder();

    return ret;
}

template <typename ValueType, BinaryPredicate<ValueType, ValueType> Comparator>
auto avl_tree<ValueType, Comparator>::find(value_type &&value) &noexcept -> iterator
{
    return find(static_cast<value_type &>(value));
}
