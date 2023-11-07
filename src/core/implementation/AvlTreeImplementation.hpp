#pragma once

#include "../memory/memory.hpp"
#include "AvlTreeHelper.hpp"
#include "traits.hpp"

/**
 * @brief An AVL tree.
 * @tparam ValueType The type of the value stored in the tree.
 * @tparam Comparator The type of the comparator used to order the values in the
 * tree (default is std::less<ValueType>).
 * @note The comparator must satisfy the BinaryPredicate concept.
 */
template <typename ValueType,
          BinaryPredicate<ValueType, ValueType> Comparator = std::less<ValueType>>

class AvlTreeBase {
public:
    using self = AvlTreeBase;
    using size_type = std::size_t;
    using value_type = ValueType;
    using node_type = AvlTreeNode<value_type>;
    using node_pointer = node_type *;

    /**
     * @brief Default constructor.
     */
    AvlTreeBase();

    /**
     * @brief Destructor.
     */
    ~AvlTreeBase();

    /**
     * @brief Move constructor.
     * @param other The tree to be moved.
     */
    AvlTreeBase(AvlTreeBase &&other) noexcept;

    /**
     * @brief Copy constructor.
     * @param other The tree to be copied.
     */
    AvlTreeBase(const AvlTreeBase &other) noexcept;

    /**
     * @brief Move assignment operator.
     * @param other The tree to be moved.
     * @return Reference to the moved tree.
     */
    self &operator=(AvlTreeBase &&other) noexcept;

    /**
     * @brief Copy assignment operator.
     * @param other The tree to be copied.
     * @return Reference to the copied tree.
     */
    self &operator=(const AvlTreeBase &other) noexcept;

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
     * @brief Copy a node and its descendants.
     * @param node The root of the subtree to be copied.
     * @param parent The parent of the copied node.
     * @return The root of the copied subtree.
     */
    node_pointer copy(node_pointer node, node_pointer parent) const &noexcept;

    /**
     * @brief Clear the tree.
     */
    void clear() &noexcept;

    /**
     * @brief Swap the positions of two nodes in the tree.
     * @param a The first node.
     * @param b The second node.
     */
    void swap_nodes(node_pointer a, node_pointer b) &noexcept;

    /**
     * @brief Helper function for swap_nodes. It is called to change old_child to new_child.
     * @param new_child The new new_child of parent.
     * @param old_child The old child of parent.
     * @param parent The parent of old child.
     * @return nothing.
     */
    void update_child(node_pointer new_child,
                      node_pointer old_child,
                      node_pointer parent) &noexcept;

    /**
     * @brief Helper function for swap_nodes.
     * @par It is called to solve situation when child.parent is child and parent.parent is parent
     * @par and parent is logicaly is a real parent of child.
     * @par if child.parent != child when nothing happens.
     * @param child - child node with cycle.
     * @param parent - parent node with cycle.
     * @return nothing.
     */
    void solve_parent_cycle(node_pointer child, node_pointer parent) &noexcept;

    /**
     * @brief Insert a node into the tree.
     * @param node The node to be inserted.
     * @note The node must be allocated in heap otherwise SEGFAULT will cause in clear function.
     * @note Node will be inserted even if it already exists in the tree.
     */
    void insert(node_pointer node) &noexcept;

    /**
     * @brief Remove a node from the tree.
     * @param node The node to be removed.
     * @note It is guaranteed that no references is invalidated (except for node).
     */
    void erase(node_pointer node) &noexcept;

    /**
     * @brief Remove a node with a specific value from the tree.
     * @param value The value of the node to be removed.
     * @note It is guaranteed that no references is invalidated (except for node).
     */
    void erase(const value_type &value) &noexcept;

    /**
     * @brief Find a node with a specific value in the tree.
     * @param value The value of the node to be found.
     * @return The node with the specific value.
     */
    node_pointer find(const value_type &value) const &noexcept;

    /**
     * @brief Recursively destroy a subtree.
     * @param root The root of the subtree to be destroyed.
     * @note root will be destroyed, but it will not be set nullptr, because it is developer/user
     * responsibility.
     */
    void recursive_destroy(node_pointer root) &noexcept;

    /**
     * @brief Rotate a node to the left.
     * @param node The node to be rotated.
     * @return The new root of the subtree.
     */
    node_pointer rotate_left(node_pointer node) &noexcept;

    /**
     * @brief Rotate a node to the right.
     * @param node The node to be rotated.
     * @return The new root of the subtree.
     */
    node_pointer rotate_right(node_pointer node) &noexcept;

    /**
     * @brief Perform a small left rotation on a node.
     * @param node The node to be rotated.
     * @return The new root of the subtree.
     */
    node_pointer small_left_rotate(node_pointer node) &noexcept;

    /**
     * @brief Perform a big left rotation on a node.
     * @param node The node to be rotated.
     * @return The new root of the subtree.
     */
    node_pointer big_left_rotate(node_pointer node) &noexcept;

    /**
     * @brief Perform a small right rotation on a node.
     * @param node The node to be rotated.
     * @return The new root of the subtree.
     */
    node_pointer small_right_rotate(node_pointer node) &noexcept;

    /**
     * @brief Perform a big right rotation on a node.
     * @param node The node to be rotated.
     * @return The new root of the subtree.
     */
    node_pointer big_right_rotate(node_pointer node) &noexcept;

    /**
     * @brief Balance a node.
     * @param node The node to be balanced.
     * @return The new root of the subtree.
     */
    node_pointer balance_node(node_pointer node) &noexcept;

    /**
     * @brief Rebalance a subtree and all ancestors.
     * @param node The root of the subtree to be rebalanced.
     */
    void rebalance(node_pointer node) &noexcept;

public:
    node_pointer root_;
    size_type number_of_nodes_;
    Comparator compare_;
};

template <typename ValueType, BinaryPredicate<ValueType, ValueType> Comparator>
AvlTreeBase<ValueType, Comparator>::AvlTreeBase() : root_(nullptr), number_of_nodes_(0)
{}

template <typename ValueType, BinaryPredicate<ValueType, ValueType> Comparator>
AvlTreeBase<ValueType, Comparator>::~AvlTreeBase()
{
    clear();
}

template <typename ValueType, BinaryPredicate<ValueType, ValueType> Comparator>
AvlTreeBase<ValueType, Comparator>::AvlTreeBase(AvlTreeBase &&other) noexcept
{
    root_ = std::exchange(other.root_, nullptr);
    number_of_nodes_ = std::exchange(other.number_of_nodes_, 0);
}

template <typename ValueType, BinaryPredicate<ValueType, ValueType> Comparator>
AvlTreeBase<ValueType, Comparator>::AvlTreeBase(const AvlTreeBase &other) noexcept
    : compare_(other.compare_), number_of_nodes_(other.number_of_nodes_)
{
    root_ = copy(other.root_, nullptr);
}

template <typename ValueType, BinaryPredicate<ValueType, ValueType> Comparator>
auto AvlTreeBase<ValueType, Comparator>::operator=(AvlTreeBase &&other) noexcept -> self &
{
    if (this != &other)
    {
        root_ = std::exchange(other.root_, nullptr);
        number_of_nodes_ = std::exchange(other.number_of_nodes_, 0);
    }

    return *this;
}

template <typename ValueType, BinaryPredicate<ValueType, ValueType> Comparator>
auto AvlTreeBase<ValueType, Comparator>::operator=(const AvlTreeBase &other) noexcept
    -> AvlTreeBase::self &
{
    if (this != &other)
    {
        clear();

        compare_ = other.compare_;
        number_of_nodes_ = other.number_of_nodes_;

        root_ = copy(other.root_, nullptr);
    }

    return *this;
}

template <typename ValueType, BinaryPredicate<ValueType, ValueType> Comparator>
auto AvlTreeBase<ValueType, Comparator>::size() const &noexcept -> size_type
{
    return number_of_nodes_;
}

template <typename ValueType, BinaryPredicate<ValueType, ValueType> Comparator>
bool AvlTreeBase<ValueType, Comparator>::empty() const &noexcept
{
    return size() == 0;
}

template <typename ValueType, BinaryPredicate<ValueType, ValueType> Comparator>
auto AvlTreeBase<ValueType, Comparator>::copy(node_pointer node,
                                              node_pointer parent) const &noexcept -> node_pointer
{
    if (node == nullptr || node->is_placeholder())
    {
        return nullptr;
    }

    auto new_node = new node_type(node->value_);

    new_node->parent_ = parent;
    new_node->left_ = copy(node->left_, new_node);
    new_node->right_ = copy(node->right_, new_node);

    return new_node;
}

template <typename ValueType, BinaryPredicate<ValueType, ValueType> Comparator>
void AvlTreeBase<ValueType, Comparator>::clear() &noexcept
{
    recursive_destroy(root_);
    root_ = nullptr;
    number_of_nodes_ = 0;
}

template <typename ValueType, BinaryPredicate<ValueType, ValueType> Comparator>
void AvlTreeBase<ValueType, Comparator>::swap_nodes(node_pointer a, node_pointer b) &noexcept
{
    /*
     * Swap the pointers and heights of the nodes a and b.
     */
    std::swap(a->parent_, b->parent_);
    std::swap(a->left_, b->left_);
    std::swap(a->right_, b->right_);
    std::swap(a->height_, b->height_);

    /*
     * Edge case: one node was the parent of another node.
     */
    solve_parent_cycle(a, b);
    solve_parent_cycle(b, a);

    /*
     * Update the left and right pointers of the parent nodes.
     */
    update_child(a, b, a->parent_);
    update_child(b, a, b->parent_);

    /*
     * Update the parent pointers of the child nodes.
     */
    a->update_parent_for_children();
    b->update_parent_for_children();
}

template <typename ValueType, BinaryPredicate<ValueType, ValueType> Comparator>
void AvlTreeBase<ValueType, Comparator>::update_child(node_pointer new_child,
                                                      node_pointer old_child,
                                                      node_pointer parent) &noexcept
{
    if (parent != nullptr)
    {
        if (parent->left_ == old_child)
        {
            parent->left_ = new_child;
        }
        else
        {
            parent->right_ = new_child;
        }
    }
}

template <typename ValueType, BinaryPredicate<ValueType, ValueType> Comparator>
void AvlTreeBase<ValueType, Comparator>::solve_parent_cycle(node_pointer child,
                                                            node_pointer parent) &noexcept
{
    if (child == nullptr || parent == nullptr)
    {
        return;
    }

    if (child->parent_ == child)
    {
        child->parent_ = parent;

        if (parent->right_ == parent)
        {
            parent->right_ = child;
        }
        else
        {
            parent->left_ = child;
        }
    }
}

template <typename ValueType, BinaryPredicate<ValueType, ValueType> Comparator>
void AvlTreeBase<ValueType, Comparator>::insert(node_pointer node) &noexcept
{
    if (root_ == nullptr)
    {
        root_ = node;
        ++number_of_nodes_;
        return;
    }

    auto leaf_node = root_;

    /*
     * Find the correct position to insert the node (most suitable leaf node).
     */
    while (leaf_node != nullptr)
    {
        const auto &expression = compare_(node->value_, leaf_node->value_);
        const auto new_leaf_node = expression ? leaf_node->left_ : leaf_node->right_;

        if (new_leaf_node == nullptr)
        {
            expression ? leaf_node->left_ = node : leaf_node->right_ = node;
            node->parent_ = leaf_node;
            break;
        }

        leaf_node = new_leaf_node;
    }

    ++number_of_nodes_;

    node->iterative_height_update();
    rebalance(node->parent_);
}

template <typename ValueType, BinaryPredicate<ValueType, ValueType> Comparator>
void AvlTreeBase<ValueType, Comparator>::erase(node_pointer node) &noexcept
{
    if (node == nullptr)
    {
        return;
    }
    if (number_of_nodes_ == 1)
    {
        clear();
        return;
    }

    /*
     * If node has two children, then swap the node with the successor to delete the successor
     * instead of node.
     */
    if (node->left_ != nullptr && node->right_ != nullptr)
    {
        swap_nodes(node, node->successor());
    }

    auto parent = node->parent_;

    if (parent == nullptr)
    {
        parent = root_ = root_->left_ != nullptr ? root_->left_ : root_->right_;
        safe_delete(root_->parent_);
    }
    else
    {
        auto child = node->right_ == nullptr ? node->left_ : node->right_;

        if (child != nullptr)
        {
            update_child(child, node, parent);
            parent->update_parent_for_children();

            safe_delete(node);
        }
        else
        {
            safe_delete(parent->left_ == node ? parent->left_ : parent->right_);
        }

        parent->iterative_height_update();
    }

    rebalance(parent);
    --number_of_nodes_;
}

template <typename ValueType, BinaryPredicate<ValueType, ValueType> Comparator>
void AvlTreeBase<ValueType, Comparator>::erase(const value_type &value) &noexcept
{
    erase(find(value));
}

template <typename ValueType, BinaryPredicate<ValueType, ValueType> Comparator>
auto AvlTreeBase<ValueType, Comparator>::find(const value_type &value) const &noexcept
    -> node_pointer
{
    node_pointer node = root_;

    while (node != nullptr && node->value_ != value)
    {
        node = compare_(value, node->value_) ? node->left_ : node->right_;
    }

    return node;
}

template <typename ValueType, BinaryPredicate<ValueType, ValueType> Comparator>
void AvlTreeBase<ValueType, Comparator>::recursive_destroy(node_pointer root) &noexcept
{
    if (root == nullptr)
    {
        return;
    }

    recursive_destroy(root->left_);
    recursive_destroy(root->right_);

    /*
     * Both leafs destroyed, so destroy root
     */
    delete root;
}

template <typename ValueType, BinaryPredicate<ValueType, ValueType> Comparator>
auto AvlTreeBase<ValueType, Comparator>::rotate_left(node_pointer node) &noexcept -> node_pointer
{
    auto new_root = node->right_;

    node->right_ = new_root->left_;
    node->update_parent_for_right_child();

    new_root->left_ = node;
    new_root->update_parent_for_left_child();

    node->iterative_height_update();
    new_root->iterative_height_update();

    return new_root;
}

template <typename ValueType, BinaryPredicate<ValueType, ValueType> Comparator>
auto AvlTreeBase<ValueType, Comparator>::rotate_right(node_pointer node) &noexcept -> node_pointer
{
    auto new_root = node->left_;

    node->left_ = new_root->right_;
    node->update_parent_for_left_child();

    new_root->right_ = node;
    new_root->update_parent_for_right_child();

    node->iterative_height_update();
    new_root->iterative_height_update();

    return new_root;
}

template <typename ValueType, BinaryPredicate<ValueType, ValueType> Comparator>
auto AvlTreeBase<ValueType, Comparator>::small_left_rotate(node_pointer node) &noexcept
    -> node_pointer
{
    if (node->parent_ == nullptr)
    {
        node = rotate_left(node);
        node->parent_ = nullptr;
        return node;
    }

    auto parent = node->parent_;
    node_pointer &child_node = parent->left_ == node ? parent->left_ : parent->right_;

    child_node = rotate_left(child_node);
    child_node->parent_ = parent;
    parent->iterative_height_update();

    return child_node;
}

template <typename ValueType, BinaryPredicate<ValueType, ValueType> Comparator>
auto AvlTreeBase<ValueType, Comparator>::big_left_rotate(node_pointer node) &noexcept
    -> node_pointer
{
    node->right_ = small_right_rotate(node->right_);
    node->update_parent_for_right_child();
    node->iterative_height_update();

    return small_left_rotate(node);
}

template <typename ValueType, BinaryPredicate<ValueType, ValueType> Comparator>
auto AvlTreeBase<ValueType, Comparator>::small_right_rotate(node_pointer node) &noexcept
    -> node_pointer
{
    if (node->parent_ == nullptr)
    {
        node = rotate_right(node);
        node->parent_ = nullptr;
        return node;
    }

    auto parent = node->parent_;
    node_pointer &child_node = parent->left_ == node ? parent->left_ : parent->right_;

    child_node = rotate_right(child_node);
    child_node->parent_ = parent;
    parent->iterative_height_update();

    return child_node;
}

template <typename ValueType, BinaryPredicate<ValueType, ValueType> Comparator>
auto AvlTreeBase<ValueType, Comparator>::big_right_rotate(node_pointer node) &noexcept
    -> node_pointer
{
    node->left_ = small_left_rotate(node->left_);
    node->update_parent_for_left_child();
    node->iterative_height_update();

    return small_right_rotate(node);
}

template <typename ValueType, BinaryPredicate<ValueType, ValueType> Comparator>
auto AvlTreeBase<ValueType, Comparator>::balance_node(node_pointer node) &noexcept -> node_pointer
{
    if (node == nullptr)
    {
        return node;
    }

    const auto balance_factor = node->balance_factor();

    if (balance_factor > 1)
    {
        if (node->right_->balance_factor() >= 0)
        {
            node = small_left_rotate(node);
        }
        else
        {
            node = big_left_rotate(node);
        }
    }
    else if (balance_factor < -1)
    {
        if (node->left_->balance_factor() <= 0)
        {
            node = small_right_rotate(node);
        }
        else
        {
            node = big_right_rotate(node);
        }
    }

    return node;
}

template <typename ValueType, BinaryPredicate<ValueType, ValueType> Comparator>
void AvlTreeBase<ValueType, Comparator>::rebalance(node_pointer node) &noexcept
{
    auto current = node;

    while (current != nullptr)
    {
        current = balance_node(current);

        if (current->parent_ == nullptr)
        {
            root_ = current;
        }

        current = current->parent_;
    }
}
