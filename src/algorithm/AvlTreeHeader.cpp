#include "AvlTreeHeader.hpp"
#include "AvlTreeHelper.hpp"

AvlTreeHeader::AvlTreeHeader() : base_()
{
	base_.parent_ = &base_;
	base_.left_ = base_.parent_;
	base_.right_ = base_.parent_;
	base_.height_ = 0;
}

AvlTreeHeader::AvlTreeHeader(AvlTreeHeader &&other) noexcept
{
	move_data(other);
}

void AvlTreeHeader::move_data(AvlTreeHeader &other)
{
	reset();

	base_.left_ = other.base_.left_;
	base_.right_ = other.base_.right_;
	base_.parent_ = &base_;
	base_.height_ = other.base_.height_;

	other.reset();
}

void AvlTreeHeader::reset()
{
	force_unlink();

	base_.parent_ = &base_;
	base_.left_ = base_.parent_;
	base_.right_ = base_.parent_;
	base_.height_ = 0;
}

void AvlTreeHeader::force_unlink() const
{
	if (!is_placeholder(base_.left_))
	{
		base_.left_->left_ = nullptr;
	}
	if (!is_placeholder(base_.right_))
	{
		base_.right_->right_ = nullptr;
	}
}

void AvlTreeHeader::restore_links() const
{
	if (!is_placeholder(base_.left_) || base_.left_ == nullptr)
	{
		base_.left_->left_ = base_.parent_;
	}
	if (!is_placeholder(base_.right_) || base_.right_ == nullptr)
	{
		base_.right_->right_ = base_.parent_;
	}
}

void AvlTreeHeader::update_left(AvlTreeNodeBase::Avl_Base_ptr new_ptr)
{
	if (new_ptr == nullptr)
	{
		base_.left_ = base_.parent_;
	}

	base_.left_ = new_ptr;
}

void AvlTreeHeader::update_right(AvlTreeNodeBase::Avl_Base_ptr new_ptr)
{
	if (new_ptr == nullptr)
	{
		base_.right_ = base_.parent_;
	}

	base_.right_ = new_ptr;
}