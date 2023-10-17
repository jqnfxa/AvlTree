#pragma once

#include <cstdint>
#include <algorithm>

template<typename Type>
class Node {
 public:
  using different_type = std::ptrdiff_t;
  using value_type = Type;
  using reference = value_type &;
  using const_reference = const value_type &;
  using ptr_type = Node<value_type> *;
  using const_ptr_type = const Node<value_type> *;
 public:
  value_type value;
  ptr_type left;
  ptr_type right;
  ptr_type parent;

  explicit Node(const_reference val, ptr_type left = nullptr, ptr_type right = nullptr);

  [[nodiscard]] different_type left_height() const;
  [[nodiscard]] different_type right_height() const;

  void update_height();
  void update_right(ptr_type new_node);
  void update_left(ptr_type new_node);
  void unlink_parent();
  void link_parent(ptr_type new_parent);

  [[nodiscard]] different_type height() const;
  [[nodiscard]] different_type balance_factor() const;

 private:
  void iterative_height_update();
  void update_height_standalone();

  different_type height_;
};

template<typename Type>
void Node<Type>::iterative_height_update()
{
	ptr_type node = this;

	different_type pre = -1, post = -2;

	while (node != nullptr && pre != post)
	{
		pre = node->height();
		update_height_standalone();
		post = node->height();

		node = node->parent;
	}
}

template<typename Type>
void Node<Type>::update_height_standalone()
{
	height_ = 1 + std::max(left_height(), right_height());
}

template<typename Type>
auto Node<Type>::right_height() const -> different_type
{
	return right == nullptr ? 0 : right->height();
}

template<typename Type>
auto Node<Type>::left_height() const -> different_type
{
	return left == nullptr ? 0 : left->height();
}

template<typename Type>
Node<Type>::Node(const_reference val, ptr_type left, ptr_type right) : value(val), left(left), right(right), parent(nullptr), height_(1)
{
	update_height();
}

template<typename Type>
Node<Type>::different_type Node<Type>::height() const
{
	return height_;
}

template<typename Type>
Node<Type>::different_type Node<Type>::balance_factor() const
{
	return right_height() - left_height();
}

template<typename Type>
void Node<Type>::update_height()
{
	iterative_height_update();
}

template<typename Type>
void Node<Type>::update_left(Node::ptr_type new_node)
{
	if (left != nullptr)
	{
		left->unlink_parent();
	}

	left = new_node;

	if (left != nullptr)
	{
		left->link_parent(this);
	}

	update_height();
}

template<typename Type>
void Node<Type>::update_right(Node::ptr_type new_node)
{
	if (right != nullptr)
	{
		right->unlink_parent();
	}

	right = new_node;

	if (right != nullptr)
	{
		right->link_parent(this);
	}

	update_height();
}

template<typename Type>
void Node<Type>::unlink_parent()
{
	// unlink our pointer from parent
	parent = nullptr;
}

template<typename Type>
void Node<Type>::link_parent(Node::ptr_type new_parent)
{
	parent = new_parent;
	parent->update_height();
}