#pragma once

#include <iterator>
#include "AvlTreeDeclaration.hpp"
#include "AvlTreeHelper.hpp"
#include "AvlTreeNode.hpp"

template<typename ValueType>
class AvlTreeIterator {
 public:
  template<typename T, class C> friend
  class AvlTree;
 public:
  using value_type = ValueType;
  using self = AvlTreeIterator<value_type>;
  using iterator_tag = std::bidirectional_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using reference = const value_type &;
  using pointer = const value_type *;

  AvlTreeIterator();

  explicit AvlTreeIterator(AvlTreeNodeBase::Avl_Base_ptr node);

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

  bool operator==(const AvlTreeIterator<value_type> &other) const;
 private:
  AvlTreeNode<value_type>::pointer_type node_;
};

template<typename ValueType>
AvlTreeIterator<ValueType>::AvlTreeIterator() : node_(nullptr)
{
}

template<typename ValueType>
AvlTreeIterator<ValueType>::AvlTreeIterator(AvlTreeNodeBase::Avl_Base_ptr node) : node_(static_cast<AvlTreeNode<value_type>::pointer_type>(node))
{
}

template<typename ValueType>
auto AvlTreeIterator<ValueType>::operator*() -> reference
{
	return node_->value_;
}

template<typename ValueType>
auto AvlTreeIterator<ValueType>::operator->() -> pointer
{
	return node_->value();
}

template<typename ValueType>
auto AvlTreeIterator<ValueType>::operator++() -> self &
{
	node_ = const_cast<AvlTreeNode<ValueType>::pointer_type>(avl_tree_increment<ValueType>(node_->to_constant()));
	return *this;
}

template<typename ValueType>
auto AvlTreeIterator<ValueType>::operator++(int) -> self
{
	self temp = *this;
	++(*this);
	return temp;
}

template<typename ValueType>
auto AvlTreeIterator<ValueType>::operator--() -> self &
{
	node_ = const_cast<AvlTreeNode<ValueType>::pointer_type>(avl_tree_decrement<ValueType>(node_->to_constant()));
	return *this;
}

template<typename ValueType>
auto AvlTreeIterator<ValueType>::operator--(int) -> self
{
	self temp = *this;
	--(*this);
	return temp;
}

template<typename ValueType>
bool AvlTreeIterator<ValueType>::operator==(const AvlTreeIterator<value_type> &other) const
{
	return node_ == other.node_;
}
