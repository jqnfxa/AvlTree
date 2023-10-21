#pragma once

#include "AvlTreeNode.hpp"

/*
template<typename PointerType>
concept AvlTreeNodePointerType = std::is_same_v<typeid(PointerType), typeid(AvlTreeNodeBase::Avl_Base_ptr)>; // || std::is_same_v<PointerType, AvlTreeNodeBase::Avl_Const_Base_ptr>;
*/

/**
 * @brief Increments iterator in the AVL tree.
 * @tparam AvlTreeNodePointer A pointer to an AVL tree node.
 * @par This can be either a pointer to an `AvlTreeNodeBase` or a pointer to a `const AvlTreeNodeBase`.
 * @param node The node to increment.
 * @return A pointer to the next node in the AVL tree.
 */
template<typename ValueType>
auto avl_tree_increment(typename AvlTreeNode<ValueType>::pointer_type node) -> AvlTreeNode<ValueType>::pointer_type
{
	using pointer_type = typename AvlTreeNode<ValueType>::pointer_type;

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
		 * If node has right child then go to leftmost node of this child.
		 */
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
		/*
		 * We've reached the leaf node, so we're coming out of it
		 */
		pointer_type old = node;

		while (node->parent_ != nullptr)
		{
			node = node->parent_;

			if (old->value_ <= node->value_)
			{
				break;
			}
		}
	}

	return node;
}

/**
 * @brief Decrements iterator in the AVL tree.
 * @tparam AvlTreeNodePointer A pointer to an AVL tree node.
 * @par This can be either a pointer to an `AvlTreeNodeBase` or a pointer to a `const AvlTreeNodeBase`.
 * @param node The node to decrement.
 * @return A pointer to the next node in the AVL tree.
 */
template<typename ValueType>
auto avl_tree_decrement(typename AvlTreeNode<ValueType>::pointer_type node) -> AvlTreeNode<ValueType>::pointer_type
{
	using pointer_type = AvlTreeNode<ValueType>::pointer_type;

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
		/*
		 * If node has left child then go to rightmost node of this child.
		 */
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

			if (node->value_ <= old->value_)
			{
				break;
			}
		}
	}

	return node;
}
