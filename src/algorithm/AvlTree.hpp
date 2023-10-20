#pragma once

#include <cstdint>
#include "AvlTreeImplementation.hpp"
#include "AvlTreeDeclaration.hpp"
#include "AvlTreeHeader.hpp"
#include "AvlTreeIterator.hpp"

template<typename ValueType, class Comparator>
class AvlTree {
 public:
  using self = AvlTree;
  using BaseImpl = AvlTreeBase<ValueType, Comparator>;
  using size_type = BaseImpl::size_type;
  using value_type = ValueType;
  using node_type = BaseImpl::node_type;
  using node_pointer = BaseImpl::node_pointer;
  using iterator = AvlTreeIterator<value_type>;
  using reverse_iterator = std::reverse_iterator<iterator>;

  // TODO implement std::reverse_iterators

  int dfs(node_pointer root)
  {
	  if (is_placeholder(root))
	  {
		  return 0;
	  }
	  if (root == nullptr)
	  {
		  return 0;
	  }

	  int left = dfs(root->left());
	  assert(root->left_height() == left);

	  if (left == -1)
	  {
		  return -1;
	  }

	  int right = dfs(root->right());
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
  void erase(iterator pos);

  /*
   * searchers
   */
  iterator find(const value_type &value) const;
  iterator find(value_type &&value) const;

 private:
  BaseImpl base_;
  AvlTreeHeader header_;
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
AvlTree<ValueType, Comparator>::AvlTree() : base_(), header_()
{
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
	header_.move_data(other.header_);
}

template<typename ValueType, class Comparator>
auto AvlTree<ValueType, Comparator>::operator=(AvlTree &&other) noexcept -> self &
{
	if (this != &other)
	{
		base_ = std::move(other.base_);
		header_.move_data(other.header_);
	}

	return *this;
}

template<typename ValueType, class Comparator>
auto AvlTree<ValueType, Comparator>::begin() const noexcept -> iterator
{
	return iterator(header_.base_.left_);
}

template<typename ValueType, class Comparator>
auto AvlTree<ValueType, Comparator>::end() const noexcept -> iterator
{
	return iterator(header_.base_.parent_);
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
	header_.reset();
	base_.clear();
}

template<typename ValueType, class Comparator>
auto AvlTree<ValueType, Comparator>::insert(const value_type &value) -> std::pair<iterator, bool>
{
	auto pos = find(value);

	/*
	 * value already exists
	 */
	if (pos != end())
	{
		return {pos, false};
	}

	header_.force_unlink();

	node_pointer node = new node_type(value);

	if (empty())
	{
		base_.root_ = node;
		header_.update_left(base_.root_);
		header_.update_right(base_.root_);
		++base_.number_of_nodes_;
	}
	else
	{
		auto smallest = static_cast<node_pointer>(header_.base_.left_);
		auto greatest = static_cast<node_pointer>(header_.base_.right_);

		/*
		 * we want to update the smallest value or greatest values, then update header
		 */
		if (base_.compare_(node->value_, smallest->value_))
		{
			header_.update_left(node);
		}
		else if (base_.compare_(greatest->value_, node->value_))
		{
			header_.update_right(node);
		}

		base_.insert(node);
	}

	header_.restore_links();

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
	erase(begin());
}

template<typename ValueType, class Comparator>
void AvlTree<ValueType, Comparator>::erase_largest()
{
	erase(std::prev(end()));
}

template<typename ValueType, class Comparator>
void AvlTree<ValueType, Comparator>::erase(const value_type &value)
{
	erase(find(value));
}

template<typename ValueType, class Comparator>
void AvlTree<ValueType, Comparator>::erase(iterator pos)
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

	header_.force_unlink();

	bool force_left = false;
	bool force_right = false;
	/*
	 * we want to update the smallest value or greatest values, then update header
	 */
	if (pos == begin())
	{
		header_.update_left(std::next(pos).node_);
	}
	else if (pos == std::prev(end()))
	{
		header_.update_right(std::prev(pos).node_);
	}

	base_.erase(pos.node_);

	header_.restore_links();
}

template<typename ValueType, class Comparator>
auto AvlTree<ValueType, Comparator>::find(const value_type &value) const -> iterator
{
	header_.force_unlink();

	auto needle = base_.find(value);
	iterator ret = needle == nullptr ? end() : iterator(needle);

	header_.restore_links();

	return ret;
}

template<typename ValueType, class Comparator>
auto AvlTree<ValueType, Comparator>::find(value_type &&value) const -> iterator
{
	return find(static_cast<value_type &>(value));
}