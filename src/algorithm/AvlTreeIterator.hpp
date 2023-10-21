#pragma once

#include <iterator>
#include "AvlTreeHelper.hpp"
#include "AvlTreeNode.hpp"

template <typename ValueType>
class AvlTreeIterator {
 public:
  template<typename T, class C> friend
  class AvlTree;

 public:
  using iterator_tag = std::bidirectional_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using value_type = ValueType;
  using pointer_type = typename AvlTreeNode<value_type>::pointer_type;
  using self = AvlTreeIterator<value_type>;
  using reference = const value_type &;
  using pointer = const value_type *;

  AvlTreeIterator();
  explicit AvlTreeIterator(const pointer_type &node);

  reference operator*();

  pointer operator->();

  /**
   * @brief prefix increment
   */
  self &operator++();

  /**
   * @brief postfix increment
   */
  self operator++(int);

  /**
   * @brief prefix decrement
   */
  self &operator--();

  /**
   * @brief prefix decrement
   */
  self operator--(int);

  bool operator!=(const AvlTreeIterator<value_type> &other) const;
  bool operator==(const AvlTreeIterator<value_type> &other) const;

 private:
  pointer_type node_;
};

template <typename ValueType>
AvlTreeIterator<ValueType>::AvlTreeIterator() : node_(nullptr)
{
}

template <typename ValueType>
AvlTreeIterator<ValueType>::AvlTreeIterator(const pointer_type &node) : node_(node)
{
}

template <typename ValueType>
auto AvlTreeIterator<ValueType>::operator*() -> reference
{
	return node_->value_;
}

template <typename ValueType>
auto AvlTreeIterator<ValueType>::operator->() -> pointer
{
	return &node_->value_;
}

template <typename ValueType>
auto AvlTreeIterator<ValueType>::operator++() -> self &
{
	node_ = avl_tree_increment<ValueType>(node_);
	return *this;
}

template <typename ValueType>
auto AvlTreeIterator<ValueType>::operator++(int) -> self
{
	self temp = *this;
	++(*this);
	return temp;
}

template <typename ValueType>
auto AvlTreeIterator<ValueType>::operator--() -> self &
{
	node_ = avl_tree_decrement<ValueType>(node_);
	return *this;
}

template <typename ValueType>
auto AvlTreeIterator<ValueType>::operator--(int) -> self
{
	self temp = *this;
	--(*this);
	return temp;
}

template <typename ValueType>
bool AvlTreeIterator<ValueType>::operator!=(const AvlTreeIterator<value_type> &other) const
{
	return node_ != other.node_;
}

template <typename ValueType>
bool AvlTreeIterator<ValueType>::operator==(const AvlTreeIterator<value_type> &other) const
{
	return node_ == other.node_;
}
