#pragma once

#include "AvlTreeNode.hpp"

/*
template<typename PointerType>
concept AvlTreeNodePointerType = std::is_same_v<typeid(PointerType), typeid(AvlTreeNodeBase::Avl_Base_ptr)>; // || std::is_same_v<PointerType, AvlTreeNodeBase::Avl_Const_Base_ptr>;
*/
/**
 * @brief Checks if a node is a placeholder.
 * @par Note: Placeholder has following property: node->parent_->parent_ = node.
 * @param node The node to check.
 * @return `true` if the node is a placeholder, `false` otherwise.
 */
bool is_placeholder(AvlTreeNodeBase::Avl_Const_Base_ptr node);

/**
 * @brief Increments iterator in the AVL tree.
 * @tparam AvlTreeNodePointer A pointer to an AVL tree node.
 * @par This can be either a pointer to an `AvlTreeNodeBase` or a pointer to a `const AvlTreeNodeBase`.
 * @param node The node to increment.
 * @return A pointer to the next node in the AVL tree.
 */
template<typename ValueType>
auto avl_tree_increment(typename AvlTreeNode<ValueType>::const_pointer_type node) -> AvlTreeNode<ValueType>::const_pointer_type
{
	using pointer_type = typename AvlTreeNode<ValueType>::const_pointer_type;

	if (node == nullptr)
	{
		return nullptr;
	}

	/*
	 * If placeholder node detected then increment does not perform.
	 */
	if (is_placeholder(node))
	{
		return node;
	}

	if (node->right() != nullptr)
	{
		/*
		 * If node has right child then go to leftmost node of this child.
		 */
		node = node->right();

		if (!is_placeholder(node))
		{
			while (node->left() != nullptr)
			{
				node = node->left();
			}
		}
	}
	else
	{
		/*
		 * We've reached the leaf node, so we're coming out of it
		 */
		pointer_type temp;

		while (node->parent() != nullptr)
		{
			temp = node;
			node = node->parent();

			if (node->left()->to_constant() == temp)
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
auto avl_tree_decrement(typename AvlTreeNode<ValueType>::const_pointer_type node) -> AvlTreeNode<ValueType>::const_pointer_type
{
	using pointer_type = AvlTreeNode<ValueType>::const_pointer_type;

	if (node == nullptr)
	{
		return nullptr;
	}

	/*
	 * If placeholder node detected then decrement performs carefully.
	 * decrement possible if largest node is not placeholder.
	 */
	if (is_placeholder(node))
	{
		if (is_placeholder(node->right_))
		{
			return node;
		}
		return node->right();
	}

	if (node->left() != nullptr)
	{
		/*
		 * If node has left child then go to rightmost node of this child.
		 */
		node = node->left();

		if (!is_placeholder(node))
		{
			while (node->right() != nullptr)
			{
				node = node->right();
			}
		}
	}
	else
	{
		pointer_type temp;

		while (node->parent() != nullptr)
		{
			temp = node;
			node = node->parent();

			if (node->right()->to_constant() == temp)
			{
				break;
			}
		}
	}

	return node;
}
