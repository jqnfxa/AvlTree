#pragma once

#include <cstddef>
#include <algorithm>

template<typename ValueType>
class AvlTreeNode {
 public:
  using value_type = ValueType;
  using const_reference = const value_type &;
  using difference_type = std::ptrdiff_t;
  using pointer_type = AvlTreeNode<value_type> *;

  AvlTreeNode() = default;
  explicit AvlTreeNode(const_reference val,
					   const pointer_type &left = nullptr,
					   const pointer_type &right = nullptr,
					   const pointer_type &parent = nullptr);
  AvlTreeNode &operator=(AvlTreeNode &&other) noexcept;

  [[nodiscard]] pointer_type predecessor() const;
  [[nodiscard]] pointer_type successor() const;
  [[nodiscard]] difference_type left_height() const;
  [[nodiscard]] difference_type right_height() const;
  [[nodiscard]] difference_type balance_factor() const;
  [[nodiscard]] bool is_placeholder() const;

  void iterative_height_update();
  void update_height_standalone();

  /*
   * placeholder related functions
   */
  void reset();
  void unlink_placeholder() const;
  void restore_placeholder() const;

 public:
  pointer_type left_;
  pointer_type right_;
  pointer_type parent_;

  value_type value_;
  difference_type height_{};
};

template<typename ValueType>
AvlTreeNode<ValueType>::AvlTreeNode(const_reference val,
									const pointer_type &left,
									const pointer_type &right,
									const pointer_type &parent)
									: left_(left),
									right_(right),
									parent_(parent),
									value_(val),
									height_(1)
{
	iterative_height_update();
}

template<typename ValueType>
auto AvlTreeNode<ValueType>::operator=(AvlTreeNode &&other) noexcept -> AvlTreeNode &
{
	reset();

	parent_ = &this;
	left_ = std::move(other.base_.left_);
	right_ = std::move(other.base_.right_);
	value_ = std::move(other.value_);
	height_ = std::move(other.base_.height_);

	other.reset();
}

template<typename ValueType>
auto AvlTreeNode<ValueType>::predecessor() const -> pointer_type
{
	if (left_ == nullptr)
	{
		return const_cast<pointer_type>(this);
	}

	pointer_type predecessor = left_;

	while (predecessor != nullptr && predecessor->right_ != nullptr)
	{
		predecessor = predecessor->right_;
	}

	return predecessor;
}

template<typename ValueType>
auto AvlTreeNode<ValueType>::successor() const -> pointer_type
{
	if (right_ == nullptr)
	{
		return const_cast<pointer_type>(this);
	}

	pointer_type successor = right_;

	while (successor != nullptr && successor->left_ != nullptr)
	{
		successor = successor->left_;
	}

	return successor;
}

template<typename ValueType>
auto AvlTreeNode<ValueType>::left_height() const -> difference_type
{
	return left_ == nullptr ? 0 : left_->height_;
}

template<typename ValueType>
auto AvlTreeNode<ValueType>::right_height() const -> difference_type
{
	return right_ == nullptr ? 0 : right_->height_;
}

template<typename ValueType>
auto AvlTreeNode<ValueType>::balance_factor() const -> difference_type
{
	return right_height() - left_height();
}

template<typename ValueType>
void AvlTreeNode<ValueType>::iterative_height_update()
{
	// update self height first
	update_height_standalone();

	pointer_type node = parent_;

	difference_type pre = -1, post = -2;

	while (node != nullptr && pre != post)
	{
		pre = node->height_;
		node->update_height_standalone();
		post = node->height_;

		node = node->parent_;
	}
}

template<typename ValueType>
void AvlTreeNode<ValueType>::update_height_standalone()
{
	height_ = 1 + std::max(left_height(), right_height());
}

template<typename ValueType>
bool AvlTreeNode<ValueType>::is_placeholder() const
{
	return parent_ == this;
}

template<typename ValueType>
void AvlTreeNode<ValueType>::reset()
{
	parent_ = this;
	left_ = parent_;
	right_ = parent_;
	height_ = 0;
}

template<typename ValueType>
void AvlTreeNode<ValueType>::unlink_placeholder() const
{
	if (!left_->is_placeholder())
	{
		left_->left_ = nullptr;
	}
	if (!right_->is_placeholder())
	{
		right_->right_ = nullptr;
	}
}

template<typename ValueType>
void AvlTreeNode<ValueType>::restore_placeholder() const
{
	left_->left_ = parent_;
	right_->right_ = parent_;
}