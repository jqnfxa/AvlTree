#include <algorithm>
#include "AvlTreeNode.hpp"

AvlTreeNodeBase::AvlTreeNodeBase(AvlTreeNodeBase::Avl_Base_ptr left, AvlTreeNodeBase::Avl_Base_ptr right, AvlTreeNodeBase::Avl_Base_ptr parent, AvlTreeNodeBase::difference_type height) : parent_(parent),
																																														   left_(left),
																																														   right_(right),
																																														   height_(height)
{
	iterative_height_update();
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
	Avl_Base_ptr node = this;

	difference_type pre = -1, post = -2;

	while (node != nullptr && pre != post)
	{
		pre = node->height_;
		update_height_standalone();
		post = node->height_;

		node = node->parent_;
	}
}

void AvlTreeNodeBase::update_height_standalone()
{
	height_ = 1 + std::max(left_height(), right_height());
}