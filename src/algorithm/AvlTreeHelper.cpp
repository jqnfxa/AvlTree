#include "AvlTreeHelper.hpp"

bool is_placeholder(AvlTreeNodeBase::Avl_Const_Base_ptr node)
{
	if (node == nullptr)
	{
		return false;
	}

	if (node->parent_ == nullptr)
	{
		return false;
	}

	return node->parent_->to_constant() == node;
}