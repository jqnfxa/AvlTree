#pragma once

#include <cstdint>
#include <cassert>
#include "AvlTreeImplementation.hpp"
#include "AvlTreeIterator.hpp"

template<typename ValueType, class Comparator = std::less<ValueType>>
class AvlTree {
 public:
  using self = AvlTree;
  using value_type = ValueType;
  using BaseImpl = AvlTreeBase<ValueType, Comparator>;
  using size_type = typename BaseImpl::size_type;
  using node_type = typename BaseImpl::node_type;
  using node_pointer = typename BaseImpl::node_pointer;
  using iterator = AvlTreeIterator<value_type>;
  using reverse_iterator = std::reverse_iterator<iterator>;

  int dfs(node_pointer root)
  {
	  if (root == nullptr || root->is_placeholder())
	  {
		  return 0;
	  }

	  int left = dfs(root->left_);
	  assert(root->left_height() == left);

	  if (left == -1)
	  {
		  return -1;
	  }

	  int right = dfs(root->right_);
	  assert(root->right_height() == right);

	  if (right == -1)
	  {
		  return -1;
	  }

	  return abs(left - right) > 1 ? -1 : 1 + std::max(left, right);
  }
  bool force_balance_check()
  {
	  auto ret = dfs(base_.root_);
	  return ret != -1;
  }

  AvlTree();

  ~AvlTree();

  AvlTree(AvlTree &&other) noexcept;

  self &operator=(AvlTree &&other) noexcept;

  // TODO implement copy constructor
  AvlTree(const AvlTree &other) = delete;
  self &operator=(const AvlTree &other) = delete;

  /*
   * iterators
   */
  iterator begin() const noexcept;
  iterator end() const noexcept;

  reverse_iterator rbegin() const noexcept;
  reverse_iterator rend() const noexcept;

  /*
   * capacity
   */
  [[nodiscard]] size_type size() const;

  [[nodiscard]] bool empty() const;

  /*
   * modifiers
   */
  void clear();

  std::pair<iterator, bool> insert(const value_type &value);
  std::pair<iterator, bool> insert(value_type &&value);
  void erase_smallest();
  void erase_largest();
  void erase(const value_type &value);
  void erase(const iterator &pos);

  /*
   * searchers
   */
  iterator find(const value_type &value) const;
  iterator find(value_type &&value) const;

 public:
  BaseImpl base_;
  AvlTreeNode<value_type> header_;
};

template<typename ValueType, class Comparator>
auto AvlTree<ValueType, Comparator>::rbegin() const noexcept -> reverse_iterator
{
	return reverse_iterator(end());
}

template<typename ValueType, class Comparator>
auto AvlTree<ValueType, Comparator>::rend() const noexcept -> reverse_iterator
{
	return reverse_iterator(begin());
}

template<typename ValueType, class Comparator>
AvlTree<ValueType, Comparator>::AvlTree() : base_(), header_(0xffffff)
{
	header_.reset();
}

template<typename ValueType, class Comparator>
AvlTree<ValueType, Comparator>::~AvlTree()
{
	clear();
}

template<typename ValueType, class Comparator>
AvlTree<ValueType, Comparator>::AvlTree(AvlTree &&other) noexcept
{
	base_ = std::move(other.base_);
	header_ = std::move(other.header_);
}

template<typename ValueType, class Comparator>
auto AvlTree<ValueType, Comparator>::operator=(AvlTree &&other) noexcept -> self &
{
	if (this != &other)
	{
		base_ = std::move(other.base_);
		header_ = std::move(other.header_);
	}

	return *this;
}

template<typename ValueType, class Comparator>
auto AvlTree<ValueType, Comparator>::begin() const noexcept -> iterator
{
	return iterator(header_.left_);
}

template<typename ValueType, class Comparator>
auto AvlTree<ValueType, Comparator>::end() const noexcept -> iterator
{
	return iterator(header_.parent_);
}

template<typename ValueType, class Comparator>
auto AvlTree<ValueType, Comparator>::size() const -> size_type
{
	return base_.size();
}

template<typename ValueType, class Comparator>
bool AvlTree<ValueType, Comparator>::empty() const
{
	return base_.empty();
}

template<typename ValueType, class Comparator>
void AvlTree<ValueType, Comparator>::clear()
{
	header_.unlink_placeholder();
	header_.reset();
	base_.clear();
}

template<typename ValueType, class Comparator>
auto AvlTree<ValueType, Comparator>::insert(const value_type &value) -> std::pair<iterator, bool>
{
	/*
	 * value already exists
	 */
	if (auto pos = find(value); pos != end())
	{
		return {pos, false};
	}

	header_.unlink_placeholder();

	node_pointer node = new node_type(value);

	if (empty())
	{
		base_.root_ = node;
		header_.left_ = base_.root_;
		header_.right_ = base_.root_;
		++base_.number_of_nodes_;
	}
	else
	{
		const auto &smallest = header_.left_;
		const auto &greatest = header_.right_;

		/*
		 * we want to update the smallest value or greatest values, then update header
		 */
		if (base_.compare_(node->value_, smallest->value_))
		{
			header_.left_ = node;
		}
		else if (base_.compare_(greatest->value_, node->value_))
		{
			header_.right_ = node;
		}

		base_.insert(node);
	}

	header_.restore_placeholder();

	return {iterator(node), true};
}

template<typename ValueType, class Comparator>
auto AvlTree<ValueType, Comparator>::insert(value_type &&value) -> std::pair<iterator, bool>
{
	return insert(static_cast<ValueType &>(value));
}

template<typename ValueType, class Comparator>
void AvlTree<ValueType, Comparator>::erase_smallest()
{
	erase(iterator(header_.left_));
}

template<typename ValueType, class Comparator>
void AvlTree<ValueType, Comparator>::erase_largest()
{
	erase(iterator(header_.right_));
}

template<typename ValueType, class Comparator>
void AvlTree<ValueType, Comparator>::erase(const value_type &value)
{
	erase(find(value));
}

template<typename ValueType, class Comparator>
void AvlTree<ValueType, Comparator>::erase(const iterator &pos)
{
	/*
	 * cannot erase placeholder
	 */
	if (pos == end())
	{
		return;
	}
	if (size() == 1)
	{
		clear();
		return;
	}

	header_.unlink_placeholder();

	/*
	 * we want to update the smallest value or greatest values, then update header
	 */
	if (pos == begin())
	{
		header_.left_ = std::next(pos).node_;
	}
	else if (pos == std::prev(end()))
	{
		header_.right_ = std::prev(pos).node_;
	}

	base_.erase(pos.node_);

	header_.restore_placeholder();
}

template<typename ValueType, class Comparator>
auto AvlTree<ValueType, Comparator>::find(const value_type &value) const -> iterator
{
	header_.unlink_placeholder();

	iterator ret = std::move(end());
	if (node_pointer node = base_.find(value); node != nullptr)
	{
		ret = std::move(iterator(node));
	}

	header_.restore_placeholder();

	return ret;
}

template<typename ValueType, class Comparator>
auto AvlTree<ValueType, Comparator>::find(value_type &&value) const -> iterator
{
	return find(static_cast<value_type &>(value));
}