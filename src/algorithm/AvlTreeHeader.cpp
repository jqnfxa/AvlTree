#include "AvlTreeHeader.hpp"

AvlTreeHeader::AvlTreeHeader() : header_(), node_count_(0)
{
	reset();
}

AvlTreeHeader::AvlTreeHeader(AvlTreeHeader &&other) noexcept
{
	if (other.header_.parent_ == nullptr)
	{
		reset();
		node_count_ = 0;
	}
	else
	{
		move_data(other);
	}
}

void AvlTreeHeader::move_data(AvlTreeHeader &other)
{
	reset();

	header_.left_ = other.header_.left_;
	header_.right_ = other.header_.right_;
	header_.parent_ = &header_;
	header_.height_ = other.header_.height_;
	node_count_ = other.node_count_;

	other.reset();
	other.node_count_ = 0;
}

void AvlTreeHeader::reset()
{
	header_.parent_ = &header_;
	header_.left_ = &header_;
	header_.right_ = &header_;
	node_count_ = 0;
	header_.height_ = 0;
}