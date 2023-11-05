#pragma once

#include "AvlTreeHelper.hpp"

/**
 * @brief An AVL tree.
 * @tparam ValueType The type of the value stored in the tree.
 * @tparam Comparator The type of the comparator used to order the values in the
 * tree (default is std::less<ValueType>).
 */
template <typename ValueType, class Comparator = std::less<ValueType>>
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
     * @brief Insert a node into the tree.
     * @param node The node to be inserted.
     */
    void insert(node_pointer node) &noexcept;

    /**
     * @brief Remove a node from the tree.
     * @param node The node to be removed.
     */
    void erase(node_pointer node) &noexcept;

    /**
     * @brief Remove a node with a specific value from the tree.
     * @param value The value of the node to be removed.
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
     * @param node The root of the subtree to be destroyed.
     */
    void recursive_destroy(node_pointer node) &noexcept;

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
     * @brief Rebalance a subtree.
     * @param node The root of the subtree to be rebalanced.
     */
    void rebalance(node_pointer node) &noexcept;

public:
    node_pointer root_;
    size_type number_of_nodes_;
    Comparator compare_;
};

template <typename ValueType, class Comparator>
AvlTreeBase<ValueType, Comparator>::AvlTreeBase() : root_(nullptr), number_of_nodes_(0)
{}

template <typename ValueType, class Comparator>
AvlTreeBase<ValueType, Comparator>::~AvlTreeBase()
{
    clear();
}

template <typename ValueType, class Comparator>
AvlTreeBase<ValueType, Comparator>::AvlTreeBase(AvlTreeBase &&other) noexcept
{
    root_ = other.root_;
    number_of_nodes_ = other.number_of_nodes_;
    other.root_ = nullptr;
    other.number_of_nodes_ = 0;
}

template <typename ValueType, class Comparator>
AvlTreeBase<ValueType, Comparator>::AvlTreeBase(const AvlTreeBase &other) noexcept
    : compare_(other.compare_), number_of_nodes_(other.number_of_nodes_)
{
    root_ = copy(other.root_, nullptr);
}

template <typename ValueType, class Comparator>
auto AvlTreeBase<ValueType, Comparator>::operator=(AvlTreeBase &&other) noexcept -> self &
{
    if (this != &other)
    {
        root_ = other.root_;
        number_of_nodes_ = other.number_of_nodes_;
        other.root_ = nullptr;
        other.number_of_nodes_ = 0;
    }

    return *this;
}

template <typename ValueType, class Comparator>
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

template <typename ValueType, class Comparator>
auto AvlTreeBase<ValueType, Comparator>::size() const &noexcept -> size_type
{
    return number_of_nodes_;
}

template <typename ValueType, class Comparator>
bool AvlTreeBase<ValueType, Comparator>::empty() const &noexcept
{
    return size() == 0;
}

template <typename ValueType, class Comparator>
auto AvlTreeBase<ValueType, Comparator>::copy(node_pointer node,
                                              node_pointer parent) const &noexcept -> node_pointer
{
    if (node == nullptr)
    {
        return nullptr;
    }

    auto new_node = new node_type(node->value_);
    new_node->parent_ = parent;
    new_node->left_ = copy(node->left_, new_node);
    new_node->right_ = copy(node->right_, new_node);

    return new_node;
}

template <typename ValueType, class Comparator>
void AvlTreeBase<ValueType, Comparator>::clear() &noexcept
{
    recursive_destroy(root_);
    root_ = nullptr;
    number_of_nodes_ = 0;
}

template <typename ValueType, class Comparator>
void AvlTreeBase<ValueType, Comparator>::swap_nodes(node_pointer a, node_pointer b) &noexcept
{
    // Swap the pointers
    std::swap(a->parent_, b->parent_);
    std::swap(a->left_, b->left_);
    std::swap(a->right_, b->right_);

    // Swap the heights
    std::swap(a->height_, b->height_);

    // Edge case when one node is parent of another
    if (a->parent_ == a)
    {
        a->parent_ = b;

        if (b->right_ == b)
        {
            b->right_ = a;
        }
        else
        {
            b->left_ = a;
        }
    }
    else if (b->parent_ == b)
    {
        b->parent_ = a;

        if (a->right_ == a)
        {
            a->right_ = b;
        }
        else
        {
            a->left_ = b;
        }
    }

    // Update the left and right pointers of the parent nodes
    if (a->parent_ != nullptr)
    {
        if (a->parent_->left_ == b)
        {
            a->parent_->left_ = a;
        }
        else
        {
            a->parent_->right_ = a;
        }
    }
    if (b->parent_ != nullptr)
    {
        if (b->parent_->left_ == a)
        {
            b->parent_->left_ = b;
        }
        else
        {
            b->parent_->right_ = b;
        }
    }

    // Update the parent pointers of the child nodes
    if (a->left_ != nullptr)
    {
        a->left_->parent_ = a;
    }
    if (a->right_ != nullptr)
    {
        a->right_->parent_ = a;
    }
    if (b->left_ != nullptr)
    {
        b->left_->parent_ = b;
    }
    if (b->right_ != nullptr)
    {
        b->right_->parent_ = b;
    }
}

template <typename ValueType, class Comparator>
void AvlTreeBase<ValueType, Comparator>::insert(node_pointer node) &noexcept
{
    if (root_ == nullptr)
    {
        root_ = node;
        ++number_of_nodes_;
        return;
    }

    node_pointer parent = root_;

    while (parent != nullptr)
    {
        if (compare_(node->value_, parent->value_))
        {
            if (parent->left_ == nullptr)
            {
                parent->left_ = node;
                node->parent_ = parent;
                break;
            }

            parent = parent->left_;
        }
        else
        {
            if (parent->right_ == nullptr)
            {
                parent->right_ = node;
                node->parent_ = parent;
                break;
            }

            parent = parent->right_;
        }
    }

    ++number_of_nodes_;

    // Update heights for all ancestors
    node->iterative_height_update();

    // Balance nodes starting from parent
    rebalance(node->parent_);
}

template <typename ValueType, class Comparator>
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

    // If node has two children, then swap value with successor and delete him
    // instead.
    if (node->left_ != nullptr && node->right_ != nullptr)
    {
        swap_nodes(node, node->successor());
    }

    node_pointer parent = node->parent_;

    if (parent == nullptr)
    {
        root_ = root_->left_ != nullptr ? root_->left_ : root_->right_;
        delete root_->parent_;
        root_->parent_ = nullptr;
        parent = root_;
    }
    else
    {
        node_pointer child = node->right_ == nullptr ? node->left_ : node->right_;

        if (child != nullptr)
        {
            if (parent->left_ == node)
            {
                parent->left_ = child;
            }
            else
            {
                parent->right_ = child;
            }

            child->parent_ = parent;
            delete node;
        }
        else
        {
            if (parent->left_ == node)
            {
                delete parent->left_;
                parent->left_ = nullptr;
            }
            else
            {
                delete parent->right_;
                parent->right_ = nullptr;
            }
        }

        parent->iterative_height_update();
    }

    rebalance(parent);
    --number_of_nodes_;
}

template <typename ValueType, class Comparator>
void AvlTreeBase<ValueType, Comparator>::erase(const value_type &value) &noexcept
{
    erase(find(value));
}

template <typename ValueType, class Comparator>
auto AvlTreeBase<ValueType, Comparator>::find(const value_type &value) const &noexcept
    -> node_pointer
{
    node_pointer node = root_;

    while (node != nullptr && node->value_ != value)
    {
        if (compare_(value, node->value_))
        {
            node = node->left_;
        }
        else
        {
            node = node->right_;
        }
    }

    return node;
}

template <typename ValueType, class Comparator>
void AvlTreeBase<ValueType, Comparator>::recursive_destroy(node_pointer node) &noexcept
{
    if (node == nullptr)
    {
        return;
    }
    if (node->left_ != nullptr)
    {
        recursive_destroy(node->left_);
    }
    if (node->right_ != nullptr)
    {
        recursive_destroy(node->right_);
    }

    // Both leafs destroyed, so destroy root
    delete node;
}

template <typename ValueType, class Comparator>
auto AvlTreeBase<ValueType, Comparator>::rotate_left(node_pointer node) &noexcept -> node_pointer
{
    node_pointer new_root = node->right_;

    node->right_ = new_root->left_;

    if (node->right_ != nullptr)
    {
        node->right_->parent_ = node;
    }

    new_root->left_ = node;
    new_root->left_->parent_ = new_root;

    node->iterative_height_update();
    new_root->iterative_height_update();

    return new_root;
}

template <typename ValueType, class Comparator>
auto AvlTreeBase<ValueType, Comparator>::rotate_right(node_pointer node) &noexcept -> node_pointer
{
    node_pointer new_root = node->left_;

    node->left_ = new_root->right_;

    if (node->left_ != nullptr)
    {
        node->left_->parent_ = node;
    }

    new_root->right_ = node;
    new_root->right_->parent_ = new_root;

    node->iterative_height_update();
    new_root->iterative_height_update();

    return new_root;
}

template <typename ValueType, class Comparator>
auto AvlTreeBase<ValueType, Comparator>::small_left_rotate(node_pointer node) &noexcept
    -> node_pointer
{
    if (node->parent_ == nullptr)
    {
        node = rotate_left(node);
        node->parent_ = nullptr;
        return node;
    }

    node_pointer parent = node->parent_;

    if (parent->left_ == node)
    {
        parent->left_ = rotate_left(node);
        parent->left_->parent_ = parent;
        parent->iterative_height_update();

        return parent->left_;
    }

    parent->right_ = rotate_left(node);
    parent->right_->parent_ = parent;
    parent->iterative_height_update();

    return parent->right_;
}

template <typename ValueType, class Comparator>
auto AvlTreeBase<ValueType, Comparator>::big_left_rotate(node_pointer node) &noexcept
    -> node_pointer
{
    node->right_ = small_right_rotate(node->right_);
    node->right_->parent_ = node;
    node->iterative_height_update();

    return small_left_rotate(node);
}

template <typename ValueType, class Comparator>
auto AvlTreeBase<ValueType, Comparator>::small_right_rotate(node_pointer node) &noexcept
    -> node_pointer
{
    if (node->parent_ == nullptr)
    {
        node = rotate_right(node);
        node->parent_ = nullptr;
        return node;
    }

    node_pointer parent = node->parent_;
    node->parent_ = nullptr;

    if (parent->left_ == node)
    {
        parent->left_ = rotate_right(node);
        parent->left_->parent_ = parent;
        parent->iterative_height_update();

        return parent->left_;
    }

    parent->right_ = rotate_right(node);
    parent->right_->parent_ = parent;
    parent->iterative_height_update();

    return parent->right_;
}

template <typename ValueType, class Comparator>
auto AvlTreeBase<ValueType, Comparator>::big_right_rotate(node_pointer node) &noexcept
    -> node_pointer
{
    node->left_ = small_left_rotate(node->left_);
    node->left_->parent_ = node;
    node->iterative_height_update();

    return small_right_rotate(node);
}

template <typename ValueType, class Comparator>
auto AvlTreeBase<ValueType, Comparator>::balance_node(node_pointer node) &noexcept -> node_pointer
{
    if (node == nullptr)
    {
        return node;
    }

    auto balance_factor = node->balance_factor();

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

template <typename ValueType, class Comparator>
void AvlTreeBase<ValueType, Comparator>::rebalance(node_pointer node) &noexcept
{
    node_pointer current = node;

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
