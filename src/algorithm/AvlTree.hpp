#pragma once

#include <cstdint>
#include "AvlTreeDeclaration.hpp"
#include "AvlTreeNode.hpp"
#include "AvlTreeHeader.hpp"
#include "AvlTreeHeader.hpp"
#include "AvlTreeIterator.hpp"

template<typename ValueType, class Comparator>
class AvlTree {
 public:
  using self = AvlTree;
  using size_type = AvlTreeHeader::size_type;
  using value_type = ValueType;
  using node_type = AvlTreeNode<value_type>;
  using node_pointer = node_type *;
  using pointer_type = node_type::Avl_Base_ptr;
  using iterator = AvlTreeIterator<value_type>;

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
	  auto ret = dfs(root_);
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
  void recursive_destroy(pointer_type node);
  node_pointer rotate_left(node_pointer node);

  node_pointer rotate_right(node_pointer node);

  /**
   * use then height(b) - height(L) == 2 and height(C) <= height(R)
   *
   * i.e this
   *                  a
   *              /       \
   *             L         b
   *                     /   \
   *                    C     R
   *
   * will converted to this
   *
   *                  b
   *              /       \
   *             a         R
   *           /   \
   *          L     C
   */
  node_pointer small_left_rotate(node_pointer node);

  /**
   * use then height(b) - height(L) == 2 and height(C) > height(R)
   * note: equivalent to small right rotate + small left rotate
   * i.e this
   *                  a
   *              /      \
   *             L        b
   *                    /   \
   *                   C     R
   *                 /   \
   *                M     N
   *
   * will converted to this
   *
   *                  c
   *              /       \
   *             a         b
   *           /  \      /   \
   *          L	   M    N     R
   */
  node_pointer big_left_rotate(node_pointer node);

  /**
   * use then height(b) - height(R) == 2 and height(C) <= height(L)
   * i.e this
   *                  a
   *             /        \
   *            b          R
   *          /   \
   *         L     C
   *
   * will converted to this
   *
   *                  b
   *             /        \
   *            L          a
   *                     /   \
   *                    C     R
   */
  node_pointer small_right_rotate(node_pointer node);

  /**
   * use then height(b) - height(R) == 2 and height(c) > height(L)
   * note: equivalent to small left rotate + small right rotate
   * i.e this
   *                  a
   *              /       \
   *             b         R
   *			 /   \
   *		    L     c
   *		        /   \
   *             M     N
   *
   * will converted to this
   *
   *                  c
   *              /       \
   *             b          a
   *           /   \      /   \
   *          L     M    N     R
   */
  node_pointer big_right_rotate(node_pointer node);

  node_pointer balance_node(node_pointer node);

  node_pointer balance_bottom_up(node_pointer node);

 private:
  AvlTreeHeader header_;
  node_pointer root_;
  Comparator compare_;
};

template<typename ValueType, class Comparator>
AvlTree<ValueType, Comparator>::AvlTree() : header_(), root_(nullptr)
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
	header_.move_data(other.header_);
	root_ = other.root_;

	other.header_.reset();
	other.root_ = nullptr;
}

template<typename ValueType, class Comparator>
auto AvlTree<ValueType, Comparator>::operator=(AvlTree &&other) noexcept -> self &
{
	if (this != &other)
	{
		header_.move_data(other.header_);
		root_ = other.root_;

		other.header_.reset();
		other.root_ = nullptr;
	}

	return *this;
}

template<typename ValueType, class Comparator>
auto AvlTree<ValueType, Comparator>::begin() const noexcept -> iterator
{
	return iterator(header_.header_.left_);
}

template<typename ValueType, class Comparator>
auto AvlTree<ValueType, Comparator>::end() const noexcept -> iterator
{
	return iterator(header_.header_.parent_);
}

template<typename ValueType, class Comparator>
auto AvlTree<ValueType, Comparator>::size() const -> size_type
{
	return header_.node_count_;
}

template<typename ValueType, class Comparator>
bool AvlTree<ValueType, Comparator>::empty() const
{
	return size() == 0;
}

template<typename ValueType, class Comparator>
void AvlTree<ValueType, Comparator>::clear()
{
	header_.reset();
	recursive_destroy(root_);
	root_ = nullptr;
}

template<typename ValueType, class Comparator>
void AvlTree<ValueType, Comparator>::recursive_destroy(pointer_type node)
{
	if (node == nullptr)
	{
		return;
	}

	if (is_placeholder(node->left_))
	{
		node->left_ = nullptr;
	}
	if (is_placeholder(node->right_))
	{
		node->right_ = nullptr;
	}

	if (node->left_ != nullptr)
	{
		recursive_destroy(node->left_);
	}
	if (node->right_ != nullptr)
	{
		recursive_destroy(node->right_);
	}

	// both leafs destroyed, so destroy root
	delete node;
}

template<typename ValueType, class Comparator>
auto AvlTree<ValueType, Comparator>::insert(const value_type &value) -> std::pair<iterator, bool>
{
	if (root_ == nullptr)
	{
		root_ = new node_type(value);
		header_.header_.left_ = root_;
		header_.header_.right_ = root_;
		root_->left_ = &header_.header_;
		root_->right_ = &header_.header_;
		header_.header_.parent_ = &header_.header_;

		header_.node_count_ = 1;
		return {begin(), true};
	}

	node_pointer needle = nullptr;
	node_pointer current = root_;

	while (needle == nullptr)
	{
		/*
		 * if value less than current node value
		 */
		if (compare_(value, current->value_))
		{
			if (is_placeholder(current->left_))
			{
				// we want to insert value smaller than smallest
				current->left_ = new node_type(value);
				current->left_->parent_ = current;
				needle = current->left();

				header_.header_.left_ = needle;
				header_.header_.left_->left_ = header_.header_.parent_;
			}
			else if (current->left() == nullptr)
			{
				current->left_ = new node_type(value);
				current->left_->parent_ = current;
				needle = current->left();
			}
			else
			{
				current = current->left();
			}
		}
		else if (compare_(current->value_, value))
		{
			if (is_placeholder(current->right_))
			{
				// we want to insert value greater than largest
				current->right_ = new node_type(value);
				current->right_->parent_ = current;
				needle = current->right();

				header_.header_.right_ = needle;
				header_.header_.right_->right_ = header_.header_.parent_;
			}
			else if (current->right_ == nullptr)
			{
				current->right_ = new node_type(value);
				current->right_->parent_ = current;
				needle = current->right();
			}
			else
			{
				current = current->right();
			}
		}
		else
		{
			// current node contains same value
			return {iterator(needle), false};
		}
	}

	++header_.node_count_;

	// update heights for all ancestors
	needle->iterative_height_update();

	// balance nodes start from parent
	balance_bottom_up(needle->parent());

	return {iterator(needle), true};
}

template<typename ValueType, class Comparator>
auto AvlTree<ValueType, Comparator>::insert(value_type &&value) -> std::pair<iterator, bool>
{
	return insert(static_cast<ValueType &>(value));
}

template<typename ValueType, class Comparator>
void AvlTree<ValueType, Comparator>::erase_smallest()
{
	//	if (header_.node_count_ == 0)
	//	{
	//		return;
	//	}
	//	if (header_.node_count_ == 1)
	//	{
	//		clear();
	//		return;
	//	}
	//
	//	node_pointer node = begin().node_;
	//
	//	/*
	//	 * root is smallest value
	//	 */
	//	if (node == root_)
	//	{
	//		root_ = root_->right();
	//		delete root_->parent_;
	//		root_->parent_ = nullptr;
	//
	//		header_.header_.left_ = root_;
	//		header_.header_.left_->left_ = header_.header_.parent_;
	//	}
	//	else
	//	{
	//		/*
	//		 * leaf node
	//		 */
	//		if (node->right_ == nullptr)
	//		{
	//			node = node->parent();
	//			delete node->left_;
	//
	//			header_.header_.left_ = node;
	//			header_.header_.left_->left_ = header_.header_.parent_;
	//			node->iterative_height_update();
	//		}
	//		else
	//		{
	//			node_pointer temp = node->right();
	//			node = node->parent();
	//			delete node->left_;
	//			node->left_ = temp;
	//			temp->parent_ = node;
	//
	//			header_.header_.left_ = temp->successor();
	//			header_.header_.left_->left_ = header_.header_.parent_;
	//			node->iterative_height_update();
	//		}
	//
	//		// balance tree
	//		node = balance_bottom_up(node);
	//	}
	//	--header_.node_count_;
}

template<typename ValueType, class Comparator>
void AvlTree<ValueType, Comparator>::erase_largest()
{
	//	if (header_.node_count_ == 0)
	//	{
	//		return;
	//	}
	//	if (header_.node_count_ == 1)
	//	{
	//		clear();
	//		return;
	//	}
	//
	//	node_pointer node = std::prev(end()).node_;
	//
	//	/*
	//	 * root is smallest value
	//	 */
	//	if (node == root_)
	//	{
	//		root_ = root_->left();
	//		delete root_->parent_;
	//		root_->parent_ = nullptr;
	//
	//		header_.header_.right_ = root_;
	//		header_.header_.right_->right_ = header_.header_.parent_;
	//	}
	//	else
	//	{
	//		/*
	//		 * leaf node
	//		 */
	//		if (node->left_ == nullptr)
	//		{
	//			node = node->parent();
	//			delete node->right_;
	//			node->right_ = nullptr;
	//
	//			header_.header_.right_ = node;
	//			header_.header_.right_->right_ = header_.header_.parent_;
	//			node->iterative_height_update();
	//		}
	//		else
	//		{
	//			node_pointer temp = node->left();
	//			node = node->parent();
	//			delete node->right_;
	//			node->right_ = temp;
	//			temp->parent_ = node;
	//
	//			header_.header_.right_ = temp->predecessor();
	//			header_.header_.right_->right_ = header_.header_.parent_;
	//			temp->iterative_height_update();
	//		}
	//
	//		// balance tree
	//		balance_bottom_up(node);
	//	}
	//	--header_.node_count_;
}

template<typename ValueType, class Comparator>
void AvlTree<ValueType, Comparator>::erase(const value_type &value)
{
	iterator pos = find(value);
	erase(pos);
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
	/*
	 * if only one value remains then clear tree
	 */
	if (header_.node_count_ == 1)
	{
		clear();
		return;
	}

	header_.header_.left_->left_ = nullptr;
	header_.header_.right_->right_ = nullptr;

	node_pointer node = pos.node_;

	/*
	 * if node has two children then replace value with its successor and delete successor instead
	 */
	if (node->left_ != nullptr && node->right_ != nullptr)
	{
		node_pointer successor = static_cast<node_pointer>(node->successor());
		node->value_ = std::move(successor->value_);
		node = successor;
	}

	node_pointer parent = node->parent();

	if (parent == nullptr)
	{
		root_ = root_->left() != nullptr ? root_->left() : root_->right();

		delete root_->parent_;
		root_->parent_ = nullptr;
		parent = root_;
	}
	else
	{
		node_pointer child = node->right_ == nullptr ? node->left() : node->right();

		if (child != nullptr)
		{
			if (parent->left() == node)
			{
				parent->left_ = child;
			}
			else
			{
				parent->right_ = child;
			}

			child->parent_ = parent;
			parent->iterative_height_update();
			delete node;
		}
		else
		{
			if (parent->left() == node)
			{
				delete parent->left_;
				parent->left_ = nullptr;
			}
			else
			{
				delete parent->right_;
				parent->right_ = nullptr;
			}

			parent->iterative_height_update();
		}
	}

	parent = balance_bottom_up(parent);
	--header_.node_count_;

	header_.header_.left_->left_ = nullptr;
	header_.header_.right_->right_ = nullptr;

	/*
	 * restore minimum and maximum
	 */

	node_pointer smallest = root_;

	while (smallest->left_ != nullptr)
	{
		smallest = smallest->left();
	}

	node_pointer largest = root_;

	while (largest->right_ != nullptr)
	{
		largest = largest->right();
	}

	header_.header_.left_ = smallest;
	header_.header_.left_->left_ = header_.header_.parent_;

	header_.header_.right_ = largest;
	header_.header_.right_->right_ = header_.header_.parent_;
}

template<typename ValueType, class Comparator>
auto AvlTree<ValueType, Comparator>::find(const value_type &value) const -> iterator
{
	node_pointer node = root_;

	header_.header_.left_->left_ = nullptr;
	header_.header_.right_->right_ = nullptr;

	while (node != nullptr && node->value_ != value)
	{
		if (compare_(value, node->value_))
		{
			node = node->left();
		}
		else
		{
			node = node->right();
		}
	}

	header_.header_.left_->left_ = header_.header_.parent_;
	header_.header_.right_->right_ = header_.header_.parent_;

	if (node == nullptr)
	{
		return end();
	}
	return iterator(node);
}

template<typename ValueType, class Comparator>
auto AvlTree<ValueType, Comparator>::find(value_type &&value) const -> iterator
{
	return find(static_cast<value_type &>(value));
}

template<typename ValueType, class Comparator>
auto AvlTree<ValueType, Comparator>::rotate_left(node_pointer node) -> node_pointer
{
	node_pointer new_root = node->right();

	node->right_ = new_root->left_;

	if (node->right_ != nullptr)
	{
		node->right_->parent_ = node;
	}

	new_root->left_ = node;
	new_root->left_->parent_ = new_root;

	new_root->parent_ = nullptr;

	node->iterative_height_update();
	new_root->iterative_height_update();

	return new_root;
}

template<typename ValueType, class Comparator>
auto AvlTree<ValueType, Comparator>::rotate_right(node_pointer node) -> node_pointer
{
	node_pointer new_root = node->left();

	node->left_ = new_root->right_;

	if (node->left_ != nullptr)
	{
		node->left_->parent_ = node;
	}

	new_root->right_ = node;
	new_root->right_->parent_ = new_root;

	new_root->parent_ = nullptr;

	node->iterative_height_update();
	new_root->iterative_height_update();

	return new_root;
}

template<typename ValueType, class Comparator>
auto AvlTree<ValueType, Comparator>::small_left_rotate(node_pointer node) -> node_pointer
{
	if (node->parent_ == nullptr)
	{
		return rotate_left(node);
	}

	node_pointer parent = node->parent();
	node->parent_ = nullptr;

	if (parent->left() == node)
	{
		parent->left_ = rotate_left(node);
		parent->left_->parent_ = parent;
		parent->iterative_height_update();

		return parent->left();
	}
	else
	{
		parent->right_ = rotate_left(node);
		parent->right_->parent_ = parent;
		parent->iterative_height_update();

		return parent->right();
	}
}

template<typename ValueType, class Comparator>
auto AvlTree<ValueType, Comparator>::big_left_rotate(node_pointer node) -> node_pointer
{
	node->right_ = small_right_rotate(node->right());
	node->right_->parent_ = node;
	node->iterative_height_update();

	return small_left_rotate(node);
}

template<typename ValueType, class Comparator>
auto AvlTree<ValueType, Comparator>::small_right_rotate(node_pointer node) -> node_pointer
{
	if (node->parent_ == nullptr)
	{
		return rotate_right(node);
	}

	node_pointer parent = node->parent();
	node->parent_ = nullptr;

	if (parent->left() == node)
	{
		parent->left_ = rotate_right(node);
		parent->left_->parent_ = parent;
		parent->iterative_height_update();

		return parent->left();
	}
	else
	{
		parent->right_ = rotate_right(node);
		parent->right_->parent_ = parent;
		parent->iterative_height_update();

		return parent->right();
	}
}

template<typename ValueType, class Comparator>
auto AvlTree<ValueType, Comparator>::big_right_rotate(node_pointer node) -> node_pointer
{
	node->left_ = small_left_rotate(node->left());
	node->left_->parent_ = node;
	node->iterative_height_update();

	return small_right_rotate(node);
}

template<typename ValueType, class Comparator>
auto AvlTree<ValueType, Comparator>::balance_node(node_pointer node) -> node_pointer
{
	if (node == nullptr)
	{
		return node;
	}

	auto balance_factor = node->balance_factor();

	if (std::abs(balance_factor) > 1)
	{
		/*
	 	 * break relations with placeholder to prevent bad consequences.
	 	 * since rotating does not affect to node value (only relations)
	 	 * then left and right nodes will
	 	 */
		header_.header_.left_->left_ = nullptr;
		header_.header_.right_->right_ = nullptr;

		if (balance_factor > 1)
		{
			if (node->right_->balance_factor() >= 0)
			{
				node = small_left_rotate(node);
			}
			else
			{
				node = big_left_rotate(node);
			}
		}
		else if (balance_factor < -1)
		{
			if (node->left_->balance_factor() <= 0)
			{
				node = small_right_rotate(node);
			}
			else
			{
				node = big_right_rotate(node);
			}
		}

		/*
	 	 * restore relations back
	 	 */
		header_.header_.left_->left_ = header_.header_.parent_;
		header_.header_.right_->right_ = header_.header_.parent_;
	}

	return node;
}

template<typename ValueType, class Comparator>
auto AvlTree<ValueType, Comparator>::balance_bottom_up(node_pointer node) -> node_pointer
{
	node_pointer temp = node;

	while (temp != nullptr)
	{
		temp = balance_node(temp);

		if (temp->parent_ == nullptr)
		{
			root_ = temp;
		}
		temp = temp->parent();
	}

	return node;
}


//template<typename Value, class Compare = std::less<Value>>
//class AvlTree {
// private:
//
//  // forward declaration
//  template<typename ValueType>
//  class __Avl_Tree_iterator;
//
// public:
//
//  using size_type = std::size_t;
//  using difference_type = std::ptrdiff_t;
//  using value_type = Value;
//  using reference = value_type &;
//  using const_reference = const value_type &;
//  using value_compare = Compare;
//  using node_type = Node<value_type>;
//  using node_pointer = node_type::ptr_type;
//  using node_reference = node_type &;
//  using iterator = __Avl_Tree_iterator<value_type>;
//
// public:
//
//  AvlTree(const AvlTree &other) = default;
//  AvlTree &operator=(const AvlTree &other) = default;
//
//  AvlTree(AvlTree &&other) noexcept;
//  AvlTree &operator=(AvlTree &&other) noexcept;
//
//  AvlTree();
//  ~AvlTree();
//
//  /**
//   * iterators
//   */
//  iterator begin() const;
//  iterator end() const;
//
//  /**
//   * useful methods
//   */
//  iterator insert(const_reference value);
//  void erase(const_reference value);
//  void erase(iterator pos);
//  [[nodiscard]] iterator find(const_reference value) const;
//  [[nodiscard]] size_type size() const;
//  [[nodiscard]] bool empty() const;
//
//  void clear();
//
// private:
//
//  // insertion stuff
//  node_pointer rotate_left(node_pointer node);
//  node_pointer rotate_right(node_pointer node);
//  node_pointer small_left_rotate(node_pointer node);
//  node_pointer big_left_rotate(node_pointer node);
//  node_pointer small_right_rotate(node_pointer node);
//  node_pointer big_right_rotate(node_pointer node);
//
//  node_pointer balance_node(node_pointer node);
//  void balance_bottom_up(node_pointer node);
//
//  // clear stuff
//  void destroy(node_pointer &node);
//  void delete_node(node_pointer node);
//
//  // private attributes
//  node_pointer root_;
//
//  // to perform begin() in O(1)
//  node_pointer smallest_;
//
//  // to perform end() in O(1)
//  node_pointer greatest_;
//
//  // number of nodes in tree
//  size_type size_;
//
//  Compare compare_;
//
// private:
//
//  template<typename ValueType>
//  class __Avl_Tree_iterator {
//   private:
//
//	friend class AvlTree;
//
//   public:
//
//	using iterator_category = std::bidirectional_iterator_tag;
//	using difference_type = std::ptrdiff_t;
//	using value_type = ValueType;
//	using const_value_type = const value_type;
//	using pointer = value_type *;
//	using reference = value_type &;
//
//   private:
//
//	using node_type = Node<value_type>;
//	using node_pointer = node_type *;
//	using const_node_pointer = const node_pointer;
//	using node_reference = node_type &;
//	using const_node_reference = const node_type &;
//
//   public:
//
//	explicit __Avl_Tree_iterator(node_pointer node = nullptr);
//	__Avl_Tree_iterator(const __Avl_Tree_iterator &other);
//
//	/**
//	 * reference operator
//	 */
//
//	reference operator*() const;
//
//	/**
//	 * pointer operator
//	 */
//	pointer operator->() const;
//
//	/**
//	 * prefix increment
//	 */
//	__Avl_Tree_iterator &operator++();
//
//	/**
//	 * postfix increment
//	 */
//	__Avl_Tree_iterator operator++(int);
//
//	/**
//	 * prefix decrement
//	 */
//	__Avl_Tree_iterator &operator--();
//
//	// Postfix decrement
//	__Avl_Tree_iterator operator--(int);
//
//	bool operator==(const __Avl_Tree_iterator &other) const;
//	bool operator!=(const __Avl_Tree_iterator &other) const;
//
//   private:
//
//	node_pointer node_;
//  };
//};
//
//template<typename Value, class Compare>
//auto AvlTree<Value, Compare>::begin() const -> iterator
//{
//	return iterator(smallest_);
//}
//
//template<typename Value, class Compare>
//auto AvlTree<Value, Compare>::end() const -> iterator
//{
//	return iterator(nullptr);
//}
//
//template<typename Value, class Compare>
//AvlTree<Value, Compare>::AvlTree(AvlTree &&other) noexcept : root_(other.root_),
//															 smallest_(other.smallest_),
//															 greatest_(greatest_),
//															 size_(other.size_)
//{
//	other.root_ = nullptr;
//	other.smallest_ = nullptr;
//	other.greatest_ = nullptr;
//	other.size_ = 0;
//}
//
//template<typename Value, class Compare>
//AvlTree<Value, Compare> &AvlTree<Value, Compare>::operator=(AvlTree &&other) noexcept
//{
//	root_ = other.root_;
//	smallest_ = other.smallest_;
//	greatest_ = other.greatest_;
//	size_ = other.size_;
//
//	other.root_ = nullptr;
//	other.smallest_ = nullptr;
//	other.greatest_ = nullptr;
//	other.size_ = 0;
//
//	return *this;
//}
//
//template<typename Value, class Compare>
//AvlTree<Value, Compare>::AvlTree() : root_(nullptr), smallest_(0), greatest_(0), size_(0)
//{
//}
//
//template<typename Value, class Compare>
//AvlTree<Value, Compare>::~AvlTree()
//{
//	clear();
//}
//
//template<typename Value, class Compare>
//void AvlTree<Value, Compare>::clear()
//{
//	destroy(root_);
//	size_ = 0;
//}
//
//template<typename Value, class Compare>
//void AvlTree<Value, Compare>::destroy(node_pointer &node)
//{
//	if (node == nullptr)
//	{
//		return;
//	}
//
//	if (node->left != nullptr)
//	{
//		destroy(node->left);
//	}
//	if (node->right != nullptr)
//	{
//		destroy(node->right);
//	}
//
//	// both leafs destroyed, so destroy root
//	delete node;
//	node = nullptr;
//}
//
//template<typename Value, class Compare>
//auto AvlTree<Value, Compare>::rotate_left(AvlTree::node_pointer node) -> node_pointer
//{
//	node_pointer new_root = node->right;
//	node->update_right(new_root->left);
//	new_root->left = nullptr;
//	new_root->update_left(node);
//	return new_root;
//}
//
//template<typename Value, class Compare>
//auto AvlTree<Value, Compare>::rotate_right(AvlTree::node_pointer node) -> node_pointer
//{
//	node_pointer new_root = node->left;
//	node->update_left(new_root->right);
//	new_root->right = nullptr;
//	new_root->update_right(node);
//	return new_root;
//}
//
///**
// * use then height(b) - height(L) == 2 and height(C) <= height(R)
// *
// * i.e this
// *                  a
// *              /       \
// *             L         b
// *					   /   \
// *					  C     R
// *
// * will converted to this
// *
// *                  b
// *              /       \
// *             a         R
// *			 /	 \
// *			L	  C
// */
//template<typename Value, class Compare>
//auto AvlTree<Value, Compare>::small_left_rotate(node_pointer node) -> node_pointer
//{
//	if (node->parent == nullptr)
//	{
//		return rotate_left(node);
//	}
//
//	node_pointer parent = node->parent;
//	node_pointer &part = parent->left == node ? parent->left : parent->right;
//
//	node->unlink_parent();
//	part = rotate_left(node);
//	part->link_parent(parent);
//
//	return part;
//}
//
///**
// * use then height(b) - height(L) == 2 and height(C) > height(R)
// * note: equivalent to small right rotate + small left rotate
// * i.e. this
// *                  a
// *              /       \
// *             L         b
// *					   /   \
// *					  C     R
// *                  /  \
// *                M     N
// *
// * will be converted to this
// *
// *                  c
// *              /       \
// *             a          b
// *			 /	 \      /   \
// *			L	  M    N     R
// */
//template<typename Value, class Compare>
//auto AvlTree<Value, Compare>::big_left_rotate(node_pointer node) -> node_pointer
//{
//	node->update_right(small_right_rotate(node->right));
//	return small_left_rotate(node);
//}
//
///**
// * use then height(b) - height(R) == 2 and height(C) <= height(L)
// * i.e this
// *                  a
// *              /       \
// *             b         R
// *			 /   \
// *		    L     C
// *
// * will be converted to this
// *
// *                  b
// *              /       \
// *             L          a
// *			            /   \
// *			           C     R
// */
//template<typename Value, class Compare>
//auto AvlTree<Value, Compare>::small_right_rotate(node_pointer node) -> node_pointer
//{
//	if (node->parent == nullptr)
//	{
//		return rotate_right(node);
//	}
//
//	node_pointer parent = node->parent;
//	node_pointer &part = parent->left == node ? parent->left : parent->right;
//
//	node->unlink_parent();
//	part = rotate_right(node);
//	part->link_parent(parent);
//
//	return part;
//}
//
///**
// * use then height(b) - height(R) == 2 and height(c) > height(L)
// * note: equivalent to small left rotate + small right rotate
// * i.e this
// *                  a
// *              /       \
// *             b         R
// *			 /   \
// *		    L     c
// *		        /   \
// *             M     N
// *
// * will converted to this
// *
// *                  c
// *              /       \
// *             b          a
// *           /   \      /   \
// *			L     M    N     R
// */
//template<typename Value, class Compare>
//auto AvlTree<Value, Compare>::big_right_rotate(node_pointer node) -> node_pointer
//{
//	node->update_left(small_left_rotate(node->left));
//	return small_right_rotate(node);
//}
//
//template<typename Value, class Compare>
//auto AvlTree<Value, Compare>::balance_node(node_pointer node) -> node_pointer
//{
//	if (node == nullptr)
//	{
//		return node;
//	}
//
//	auto balance_factor = node->balance_factor();
//
//	if (std::abs(balance_factor) >= 2)
//	{
//		if (balance_factor > 1)
//		{
//			if (node->right->balance_factor() >= 0)
//			{
//				node = small_left_rotate(node);
//			}
//			else
//			{
//				node = big_left_rotate(node);
//			}
//		}
//		else if (balance_factor < -1)
//		{
//			if (node->left->balance_factor() <= 0)
//			{
//				node = small_right_rotate(node);
//			}
//			else
//			{
//				node = big_right_rotate(node);
//			}
//		}
//	}
//
//	return node;
//}
//
//template<typename Value, class Compare>
//void AvlTree<Value, Compare>::balance_bottom_up(AvlTree::node_pointer node)
//{
//	node_pointer temp = node;
//
//	while (temp != nullptr)
//	{
//		temp = balance_node(temp);
//
//		if (temp->parent == nullptr)
//		{
//			root_ = temp;
//		}
//		temp = temp->parent;
//	}
//}
//
//template<typename Value, class Compare>
//auto AvlTree<Value, Compare>::insert(const_reference value) -> iterator
//{
//	if (root_ == nullptr)
//	{
//		root_ = new node_type(value);
//		smallest_ = root_;
//		size_ = 1;
//		return begin();
//	}
//
//	node_pointer needle = nullptr;
//	node_pointer current = root_;
//
//	bool found = false;
//
//	while (needle == nullptr)
//	{
//		if (compare_(value, current->value))
//		{
//			if (current->left == nullptr)
//			{
//				current->update_left(new node_type(value));
//				needle = current->left;
//			}
//			else
//			{
//				current = current->left;
//			}
//		}
//		else if (compare_(current->value, value))
//		{
//			if (current->right == nullptr)
//			{
//				current->update_right(new node_type(value));
//				needle = current->right;
//			}
//			else
//			{
//				current = current->right;
//			}
//		}
//		else
//		{
//			// current node contains same value
//			needle = current;
//			found = true;
//		}
//	}
//
//	if (!found)
//	{
//		++size_;
//
//		if (compare_(needle->value, smallest_->value))
//		{
//			smallest_ = needle;
//		}
//
//		balance_bottom_up(needle->parent);
//	}
//
//	return iterator(needle);
//}
//
//template<typename Key, class Compare>
//auto AvlTree<Key, Compare>::size() const -> size_type
//{
//	return size_;
//}
//
//template<typename Key, class Compare>
//auto AvlTree<Key, Compare>::empty() const -> bool
//{
//	return size() == 0;
//}
//
//template<typename Value, class Compare>
//auto AvlTree<Value, Compare>::find(const_reference value) const -> iterator
//{
//	node_pointer node = root_;
//
//	while (node != nullptr && node->value != value)
//	{
//		if (compare_(value, node->value))
//		{
//			node = node->left;
//		}
//		else
//		{
//			node = node->right;
//		}
//	}
//
//	return iterator(node);
//}
//
//template<typename Value, class Compare>
//void AvlTree<Value, Compare>::delete_node(AvlTree::node_pointer node)
//{
//	if (node == nullptr)
//	{
//		return;
//	}
//
//	// node with two children
//	if (node->left != nullptr && node->right != nullptr)
//	{
//		/*
//		 * find left most value (smallest) in right subtree
//		 */
//		node_pointer successor = node->right;
//
//		while (successor->left != nullptr)
//		{
//			successor = successor->left;
//		}
//
//		// swap node value with successor value
//		node->value = successor->value;
//		node = successor;
//	}
//
//	node_pointer parent = node->parent;
//	node_pointer child = (node->left != nullptr) ? node->left : node->right;
//
//	// node with one child or no children
//	if (parent == nullptr)
//	{
//		root_ = child;
//	}
//	else if (node == parent->left)
//	{
//		parent->update_left(child);
//	}
//	else
//	{
//		parent->update_right(child);
//	}
//
//	delete node;
//	--size_;
//
//	// balance tree
//	balance_bottom_up(parent);
//}
//
//template<typename Value, class Compare>
//void AvlTree<Value, Compare>::erase(iterator pos)
//{
//	node_pointer needle = pos.node_;
//	value_type value = *pos;
//
//	if (needle != nullptr)
//	{
//		delete_node(needle);
//	}
//
//	// update smallest node
//	if (compare_(value, smallest_->value))
//	{
//		node_pointer node = root_;
//
//		while (node != nullptr && node->left != nullptr)
//		{
//			node = node->left;
//		}
//
//		smallest_ = node;
//	}
//}
//
//template<typename Key, class Compare>
//void AvlTree<Key, Compare>::erase(const_reference value)
//{
//	erase(find(value));
//}
//
///*
// * iterator implementation
// */
//
//template<typename Value, class Compare>
//template<typename ValueType>
//AvlTree<Value, Compare>::__Avl_Tree_iterator<ValueType>::__Avl_Tree_iterator(node_pointer node) : node_(node)
//{
//}
//
//template<typename Value, class Compare>
//template<typename ValueType>
//AvlTree<Value, Compare>::__Avl_Tree_iterator<ValueType>::__Avl_Tree_iterator(const __Avl_Tree_iterator &other)
//{
//	node_ = other.node_;
//}
//
//template<typename Value, class Compare>
//template<typename ValueType>
//auto AvlTree<Value, Compare>::__Avl_Tree_iterator<ValueType>::operator*() const -> reference
//{
//	return node_->value;
//}
//
//template<typename Value, class Compare>
//template<typename ValueType>
//auto AvlTree<Value, Compare>::__Avl_Tree_iterator<ValueType>::operator->() const -> pointer
//{
//	return &(node_->value);
//}
//
//template<typename Value, class Compare>
//template<typename ValueType>
//auto AvlTree<Value, Compare>::__Avl_Tree_iterator<ValueType>::operator++() -> __Avl_Tree_iterator &
//{
//	/*if (node_ == nullptr)
//	  {
//	  	node_ = smallest_;
//		return *this;
//	  }*/
//	if (node_->right)
//	{
//		/*
//		 * if node has right child then go to
//		 * leftmost node of this child
//		 */
//
//		node_ = node_->right;
//
//		while (node_->left)
//		{
//			node_ = node_->left;
//		}
//	}
//	else
//	{
//		/*
//		 * we have reached leaf node,
//		 * so we want to get out
//		 */
//		node_pointer temp;
//		bool stopped = false;
//
//		while (node_->parent != nullptr)
//		{
//			temp = node_;
//			node_ = node_->parent;
//
//			if (node_->left == temp)
//			{
//				stopped = true;
//				break;
//			}
//		}
//		if (!stopped)
//		{
//			if (node_->parent == nullptr)
//			{
//				node_ = nullptr;
//			}
//		}
//	}
//	return *this;
//}
//
//template<typename Value, class Compare>
//template<typename ValueType>
//auto AvlTree<Value, Compare>::__Avl_Tree_iterator<ValueType>::operator++(int) -> __Avl_Tree_iterator
//{
//	__Avl_Tree_iterator temp = *this;
//	++(*this);
//	return temp;
//}
//
//template<typename Value, class Compare>
//template<typename ValueType>
//auto AvlTree<Value, Compare>::__Avl_Tree_iterator<ValueType>::operator--() -> __Avl_Tree_iterator &
//{
//	/*if (node_ == nullptr)
//		{
//			node_ = greatest_;
//			return *this;
//		}*/
//	if (node_->left)
//	{
//		/*
//		 * similar to increment but mirrored
//		 */
//		node_ = node_->left;
//
//		while (node_->right)
//		{
//			node_ = node_->right;
//		}
//	}
//	else
//	{
//		node_pointer temp;
//		bool stopped = false;
//
//		while (node_->parent != nullptr)
//		{
//			temp = node_;
//			node_ = node_->parent;
//
//			if (node_->right == temp)
//			{
//				stopped = true;
//				break;
//			}
//		}
//
//		if (!stopped)
//		{
//			if (node_->parent == nullptr)
//			{
//				node_ = nullptr;
//			}
//		}
//	}
//	return *this;
//}
//
//template<typename Value, class Compare>
//template<typename ValueType>
//auto AvlTree<Value, Compare>::__Avl_Tree_iterator<ValueType>::operator--(int) -> __Avl_Tree_iterator
//{
//	__Avl_Tree_iterator temp = *this;
//	--(*this);
//	return temp;
//}
//
//template<typename Value, class Compare>
//template<typename ValueType>
//bool AvlTree<Value, Compare>::__Avl_Tree_iterator<ValueType>::operator==(const __Avl_Tree_iterator &other) const
//{
//	return node_ == other.node_;
//}
//
//template<typename Value, class Compare>
//template<typename ValueType>
//bool AvlTree<Value, Compare>::__Avl_Tree_iterator<ValueType>::operator!=(const __Avl_Tree_iterator &other) const
//{
//	return node_ != other.node_;
//}