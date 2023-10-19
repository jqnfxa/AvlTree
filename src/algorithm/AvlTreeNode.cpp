#include <algorithm>
#include "AvlTreeNode.hpp"

AvlTreeNodeBase::AvlTreeNodeBase(AvlTreeNodeBase::Avl_Base_ptr left, AvlTreeNodeBase::Avl_Base_ptr right, AvlTreeNodeBase::Avl_Base_ptr parent, AvlTreeNodeBase::difference_type height) : parent_(parent),
																																														   left_(left),
																																														   right_(right),
																																														   height_(height)
{
	iterative_height_update();
}

AvlTreeNodeBase *AvlTreeNodeBase::predecessor() const
{
	if (left_ == nullptr)
	{
		return const_cast<AvlTreeNodeBase *>(this);
	}
	Avl_Base_ptr predecessor = left_;

	while (predecessor != nullptr && predecessor->right_ != nullptr)
	{
		predecessor = predecessor->right_;
	}

	return predecessor;
}

AvlTreeNodeBase *AvlTreeNodeBase::successor() const
{
	if (right_ == nullptr)
	{
		return const_cast<AvlTreeNodeBase *>(this);
	}

	Avl_Base_ptr successor = right_;

	while (successor != nullptr && successor->left_ != nullptr)
	{
		successor = successor->left_;
	}

	return successor;
}

AvlTreeNodeBase::difference_type AvlTreeNodeBase::left_height() const
{
	return left_ == nullptr ? 0 : left_->height_;
}

AvlTreeNodeBase::difference_type AvlTreeNodeBase::right_height() const
{
	return right_ == nullptr ? 0 : right_->height_;
}

AvlTreeNodeBase::difference_type AvlTreeNodeBase::balance_factor() const
{
	return right_height() - left_height();
}

auto AvlTreeNodeBase::AvlTreeNodeBase::to_constant() const -> Avl_Const_Base_ptr
{
	return static_cast<Avl_Const_Base_ptr>(this);
}

void AvlTreeNodeBase::iterative_height_update()
{
	// update our height first
	update_height_standalone();

	Avl_Base_ptr node = parent_;

	difference_type pre = -1, post = -2;

	while (node != nullptr && pre != post)
	{
		pre = node->height_;
		node->update_height_standalone();
		post = node->height_;

		node = node->parent_;
	}
}

void AvlTreeNodeBase::update_height_standalone()
{
	height_ = 1 + std::max(left_height(), right_height());
}
