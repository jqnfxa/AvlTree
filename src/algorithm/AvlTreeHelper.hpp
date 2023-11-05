#pragma once

#include "AvlTreeNode.hpp"

/**
 * @brief Function to increment a node in an AVL tree.
 * @tparam ValueType The type of the value stored in the tree node.
 * @param node The node to increment.
 * @return The incremented node (next).
 */
template <typename ValueType>
auto avl_tree_increment(typename AvlTreeNode<ValueType>::pointer_type node) ->
    typename AvlTreeNode<ValueType>::pointer_type
{
    using pointer_type = typename AvlTreeNode<ValueType>::pointer_type;

    if (node == nullptr)
    {
        return nullptr;
    }

    // If placeholder node detected then increment does not perform.
    if (node->is_placeholder())
    {
        return node;
    }

    if (node->right_ != nullptr)
    {
        // If node has right child then go to leftmost node of this child.
        node = node->right_;

        if (!node->is_placeholder())
        {
            while (node->left_ != nullptr)
            {
                node = node->left_;
            }
        }
    }
    else
    {
        // We've reached the leaf node, so we're coming out of it
        pointer_type old = node;

        while (node->parent_ != nullptr)
        {
            node = node->parent_;

            if (node->left_ == old)
            {
                break;
            }

            old = node;
        }
    }

    return node;
}

/**
 * @brief Function to decrement a node in an AVL tree.
 * @tparam ValueType The type of the value stored in the tree node.
 * @param node The node to decrement.
 * @return The decremented node (previous).
 */
template <typename ValueType>
auto avl_tree_decrement(typename AvlTreeNode<ValueType>::pointer_type node) ->
    typename AvlTreeNode<ValueType>::pointer_type
{
    using pointer_type = typename AvlTreeNode<ValueType>::pointer_type;

    if (node == nullptr)
    {
        return nullptr;
    }

    /*
     * If placeholder node detected then decrement performs carefully.
     * decrement possible if largest node is not placeholder.
     */
    if (node->is_placeholder())
    {
        if (node->right_->is_placeholder())
        {
            return node;
        }

        return node->right_;
    }

    if (node->left_ != nullptr)
    {
        // If node has left child then go to rightmost node of this child.
        node = node->left_;

        if (!node->is_placeholder())
        {
            while (node->right_ != nullptr)
            {
                node = node->right_;
            }
        }
    }
    else
    {
        pointer_type old = node;

        while (node->parent_ != nullptr)
        {
            node = node->parent_;

            if (node->right_ == old)
            {
                break;
            }

            old = node;
        }
    }

    return node;
}
