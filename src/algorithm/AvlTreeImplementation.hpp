#pragma once

#include <valarray>
#include "AvlTreeHelper.hpp"

template<typename ValueType, class Comparator = std::less<ValueType>>
class AvlTreeBase {
 public:
  using self = AvlTreeBase;
  using size_type = std::size_t;
  using value_type = ValueType;
  using node_type = AvlTreeNode<value_type>;
  using node_pointer = node_type *;
  using base_pointer_type = node_type::Avl_Base_ptr;

  AvlTreeBase();

  ~AvlTreeBase();

  AvlTreeBase(AvlTreeBase &&other) noexcept;

  self &operator=(AvlTreeBase &&other) noexcept;

  // TODO implement copy constructor
  AvlTreeBase(const AvlTreeBase &other) = delete;
  self &operator=(const AvlTreeBase &other) = delete;

  /*
   * capacity
   */
  [[nodiscard]] size_type size() const;
  [[nodiscard]] bool empty() const;

  /*
   * modifiers
   */
  void clear();

  /*
   * this function swap nodes completely
   * i.e. they will be physically swap in tree
   */
  void swap_nodes(node_pointer a, node_pointer b);

  void insert(node_pointer node);
  node_pointer insert(const value_type &value);
  void erase(node_pointer node);
  void erase(const value_type &value);

  node_pointer find(const value_type &value) const;

  void recursive_destroy(base_pointer_type node);

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

  void rebalance(node_pointer node);

 public:
  node_pointer root_;
  size_type number_of_nodes_;
  Comparator compare_;
};

template<typename ValueType, class Comparator>
void AvlTreeBase<ValueType, Comparator>::swap_nodes(node_pointer a, node_pointer b)
{
	// swap the pointers
	std::swap(a->parent_, b->parent_);
	std::swap(a->left_, b->left_);
	std::swap(a->right_, b->right_);

	// swap the heights
	std::swap(a->height_, b->height_);

	/*
	 * edge case when one node is parent of another
	 */
	if (a->parent_ == a)
	{
		a->parent_ = b;

		if (b->right_ == b)
		{
			b->right_ = a;
		}
		else
		{
			b->left_ = a;
		}
	}
	else if (b->parent_ == b)
	{
		b->parent_ = a;

		if (a->right_ == a)
		{
			a->right_ = b;
		}
		else
		{
			a->left_ = b;
		}
	}

	// update the left and right pointers of the parent nodes
	if (a->parent_ != nullptr)
	{
		if (a->parent_->left_ == b)
		{
			a->parent_->left_ = a;
		}
		else
		{
			a->parent_->right_ = a;
		}
	}
	if (b->parent_ != nullptr)
	{
		if (b->parent_->left_ == a)
		{
			b->parent_->left_ = b;
		}
		else
		{
			b->parent_->right_ = b;
		}
	}

	// update the parent pointers of the child nodes
	if (a->left_ != nullptr)
	{
		a->left_->parent_ = a;
	}
	if (a->right_ != nullptr)
	{
		a->right_->parent_ = a;
	}
	if (b->left_ != nullptr)
	{
		b->left_->parent_ = b;
	}
	if (b->right_ != nullptr)
	{
		b->right_->parent_ = b;
	}
}

template<typename ValueType, class Comparator>
AvlTreeBase<ValueType, Comparator>::AvlTreeBase() : root_(nullptr), number_of_nodes_(0)
{
}

template<typename ValueType, class Comparator>
AvlTreeBase<ValueType, Comparator>::~AvlTreeBase()
{
	clear();
}

template<typename ValueType, class Comparator>
AvlTreeBase<ValueType, Comparator>::AvlTreeBase(AvlTreeBase &&other) noexcept
{
	root_ = other.root_;
	number_of_nodes_ = other.number_of_nodes_;
	other.root_ = nullptr;
	other.number_of_nodes_ = 0;
}

template<typename ValueType, class Comparator>
auto AvlTreeBase<ValueType, Comparator>::operator=(AvlTreeBase &&other) noexcept -> self &
{
	if (this != &other)
	{
		root_ = other.root_;
		number_of_nodes_ = other.number_of_nodes_;
		other.root_ = nullptr;
		other.number_of_nodes_ = 0;
	}

	return *this;
}

template<typename ValueType, class Comparator>
auto AvlTreeBase<ValueType, Comparator>::size() const -> size_type
{
	return number_of_nodes_;
}

template<typename ValueType, class Comparator>
bool AvlTreeBase<ValueType, Comparator>::empty() const
{
	return size() == 0;
}

template<typename ValueType, class Comparator>
void AvlTreeBase<ValueType, Comparator>::insert(node_pointer node)
{
	if (root_ == nullptr)
	{
		root_ = node;
		++number_of_nodes_;
		return;
	}

	node_pointer current = root_;
	bool inserted = false;

	while (!inserted)
	{
		/*
		 * if value less than current node value
		 */
		if (compare_(node->value_, current->value_))
		{
			if (current->left_ == nullptr)
			{
				current->left_ = node;
				current->left_->parent_ = current;
				inserted = true;
			}
			else
			{
				current = current->left();
			}
		}
		else
		{
			if (current->right_ == nullptr)
			{
				current->right_ = node;
				current->right_->parent_ = current;
				inserted = true;
			}
			else
			{
				current = current->right();
			}
		}
	}

	++number_of_nodes_;

	// update heights for all ancestors
	node->iterative_height_update();

	// balance nodes start from parent
	rebalance(node->parent());
}

template<typename ValueType, class Comparator>
auto AvlTreeBase<ValueType, Comparator>::insert(const value_type &value) -> node_pointer
{
	auto old = find(value);

	if (old != nullptr)
	{
		return old;
	}

	return insert(new node_type(value));
}

template<typename ValueType, class Comparator>
void AvlTreeBase<ValueType, Comparator>::erase(node_pointer node)
{
	if (node == nullptr)
	{
		return;
	}

	/*
	 * if node has two children, then swap value with successor and delete him instead.
	 */
	if (node->left_ != nullptr && node->right_ != nullptr)
	{
		/*auto successor = static_cast<node_pointer>(node->successor());
		node->value_ = std::move(successor->value_);
		node = successor;*/
		auto successor = static_cast<node_pointer>(node->successor());
		swap_nodes(node, successor);

	}

	node_pointer parent = node->parent();

	if (parent == nullptr)
	{
		root_ = root_->left() != nullptr ? root_->left() : root_->right();

		delete root_->parent();
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
			delete node;
		}
		else
		{
			if (parent->left() == node)
			{
				delete parent->left();
				parent->left_ = nullptr;
			}
			else
			{
				delete parent->right();
				parent->right_ = nullptr;
			}
		}

		parent->iterative_height_update();
	}

	rebalance(parent);
	--number_of_nodes_;
}

template<typename ValueType, class Comparator>
void AvlTreeBase<ValueType, Comparator>::erase(const value_type &value)
{
	node_pointer node = find(value);
	erase(node);
}

template<typename ValueType, class Comparator>
auto AvlTreeBase<ValueType, Comparator>::find(const value_type &value) const -> node_pointer
{
	node_pointer node = root_;

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

	return node;
}

template<typename ValueType, class Comparator>
void AvlTreeBase<ValueType, Comparator>::clear()
{
	recursive_destroy(root_);
	root_ = nullptr;
	number_of_nodes_ = 0;
}

template<typename ValueType, class Comparator>
void AvlTreeBase<ValueType, Comparator>::recursive_destroy(base_pointer_type node)
{
	if (node == nullptr)
	{
		return;
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
auto AvlTreeBase<ValueType, Comparator>::rotate_left(node_pointer node) -> node_pointer
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
auto AvlTreeBase<ValueType, Comparator>::rotate_right(node_pointer node) -> node_pointer
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
auto AvlTreeBase<ValueType, Comparator>::small_left_rotate(node_pointer node) -> node_pointer
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
auto AvlTreeBase<ValueType, Comparator>::big_left_rotate(node_pointer node) -> node_pointer
{
	node->right_ = small_right_rotate(node->right());
	node->right_->parent_ = node;
	node->iterative_height_update();

	return small_left_rotate(node);
}

template<typename ValueType, class Comparator>
auto AvlTreeBase<ValueType, Comparator>::small_right_rotate(node_pointer node) -> node_pointer
{
	if (node->parent_ == nullptr)
	{
		return rotate_right(node);
	}

	node_pointer parent = node->parent();
	node->parent_ = nullptr;

	// TODO rewrite this part to reduce code garbage
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
auto AvlTreeBase<ValueType, Comparator>::big_right_rotate(node_pointer node) -> node_pointer
{
	node->left_ = small_left_rotate(node->left());
	node->left_->parent_ = node;
	node->iterative_height_update();

	return small_right_rotate(node);
}

template<typename ValueType, class Comparator>
auto AvlTreeBase<ValueType, Comparator>::balance_node(node_pointer node) -> node_pointer
{
	if (node == nullptr)
	{
		return node;
	}

	auto balance_factor = node->balance_factor();

	if (std::abs(balance_factor) > 1)
	{
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
	}

	return node;
}

template<typename ValueType, class Comparator>
void AvlTreeBase<ValueType, Comparator>::rebalance(node_pointer node)
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
}

