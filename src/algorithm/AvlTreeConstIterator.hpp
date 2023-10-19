#pragma once

#include "AvlTreeIterator.hpp"

template<typename ValueType>
class AvlTreeConstIterator {
 public:
  template<typename T, class C> friend
  class AvlTree;
 public:
  using value_type = ValueType;
  using iterator = AvlTreeIterator<value_type>;
  using self = AvlTreeConstIterator<value_type>;
  using iterator_tag = std::bidirectional_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using reference = const value_type &;
  using pointer = const value_type *;

  AvlTreeConstIterator();

  explicit AvlTreeConstIterator(AvlTreeNodeBase::Avl_Const_Base_ptr node);

  explicit AvlTreeConstIterator(const iterator &other);

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

  bool operator==(const AvlTreeConstIterator<value_type> &other) const;
 private:
  AvlTreeNode<value_type>::const_pointer_type node_;
};

template<typename ValueType>
AvlTreeConstIterator<ValueType>::AvlTreeConstIterator() : node_(nullptr)
{
}

template<typename ValueType>
AvlTreeConstIterator<ValueType>::AvlTreeConstIterator(AvlTreeNodeBase::Avl_Const_Base_ptr node) : node_(static_cast<AvlTreeNode<ValueType>::const_pointer_type>(node))
{
}

template<typename ValueType>
AvlTreeConstIterator<ValueType>::AvlTreeConstIterator(const AvlTreeConstIterator::iterator &other) : node_(other.node_)
{
}

template<typename ValueType>
auto AvlTreeConstIterator<ValueType>::operator*() -> reference
{
	return node_->value_;
}

template<typename ValueType>
auto AvlTreeConstIterator<ValueType>::operator->() -> pointer
{
	return node_->value();
}

template<typename ValueType>
auto AvlTreeConstIterator<ValueType>::operator++() -> self &
{
	node_ = avl_tree_increment<ValueType>(node_);
	return *this;
}

template<typename ValueType>
auto AvlTreeConstIterator<ValueType>::operator++(int) -> self
{
	self temp = *this;
	++(*this);
	return temp;
}

template<typename ValueType>
auto AvlTreeConstIterator<ValueType>::operator--() -> self &
{
	node_ = avl_tree_decrement<ValueType>(node_);
	return *this;
}

template<typename ValueType>
auto AvlTreeConstIterator<ValueType>::operator--(int) -> self
{
	self temp = *this;
	--(*this);
	return temp;
}

template<typename ValueType>
bool AvlTreeConstIterator<ValueType>::operator==(const AvlTreeConstIterator<value_type> &other) const
{
	return node_ == other.node_;
}