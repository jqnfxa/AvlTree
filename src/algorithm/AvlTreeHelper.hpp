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
    /*
     * No node to increment.
     */
    if (node == nullptr)
    {
        return nullptr;
    }

    /*
     * If placeholder node detected then increment does not perform.
     */
    if (node->is_placeholder())
    {
        return node;
    }

    if (node->right_ != nullptr)
    {
        /*
         * If node has right child then go to successor of this node.
         */
        node = node->successor();
    }
    else
    {
        /*
         * We've reached the leaf node, so we're coming out of it.
         */
        auto old = node;

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
    /*
     * No node to decrement.
     */
    if (node == nullptr)
    {
        return nullptr;
    }

    /*
     * If placeholder node detected then decrement performs carefully.
     * Decrement possible if largest node is not a placeholder.
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
        /*
         * If node has left child then go to predecessor of this node.
         */
        node = node->predecessor();
    }
    else
    {
        /*
         * We've reached the leaf node, so we're coming out of it.
         */
        auto old = node;

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
