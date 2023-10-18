#pragma once

#include <cstdint>
#include "Node.hpp"

/**
 * @class AvlTreeNodeBase
 * @brief Base class for AVL tree nodes.
 * @details This class provides a base for AVL tree nodes. It has pointers to the parent, left and right child nodes.
 */
class AvlTreeNodeBase {
 public:
  using difference_type = std::ptrdiff_t;
  using Avl_Base_ptr = AvlTreeNodeBase *;
  using Avl_Const_Base_ptr = const AvlTreeNodeBase *;

  explicit AvlTreeNodeBase(Avl_Base_ptr left = nullptr, Avl_Base_ptr right = nullptr, Avl_Base_ptr parent = nullptr, difference_type height = 1);

  [[nodiscard]] inline difference_type left_height() const;
  [[nodiscard]] inline difference_type right_height() const;
  [[nodiscard]] inline difference_type balance_factor() const;

  void inline iterative_height_update();
  void inline update_height_standalone();

 public:
  Avl_Base_ptr parent_;
  Avl_Base_ptr left_;
  Avl_Base_ptr right_;
  difference_type height_;
};

AvlTreeNodeBase::AvlTreeNodeBase(AvlTreeNodeBase::Avl_Base_ptr left, AvlTreeNodeBase::Avl_Base_ptr right, AvlTreeNodeBase::Avl_Base_ptr parent, AvlTreeNodeBase::difference_type height) : parent_(parent),
																																														   left_(left),
																																														   right_(right),
																																														   height_(height)
{
}

AvlTreeNodeBase::difference_type AvlTreeNodeBase::left_height() const
{
	return left_ == nullptr ? 0 : left_->height_;
}

AvlTreeNodeBase::difference_type AvlTreeNodeBase::right_height() const
{
	return right_ == nullptr ? 0 : right_->height_;
}

AvlTreeNodeBase::difference_type AvlTreeNodeBase::balance_factor() const
{
	return right_height() - left_height();
}

void AvlTreeNodeBase::iterative_height_update()
{
	Avl_Base_ptr node = this;

	difference_type pre = -1, post = -2;

	while (node != nullptr && pre != post)
	{
		pre = node->height_;
		update_height_standalone();
		post = node->height_;

		node = node->parent_;
	}
}

void AvlTreeNodeBase::update_height_standalone()
{
	height_ = 1 + std::max(left_height(), right_height());
}

/**
 * @class AvlTreeNode
 * @brief A node in an AVL tree.
 * @tparam Value_ The type of the value stored in the node.
 * @details This class represents a node in an AVL tree.
 * @par It inherits from `AvlTreeNodeBase` and adds a member for the value stored in the node.
 * @par It also provides methods for accessing the value stored in the node.
 */
template<typename ValueType>
class AvlTreeNode : public AvlTreeNodeBase {
 public:
  using value_type = ValueType;
  using difference_type = std::ptrdiff_t;
  using const_reference = const value_type &;
  using pointer_type = AvlTreeNode<value_type> *;
  using const_pointer_type = const AvlTreeNode<const value_type> *;

  AvlTreeNode() = default;

  explicit AvlTreeNode(const_reference val, pointer_type left = nullptr, pointer_type right = nullptr, pointer_type parent = nullptr, difference_type height = 1);
  /**
   * @brief Gets a pointer to the value stored in the node.
   * @return A pointer to the value stored in the node.
   */
  value_type *value();

  /**
   * @brief Gets a const pointer to the value stored in the node.
   * @return A const pointer to the value stored in the node.
   */
  const value_type *const_value() const;

  value_type value_;
};

template<typename ValueType>
AvlTreeNode<ValueType>::AvlTreeNode(const_reference val, pointer_type left, pointer_type right, pointer_type parent, difference_type height)  : AvlTreeNodeBase(left, right, parent, height),
																																				value_(val)
{
}

template<typename ValueType>
auto AvlTreeNode<ValueType>::value() -> AvlTreeNode<ValueType>::value_type *
{
	return &value;
}

template<typename ValueType>
auto AvlTreeNode<ValueType>::const_value() const -> const AvlTreeNode<ValueType>::value_type *
{
	return &value;
}

/**
 * @class AvlTreeHeader
 * @brief Header for AVL tree.
 * @details This class holds the header for an AVL tree (used as placeholder).
 * @par It includes a pointer to the root node of the tree and the count of nodes in the tree.
 */
class AvlTreeHeader {
 public:
  using size_type = std::size_t;

  /**
   * @brief Constructs a new `AvlTreeHeader` by moving data from another `AvlTreeHeader`.
   * @param other The `AvlTreeHeader` to move data from.
   * @par If the other `AvlTreeHeader` is empty, this `AvlTreeHeader` is reset.
   * @par Otherwise, data is moved from the other `AvlTreeHeader`.
   */
  AvlTreeHeader();

  AvlTreeHeader(AvlTreeHeader &&other) noexcept;

  /**
   * @brief Moves data from another `AvlTreeHeader` to this `AvlTreeHeader`.
   * @param other The `AvlTreeHeader` to move data from.
   */
  void move_data(AvlTreeHeader &other);

  /**
   * @brief Resets this `AvlTreeHeader` to its initial state.
   */
  void reset();

 public:
  AvlTreeNodeBase header_;
  size_type node_count_;
};

AvlTreeHeader::AvlTreeHeader()
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
	node_count_ = other.node_count_;

	other.reset();
	other.node_count_ = 0;
}

void AvlTreeHeader::reset()
{
	header_.parent_ = nullptr;
	header_.left_ = &header_;
	header_.right_ = &header_;
	node_count_ = 0;
}

/*
 * Forward declarations of tree classes.
 */
template<typename ValueType, class Comparator = std::less<ValueType>>
class AvlTreeImplementation;

template<typename ValueType, class Comparator = std::less<ValueType>>
class AvlTree;

/*
template<typename PointerType>
concept AvlTreeNodePointerType = std::is_same_v<typeid(PointerType), typeid(AvlTreeNodeBase::Avl_Base_ptr)>; // || std::is_same_v<PointerType, AvlTreeNodeBase::Avl_Const_Base_ptr>;
*/
/**
 * @brief Checks if a node is a placeholder.
 * @par Note: Placeholder has following property: node->parent->parent = node.
 * @tparam AvlTreeNodePointer A pointer to an AVL tree node. This can be either a pointer to an `AvlTreeNodeBase` or a pointer to a `const AvlTreeNodeBase`.
 * @param node The node to check.
 * @return `true` if the node is a placeholder, `false` otherwise.
 */
bool is_placeholder(AvlTreeNodeBase::Avl_Base_ptr node)
{
	if (node == nullptr)
	{
		return false;
	}

	return node->parent_ == node;
}

bool is_placeholder(AvlTreeNodeBase::Avl_Const_Base_ptr node)
{
	if (node == nullptr)
	{
		return false;
	}

	return node->parent_ == node;
}

/**
 * @brief Increments iterator in the AVL tree.
 * @tparam AvlTreeNodePointer A pointer to an AVL tree node.
 * @par This can be either a pointer to an `AvlTreeNodeBase` or a pointer to a `const AvlTreeNodeBase`.
 * @param node The node to increment.
 * @return A pointer to the next node in the AVL tree.
 */
template<typename ValueType>
AvlTreeNode<ValueType>::pointer_type avl_tree_increment(typename AvlTreeNode<ValueType>::pointer_type node)
{
	using pointer_type = typename AvlTreeNode<ValueType>::pointer_type;

	if (node == nullptr)
	{
		return nullptr;
	}

	/*
	 * If placeholder node detected then increment does not perform.
	 */
	if (is_placeholder(node))
	{
		return node;
	}

	if (node->right_ != nullptr)
	{
		/*
		 * If node has right child then go to leftmost node of this child.
		 */
		node = static_cast<pointer_type>(node->right_);

		if (!is_placeholder(node))
		{
			while (node->left_ != nullptr)
			{
				node = static_cast<pointer_type>(node->left_);
			}
		}
	}
	else
	{
		/*
		 * We've reached the leaf node, so we're coming out of it
		 */
		pointer_type temp;

		while (node->parent_ != nullptr)
		{
			temp = node;
			node = static_cast<pointer_type>(node->parent_);

			if (static_cast<pointer_type>(node->left_) == temp)
			{
				break;
			}
		}
	}

	return node;
}

template<typename ValueType>
AvlTreeNode<ValueType>::const_pointer_type cavl_tree_increment(typename AvlTreeNode<ValueType>::const_pointer_type node)
{
	using pointer_type = typename AvlTreeNode<ValueType>::const_pointer_type;

	if (node == nullptr)
	{
		return nullptr;
	}

	/*
	 * If placeholder node detected then increment does not perform.
	 */
	if (is_placeholder(node))
	{
		return node;
	}

	if (node->right_ != nullptr)
	{
		/*
		 * If node has right child then go to leftmost node of this child.
		 */
		node = static_cast<pointer_type>(node->right_);

		if (!is_placeholder(node))
		{
			while (node->left_ != nullptr)
			{
				node = static_cast<pointer_type>(node->left_);
			}
		}
	}
	else
	{
		/*
		 * We've reached the leaf node, so we're coming out of it
		 */
		pointer_type temp;

		while (node->parent_ != nullptr)
		{
			temp = node;
			node = static_cast<pointer_type>(node->parent_);

			if (static_cast<pointer_type>(node->left_) == temp)
			{
				break;
			}
		}
	}

	return node;
}

/**
 * @brief Decrements iterator in the AVL tree.
 * @tparam AvlTreeNodePointer A pointer to an AVL tree node.
 * @par This can be either a pointer to an `AvlTreeNodeBase` or a pointer to a `const AvlTreeNodeBase`.
 * @param node The node to decrement.
 * @return A pointer to the next node in the AVL tree.
 */
template<typename ValueType>
AvlTreeNode<ValueType>::pointer_type avl_tree_decrement(typename AvlTreeNode<ValueType>::pointer_type node)
{
	using pointer_type = typename AvlTreeNode<ValueType>::pointer_type;

	if (node == nullptr)
	{
		return nullptr;
	}

	/*
	 * If placeholder node detected then decrement does not perform.
	 */
	if (is_placeholder(node))
	{
		return node;
	}

	if (node->left_ != nullptr)
	{
		/*
		 * If node has left child then go to rightmost node of this child.
		 */
		node = static_cast<pointer_type>(node->left_);

		if (!is_placeholder(node))
		{
			while (node->right_ != nullptr)
			{
				node = static_cast<pointer_type>(node->right_);
			}
		}
	}
	else
	{
		pointer_type temp;

		while (node->parent_ != nullptr)
		{
			temp = node;
			node = static_cast<pointer_type>(node->parent_);

			if (static_cast<pointer_type>(node->right_) == temp)
			{
				break;
			}
		}
	}
	return node;
}

template<typename ValueType>
AvlTreeNode<ValueType>::const_pointer_type cavl_tree_decrement(typename AvlTreeNode<ValueType>::const_pointer_type node)
{
	using pointer_type = typename AvlTreeNode<ValueType>::const_pointer_type;

	if (node == nullptr)
	{
		return nullptr;
	}

	/*
	 * If placeholder node detected then decrement does not perform.
	 */
	if (is_placeholder(node))
	{
		return node;
	}

	if (node->left_ != nullptr)
	{
		/*
		 * If node has left child then go to rightmost node of this child.
		 */
		node = static_cast<pointer_type>(node->left_);

		if (!is_placeholder(node))
		{
			while (node->right_ != nullptr)
			{
				node = static_cast<pointer_type>(node->right_);
			}
		}
	}
	else
	{
		pointer_type temp;

		while (node->parent_ != nullptr)
		{
			temp = node;
			node = static_cast<pointer_type>(node->parent_);

			if (static_cast<pointer_type>(node->right_) == temp)
			{
				break;
			}
		}
	}
	return node;
}

template<typename ValueType>
class AvlTreeIterator {
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
  AvlTreeNode<value_type> *node_;
};

template<typename ValueType>
AvlTreeIterator<ValueType>::AvlTreeIterator() : node_(nullptr)
{
}

template<typename ValueType>
AvlTreeIterator<ValueType>::AvlTreeIterator(AvlTreeNodeBase::Avl_Base_ptr node) : node_(static_cast<AvlTreeNode<ValueType> *>(node))
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
	node_ = avl_tree_increment<ValueType>(node_);
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
	node_ = avl_tree_decrement<ValueType>(node_);
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

template<typename ValueType>
class AvlTreeConstIterator {
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
	return node_->const_value();
}

template<typename ValueType>
auto AvlTreeConstIterator<ValueType>::operator++() -> self &
{
	node_ = cavl_tree_increment<ValueType>(node_);
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
	node_ = cavl_tree_decrement<ValueType>(node_);
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

template<typename ValueType, class Comparator>
class AvlTreeImplementation : public AvlTreeHeader {
 public:
  using self = AvlTreeImplementation;
  using value_type = ValueType;
  using node_type = AvlTreeNode<value_type>;
  using node_pointer = node_type *;
  using pointer_type = node_type::Avl_Base_ptr;
  using iterator = AvlTreeIterator<value_type>;
  using const_iterator = AvlTreeConstIterator<value_type>;

  // TODO implement std::reverse_iterators

  AvlTreeImplementation();

  ~AvlTreeImplementation();

  AvlTreeImplementation(AvlTreeImplementation &&other) noexcept;

  self &operator=(AvlTreeImplementation &&other) noexcept;

  // TODO implement copy constructor
  AvlTreeImplementation(const AvlTreeImplementation &other) = delete;
  self &operator=(const AvlTreeImplementation &other) = delete;

  /*
   * iterators
   */
  iterator begin() noexcept;

  const_iterator begin() const noexcept;

  const_iterator cbegin() const noexcept;

  iterator end() noexcept;

  const_iterator end() const noexcept;

  const_iterator cend() const noexcept;

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

  iterator find(const value_type &value);

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
  void recursive_destroy(pointer_type node);

  node_pointer root_;
  Comparator compare_;
};

template<typename ValueType, class Comparator>
auto AvlTreeImplementation<ValueType, Comparator>::find(const value_type &value) -> iterator
{
	node_pointer node = root_;

	while (node != nullptr && node->value_ != value)
	{
		if (compare_(value, node->value_))
		{
			node = static_cast<node_pointer>(node->left_);
		}
		else
		{
			node = static_cast<node_pointer>(node->right_);
		}
	}

	return iterator(node);
}

template<typename ValueType, class Comparator>
AvlTreeImplementation<ValueType, Comparator>::AvlTreeImplementation() : AvlTreeHeader(), root_(nullptr)
{
}

template<typename ValueType, class Comparator>
AvlTreeImplementation<ValueType, Comparator>::~AvlTreeImplementation()
{
	clear();
}

template<typename ValueType, class Comparator>
AvlTreeImplementation<ValueType, Comparator>::AvlTreeImplementation(AvlTreeImplementation &&other) noexcept
{
	// TODO should we check self assigment here ?
	header_ = std::move(other.header_);
	root_ = other.root_;
}

template<typename ValueType, class Comparator>
auto AvlTreeImplementation<ValueType, Comparator>::operator=(AvlTreeImplementation &&other) noexcept -> self &
{
	if (this != &other)
	{
		header_ = std::move(other.header_);
		root_ = other.root_;
	}

	return *this;
}

template<typename ValueType, class Comparator>
auto AvlTreeImplementation<ValueType, Comparator>::begin() noexcept -> iterator
{
	return iterator(header_.left_);
}

template<typename ValueType, class Comparator>
auto AvlTreeImplementation<ValueType, Comparator>::begin() const noexcept -> const_iterator
{
	return iterator(header_.left_);
}

template<typename ValueType, class Comparator>
auto AvlTreeImplementation<ValueType, Comparator>::cbegin() const noexcept -> const_iterator
{
	return const_iterator(header_.left_);
}

template<typename ValueType, class Comparator>
auto AvlTreeImplementation<ValueType, Comparator>::end() noexcept -> iterator
{
	return iterator(&header_);
}

template<typename ValueType, class Comparator>
auto AvlTreeImplementation<ValueType, Comparator>::end() const noexcept -> const_iterator
{
	return const_iterator(&header_);
}

template<typename ValueType, class Comparator>
auto AvlTreeImplementation<ValueType, Comparator>::cend() const noexcept -> const_iterator
{
	return const_iterator(&header_);
}

template<typename ValueType, class Comparator>
auto AvlTreeImplementation<ValueType, Comparator>::size() const -> size_type
{
	return node_count_;
}

template<typename ValueType, class Comparator>
bool AvlTreeImplementation<ValueType, Comparator>::empty() const
{
	return size() == 0;
}

template<typename ValueType, class Comparator>
void AvlTreeImplementation<ValueType, Comparator>::clear()
{
	recursive_destroy(root_);
	reset();
}

template<typename ValueType, class Comparator>
void AvlTreeImplementation<ValueType, Comparator>::recursive_destroy(pointer_type node)
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
auto AvlTreeImplementation<ValueType, Comparator>::insert(const value_type &value) -> std::pair<iterator, bool>
{
	if (root_ == nullptr)
	{
		root_ = new node_type(value);
		header_.left_ = root_;
		header_.right_ = root_;
		root_->left_ = &header_;
		root_->right_ = &header_;
		header_.parent_ = &header_;

		node_count_ = 1;
		return {begin(), true};
	}

	node_pointer needle = nullptr;
	node_pointer current = root_;

	while (needle == nullptr)
	{
		if (is_placeholder(current->left_))
		{
			// we want to insert value smaller than smallest
			current->left_ = new node_type(value);
			current->left_->parent_ = current;
			needle = static_cast<node_pointer>(current->left_);

			header_.left_ = needle;
			needle->left_ = &header_;
		}
		else if (is_placeholder(current->right_))
		{
			// we want to insert value greater than largest
			current->right_ = new node_type(value);
			current->right_->parent_ = current;
			needle = static_cast<node_pointer>(current->right_);

			header_.right_ = needle;
			needle->right_ = &header_;
		}
		else
		{
			if (compare_(value, current->value_))
			{
				if (current->left_ == nullptr)
				{
					current->left_ = new node_type(value);
					current->left_->parent_ = current;
					needle = static_cast<node_pointer>(current->left_);
				}
				else
				{
					current = static_cast<node_pointer>(current->left_);
				}
			}
			else if (compare_(current->value_, value))
			{
				if (current->right_ == nullptr)
				{
					current->right_ = new node_type(value);
					current->right_->parent_ = current;
					needle = static_cast<node_pointer>(current->right_);
				}
				else
				{
					current = static_cast<node_pointer>(current->right_);
				}
			}
			else
			{
				// current node contains same value
				return {iterator(needle), false};
			}
		}
	}

	++node_count_;

	// update heights for all ancestors
	needle->iterative_height_update();

	// balance nodes start from parent
	balance_bottom_up(static_cast<node_pointer>(needle->parent_));

	return {iterator(needle), true};
}

template<typename ValueType, class Comparator>
auto AvlTreeImplementation<ValueType, Comparator>::rotate_left(node_pointer node) -> node_pointer
{
	node_pointer new_root = static_cast<node_pointer>(node->right_);

	node->right_ = new_root->left_;
	node->right_->parent_ = node;

	new_root->left_ = node;
	new_root->left_->parent_ = new_root;

	new_root->parent_ = nullptr;

	node->iterative_height_update();
	new_root->iterative_height_update();

	return new_root;
}

template<typename ValueType, class Comparator>
auto AvlTreeImplementation<ValueType, Comparator>::rotate_right(node_pointer node) -> node_pointer
{
	node_pointer new_root = static_cast<node_pointer>(node->left_);

	node->left_ = new_root->right_;
	node->left_->parent_ = node;

	new_root->right_ = node;
	new_root->right_->parent_ = new_root;

	new_root->parent_ = nullptr;

	node->iterative_height_update();
	new_root->iterative_height_update();

	return new_root;
}

template<typename ValueType, class Comparator>
auto AvlTreeImplementation<ValueType, Comparator>::small_left_rotate(node_pointer node) -> node_pointer
{
	if (node->parent_ == nullptr)
	{
		return rotate_left(node);
	}

	node_pointer parent = static_cast<node_pointer>(node->parent_);
	node_pointer left = static_cast<node_pointer>(parent->left_);
	node_pointer right = static_cast<node_pointer>(parent->right_);

	node_pointer &part = static_cast<node_pointer>(parent->left_) == node ? left : right;

	node->parent_ = nullptr;
	part = rotate_left(node);
	part->parent_ = parent;
	parent->iterative_height_update();

	return part;
}

template<typename ValueType, class Comparator>
auto AvlTreeImplementation<ValueType, Comparator>::big_left_rotate(node_pointer node) -> node_pointer
{
	node->right_ = small_right_rotate(static_cast<node_pointer>(node->right_));
	node->right_->parent_ = node;
	node->iterative_height_update();

	return small_left_rotate(node);
}

template<typename ValueType, class Comparator>
auto AvlTreeImplementation<ValueType, Comparator>::small_right_rotate(node_pointer node) -> node_pointer
{
	if (node->parent_ == nullptr)
	{
		return rotate_right(node);
	}

	node_pointer parent = static_cast<node_pointer>(node->parent_);
	node_pointer left = static_cast<node_pointer>(parent->left_);
	node_pointer right = static_cast<node_pointer>(parent->right_);

	node_pointer &part = static_cast<node_pointer>(parent->left_) == node ? left : right;

	node->parent_ = nullptr;
	part = rotate_right(node);
	part->parent_ = parent;
	parent->iterative_height_update();

	return part;
}

template<typename ValueType, class Comparator>
auto AvlTreeImplementation<ValueType, Comparator>::big_right_rotate(node_pointer node) -> node_pointer
{
	node->left_ = small_left_rotate(static_cast<node_pointer>(node->left_));
	node->left_->parent_ = node;
	node->iterative_height_update();

	return small_right_rotate(node);
}

template<typename ValueType, class Comparator>
auto AvlTreeImplementation<ValueType, Comparator>::balance_node(node_pointer node) -> node_pointer
{
	if (node == nullptr)
	{
		return node;
	}

	auto balance_factor = node->balance_factor();

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

	return node;
}

template<typename ValueType, class Comparator>
auto AvlTreeImplementation<ValueType, Comparator>::balance_bottom_up(node_pointer node) -> node_pointer
{
	node_pointer temp = node;

	while (temp != nullptr)
	{
		temp = balance_node(temp);

		if (temp->parent_ == nullptr)
		{
			root_ = temp;
		}
		temp = static_cast<node_pointer>(temp->parent_);
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
// * i.e this
// *                  a
// *              /       \
// *             L         b
// *					   /   \
// *					  C     R
// *                  /  \
// *                M     N
// *
// * will converted to this
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
// * will converted to this
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