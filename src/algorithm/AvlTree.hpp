#pragma once

#include <cstdint>
#include "Node.hpp"

template<typename Value, class Compare = std::less<Value>>
class AvlTree {
 private:

  // forward declaration
  template<typename ValueType>
  class __Avl_Tree_iterator;

 public:

  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using value_type = Value;
  using reference = value_type &;
  using const_reference = const value_type &;
  using value_compare = Compare;
  using node_type = Node<value_type>;
  using node_pointer = node_type::ptr_type;
  using node_reference = node_type &;
  using iterator = __Avl_Tree_iterator<value_type>;

 public:

  AvlTree(const AvlTree &other) = default;
  AvlTree &operator=(const AvlTree &other) = default;

  AvlTree(AvlTree &&other) noexcept;
  AvlTree &operator=(AvlTree &&other) noexcept;

  AvlTree();
  ~AvlTree();

  /**
   * iterators
   */
  iterator begin() const;
  iterator end() const;

  /**
   * useful methods
   */
  iterator insert(const_reference value);
  void erase(const_reference value);
  void erase(iterator pos);
  [[nodiscard]] iterator find(const_reference value) const;
  [[nodiscard]] size_type size() const;
  [[nodiscard]] bool empty() const;

  void clear();

 private:

  // insertion stuff
  node_pointer rotate_left(node_pointer node);
  node_pointer rotate_right(node_pointer node);
  node_pointer small_left_rotate(node_pointer node);
  node_pointer big_left_rotate(node_pointer node);
  node_pointer small_right_rotate(node_pointer node);
  node_pointer big_right_rotate(node_pointer node);

  node_pointer balance_node(node_pointer node);
  void balance_bottom_up(node_pointer node);

  // clear stuff
  void destroy(node_pointer &node);
  void delete_node(node_pointer node);

  // private attributes
  node_pointer root_;

  // to perform begin() in O(1)
  node_pointer smallest_;

  // to perform end() in O(1)
  node_pointer greatest_;

  // number of nodes in tree
  size_type size_;

  Compare compare_;

 private:

  template<typename ValueType>
  class __Avl_Tree_iterator {
   private:

	friend class AvlTree;

   public:

	using iterator_category = std::bidirectional_iterator_tag;
	using difference_type = std::ptrdiff_t;
	using value_type = ValueType;
	using const_value_type = const value_type;
	using pointer = value_type *;
	using reference = value_type &;

   private:

	using node_type = Node<value_type>;
	using node_pointer = node_type *;
	using const_node_pointer = const node_pointer;
	using node_reference = node_type &;
	using const_node_reference = const node_type &;

   public:

	explicit __Avl_Tree_iterator(node_pointer node = nullptr);
	__Avl_Tree_iterator(const __Avl_Tree_iterator &other);

	/**
	 * reference operator
	 */

	reference operator*() const;

	/**
	 * pointer operator
	 */
	pointer operator->() const;

	/**
	 * prefix increment
	 */
	__Avl_Tree_iterator &operator++();

	/**
	 * postfix increment
	 */
	__Avl_Tree_iterator operator++(int);

	/**
	 * prefix decrement
	 */
	__Avl_Tree_iterator &operator--();

	// Postfix decrement
	__Avl_Tree_iterator operator--(int);

	bool operator==(const __Avl_Tree_iterator &other) const;
	bool operator!=(const __Avl_Tree_iterator &other) const;

   private:

	node_pointer node_;
  };
};

template<typename Value, class Compare>
auto AvlTree<Value, Compare>::begin() const -> iterator
{
	return iterator(smallest_);
}

template<typename Value, class Compare>
auto AvlTree<Value, Compare>::end() const -> iterator
{
	return iterator(nullptr);
}

template<typename Value, class Compare>
AvlTree<Value, Compare>::AvlTree(AvlTree &&other) noexcept : root_(other.root_),
															 smallest_(other.smallest_),
															 greatest_(greatest_),
															 size_(other.size_)
{
	other.root_ = nullptr;
	other.smallest_ = nullptr;
	other.greatest_ = nullptr;
	other.size_ = 0;
}

template<typename Value, class Compare>
AvlTree<Value, Compare> &AvlTree<Value, Compare>::operator=(AvlTree &&other) noexcept
{
	root_ = other.root_;
	smallest_ = other.smallest_;
	greatest_ = other.greatest_;
	size_ = other.size_;

	other.root_ = nullptr;
	other.smallest_ = nullptr;
	other.greatest_ = nullptr;
	other.size_ = 0;

	return *this;
}

template<typename Value, class Compare>
AvlTree<Value, Compare>::AvlTree() : root_(nullptr), smallest_(0), greatest_(0), size_(0)
{
}

template<typename Value, class Compare>
AvlTree<Value, Compare>::~AvlTree()
{
	clear();
}

template<typename Value, class Compare>
void AvlTree<Value, Compare>::clear()
{
	destroy(root_);
	size_ = 0;
}

template<typename Value, class Compare>
void AvlTree<Value, Compare>::destroy(node_pointer &node)
{
	if (node == nullptr)
	{
		return;
	}

	if (node->left != nullptr)
	{
		destroy(node->left);
	}
	if (node->right != nullptr)
	{
		destroy(node->right);
	}

	// both leafs destroyed, so destroy root
	delete node;
	node = nullptr;
}

template<typename Value, class Compare>
auto AvlTree<Value, Compare>::rotate_left(AvlTree::node_pointer node) -> node_pointer
{
	node_pointer new_root = node->right;
	node->update_right(new_root->left);
	new_root->left = nullptr;
	new_root->update_left(node);
	return new_root;
}

template<typename Value, class Compare>
auto AvlTree<Value, Compare>::rotate_right(AvlTree::node_pointer node) -> node_pointer
{
	node_pointer new_root = node->left;
	node->update_left(new_root->right);
	new_root->right = nullptr;
	new_root->update_right(node);
	return new_root;
}

/**
 * use then height(b) - height(L) == 2 and height(C) <= height(R)
 *
 * i.e this
 *                  a
 *              /       \
 *             L         b
 *					   /   \
 *					  C     R
 *
 * will converted to this
 *
 *                  b
 *              /       \
 *             a         R
 *			 /	 \
 *			L	  C
 */
template<typename Value, class Compare>
auto AvlTree<Value, Compare>::small_left_rotate(node_pointer node) -> node_pointer
{
	if (node->parent == nullptr)
	{
		return rotate_left(node);
	}

	node_pointer parent = node->parent;
	node_pointer &part = parent->left == node ? parent->left : parent->right;

	node->unlink_parent();
	part = rotate_left(node);
	part->link_parent(parent);

	return part;
}

/**
 * use then height(b) - height(L) == 2 and height(C) > height(R)
 * note: equivalent to small right rotate + small left rotate
 * i.e this
 *                  a
 *              /       \
 *             L         b
 *					   /   \
 *					  C     R
 *                  /  \
 *                M     N
 *
 * will converted to this
 *
 *                  c
 *              /       \
 *             a          b
 *			 /	 \      /   \
 *			L	  M    N     R
 */
template<typename Value, class Compare>
auto AvlTree<Value, Compare>::big_left_rotate(node_pointer node) -> node_pointer
{
	node->update_right(small_right_rotate(node->right));
	return small_left_rotate(node);
}

/**
 * use then height(b) - height(R) == 2 and height(C) <= height(L)
 * i.e this
 *                  a
 *              /       \
 *             b         R
 *			 /   \
 *		    L     C
 *
 * will converted to this
 *
 *                  b
 *              /       \
 *             L          a
 *			            /   \
 *			           C     R
 */
template<typename Value, class Compare>
auto AvlTree<Value, Compare>::small_right_rotate(node_pointer node) -> node_pointer
{
	if (node->parent == nullptr)
	{
		return rotate_right(node);
	}

	node_pointer parent = node->parent;
	node_pointer &part = parent->left == node ? parent->left : parent->right;

	node->unlink_parent();
	part = rotate_right(node);
	part->link_parent(parent);

	return part;
}

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
 *			L     M    N     R
 */
template<typename Value, class Compare>
auto AvlTree<Value, Compare>::big_right_rotate(node_pointer node) -> node_pointer
{
	node->update_left(small_left_rotate(node->left));
	return small_right_rotate(node);
}

template<typename Value, class Compare>
auto AvlTree<Value, Compare>::balance_node(node_pointer node) -> node_pointer
{
	if (node == nullptr)
	{
		return node;
	}

	auto balance_factor = node->balance_factor();

	if (std::abs(balance_factor) >= 2)
	{
		if (balance_factor > 1)
		{
			if (node->right->balance_factor() >= 0)
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
			if (node->left->balance_factor() <= 0)
			{
				node = small_right_rotate(node);
			}
			else
			{
				node = big_right_rotate(node);
			}
		}
	}

	return node;
}

template<typename Value, class Compare>
void AvlTree<Value, Compare>::balance_bottom_up(AvlTree::node_pointer node)
{
	node_pointer temp = node;

	while (temp != nullptr)
	{
		temp = balance_node(temp);

		if (temp->parent == nullptr)
		{
			root_ = temp;
		}
		temp = temp->parent;
	}
}

template<typename Value, class Compare>
auto AvlTree<Value, Compare>::insert(const_reference value) -> iterator
{
	if (root_ == nullptr)
	{
		root_ = new node_type(value);
		smallest_ = root_;
		size_ = 1;
		return begin();
	}

	node_pointer needle = nullptr;
	node_pointer current = root_;

	bool found = false;

	while (needle == nullptr)
	{
		if (compare_(value, current->value))
		{
			if (current->left == nullptr)
			{
				current->update_left(new node_type(value));
				needle = current->left;
			}
			else
			{
				current = current->left;
			}
		}
		else if (compare_(current->value, value))
		{
			if (current->right == nullptr)
			{
				current->update_right(new node_type(value));
				needle = current->right;
			}
			else
			{
				current = current->right;
			}
		}
		else
		{
			// current node contains same value
			needle = current;
			found = true;
		}
	}

	if (!found)
	{
		++size_;

		if (compare_(needle->value, smallest_->value))
		{
			smallest_ = needle;
		}

		balance_bottom_up(needle->parent);
	}

	return iterator(needle);
}

template<typename Key, class Compare>
auto AvlTree<Key, Compare>::size() const -> size_type
{
	return size_;
}

template<typename Key, class Compare>
auto AvlTree<Key, Compare>::empty() const -> bool
{
	return size() == 0;
}

template<typename Value, class Compare>
auto AvlTree<Value, Compare>::find(const_reference value) const -> iterator
{
	node_pointer node = root_;

	while (node != nullptr && node->value != value)
	{
		if (compare_(value, node->value))
		{
			node = node->left;
		}
		else
		{
			node = node->right;
		}
	}

	return iterator(node);
}

template<typename Value, class Compare>
void AvlTree<Value, Compare>::delete_node(AvlTree::node_pointer node)
{
	if (node == nullptr)
	{
		return;
	}

	// node with two children
	if (node->left != nullptr && node->right != nullptr)
	{
		/*
		 * find left most value (smallest) in right subtree
		 */
		node_pointer successor = node->right;

		while (successor->left != nullptr)
		{
			successor = successor->left;
		}

		// swap node value with successor value
		node->value = successor->value;
		node = successor;
	}

	node_pointer parent = node->parent;
	node_pointer child = (node->left != nullptr) ? node->left : node->right;

	// node with one child or no children
	if (parent == nullptr)
	{
		root_ = child;
	}
	else if (node == parent->left)
	{
		parent->update_left(child);
	}
	else
	{
		parent->update_right(child);
	}

	delete node;
	--size_;

	// balance tree
	balance_bottom_up(parent);
}

template<typename Value, class Compare>
void AvlTree<Value, Compare>::erase(iterator pos)
{
	node_pointer needle = pos.node_;
	value_type value = *pos;

	if (needle != nullptr)
	{
		delete_node(needle);
	}

	// update smallest node
	if (compare_(value, smallest_->value))
	{
		node_pointer node = root_;

		while (node != nullptr && node->left != nullptr)
		{
			node = node->left;
		}

		smallest_ = node;
	}
}

template<typename Key, class Compare>
void AvlTree<Key, Compare>::erase(const_reference value)
{
	erase(find(value));
}

/*
 * iterator implementation
 */

template<typename Value, class Compare>
template<typename ValueType>
AvlTree<Value, Compare>::__Avl_Tree_iterator<ValueType>::__Avl_Tree_iterator(node_pointer node) : node_(node)
{
}

template<typename Value, class Compare>
template<typename ValueType>
AvlTree<Value, Compare>::__Avl_Tree_iterator<ValueType>::__Avl_Tree_iterator(const __Avl_Tree_iterator &other)
{
	node_ = other.node_;
}

template<typename Value, class Compare>
template<typename ValueType>
auto AvlTree<Value, Compare>::__Avl_Tree_iterator<ValueType>::operator*() const -> reference
{
	return node_->value;
}

template<typename Value, class Compare>
template<typename ValueType>
auto AvlTree<Value, Compare>::__Avl_Tree_iterator<ValueType>::operator->() const -> pointer
{
	return &(node_->value);
}

template<typename Value, class Compare>
template<typename ValueType>
auto AvlTree<Value, Compare>::__Avl_Tree_iterator<ValueType>::operator++() -> __Avl_Tree_iterator &
{
	/*if (node_ == nullptr)
	  {
	  	node_ = smallest_;
		return *this;
	  }*/
	if (node_->right)
	{
		/*
		 * if node has right child then go to
		 * leftmost node of this child
		 */

		node_ = node_->right;

		while (node_->left)
		{
			node_ = node_->left;
		}
	}
	else
	{
		/*
		 * we have reached leaf node,
		 * so we want to get out
		 */
		node_pointer temp;
		bool stopped = false;

		while (node_->parent != nullptr)
		{
			temp = node_;
			node_ = node_->parent;

			if (node_->left == temp)
			{
				stopped = true;
				break;
			}
		}
		if (!stopped)
		{
			if (node_->parent == nullptr)
			{
				node_ = nullptr;
			}
		}
	}
	return *this;
}

template<typename Value, class Compare>
template<typename ValueType>
auto AvlTree<Value, Compare>::__Avl_Tree_iterator<ValueType>::operator++(int) -> __Avl_Tree_iterator
{
	__Avl_Tree_iterator temp = *this;
	++(*this);
	return temp;
}

template<typename Value, class Compare>
template<typename ValueType>
auto AvlTree<Value, Compare>::__Avl_Tree_iterator<ValueType>::operator--() -> __Avl_Tree_iterator &
{
	/*if (node_ == nullptr)
		{
			node_ = greatest_;
			return *this;
		}*/
	if (node_->left)
	{
		/*
		 * similar to increment but mirrored
		 */
		node_ = node_->left;

		while (node_->right)
		{
			node_ = node_->right;
		}
	}
	else
	{
		node_pointer temp;
		bool stopped = false;

		while (node_->parent != nullptr)
		{
			temp = node_;
			node_ = node_->parent;

			if (node_->right == temp)
			{
				stopped = true;
				break;
			}
		}

		if (!stopped)
		{
			if (node_->parent == nullptr)
			{
				node_ = nullptr;
			}
		}
	}
	return *this;
}

template<typename Value, class Compare>
template<typename ValueType>
auto AvlTree<Value, Compare>::__Avl_Tree_iterator<ValueType>::operator--(int) -> __Avl_Tree_iterator
{
	__Avl_Tree_iterator temp = *this;
	--(*this);
	return temp;
}

template<typename Value, class Compare>
template<typename ValueType>
bool AvlTree<Value, Compare>::__Avl_Tree_iterator<ValueType>::operator==(const __Avl_Tree_iterator &other) const
{
	return node_ == other.node_;
}

template<typename Value, class Compare>
template<typename ValueType>
bool AvlTree<Value, Compare>::__Avl_Tree_iterator<ValueType>::operator!=(const __Avl_Tree_iterator &other) const
{
	return node_ != other.node_;
}