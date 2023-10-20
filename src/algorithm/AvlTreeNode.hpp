#pragma once

#include <cstddef>

/**
 * @class AvlTreeNodeBase
 * @brief Base class for AVL tree nodes.
 * @details This class provides a BaseImpl for AVL tree nodes. It has pointers to the parent, left and right child nodes.
 */
class AvlTreeNodeBase {
 public:
  using difference_type = std::ptrdiff_t;
  using Avl_Base_ptr = AvlTreeNodeBase *;
  using Avl_Const_Base_ptr = const AvlTreeNodeBase *;

  explicit AvlTreeNodeBase(Avl_Base_ptr left = nullptr, Avl_Base_ptr right = nullptr, Avl_Base_ptr parent = nullptr, difference_type height = 1);

  [[nodiscard]] Avl_Base_ptr predecessor() const;
  [[nodiscard]] Avl_Base_ptr successor() const;
  [[nodiscard]] difference_type left_height() const;
  [[nodiscard]] difference_type right_height() const;
  [[nodiscard]] difference_type balance_factor() const;
  [[nodiscard]] virtual Avl_Const_Base_ptr to_constant() const;

  void iterative_height_update();
  void update_height_standalone();
 public:
  Avl_Base_ptr parent_;
  Avl_Base_ptr left_;
  Avl_Base_ptr right_;
  difference_type height_;
};

/**
 * @class AvlTreeNode
 * @brief A node in an AVL tree.
 * @tparam ValueType The type of the value stored in the node.
 * @details This class represents a node in an AVL tree.
 * @par It inherits from `AvlTreeNodeBase` and adds a member for the value stored in the node.
 * @par It also provides methods for accessing the value stored in the node.
 */
template<typename ValueType>
class AvlTreeNode : public AvlTreeNodeBase {
 public:
  using value_type = ValueType;
  using value_pointer = value_type *;
  using const_value_pointer = const value_type *;
  using difference_type = std::ptrdiff_t;
  using const_reference = const value_type &;
  using pointer_type = AvlTreeNode<value_type> *;
  using const_pointer_type = const AvlTreeNode<value_type> *;

  AvlTreeNode() = default;

  explicit AvlTreeNode(const_reference val, pointer_type left = nullptr, pointer_type right = nullptr, pointer_type parent = nullptr, difference_type height = 1);
  /**
   * @brief Gets a pointer to the value stored in the node.
   * @return A pointer to the value stored in the node.
   */
  value_pointer value();

  /**
   * @brief Gets a const pointer to the value stored in the node.
   * @return A const pointer to the value stored in the node.
   */
  const_value_pointer value() const;

  pointer_type left();
  pointer_type right();
  pointer_type parent();

  const_pointer_type left() const;
  const_pointer_type right() const;
  const_pointer_type parent() const;

  const_pointer_type to_constant() const override;

  value_type value_;
};

template<typename ValueType>
AvlTreeNode<ValueType>::AvlTreeNode(const_reference val, pointer_type left, pointer_type right, pointer_type parent, difference_type height)  : AvlTreeNodeBase(left, right, parent, height),
																																				value_(val)
{
}

template<typename ValueType>
auto AvlTreeNode<ValueType>::value() -> value_pointer
{
	return &value_;
}

template<typename ValueType>
auto AvlTreeNode<ValueType>::value() const -> const_value_pointer
{
	return &value_;
}

template<typename ValueType>
auto AvlTreeNode<ValueType>::left() -> pointer_type
{
	return static_cast<pointer_type>(left_);
}

template<typename ValueType>
auto AvlTreeNode<ValueType>::right() -> pointer_type
{
	return static_cast<pointer_type>(right_);
}

template<typename ValueType>
auto AvlTreeNode<ValueType>::parent() -> pointer_type
{
	return static_cast<pointer_type>(parent_);
}

template<typename ValueType>
auto AvlTreeNode<ValueType>::left() const -> const_pointer_type
{
	return static_cast<const_pointer_type>(left_);
}

template<typename ValueType>
auto AvlTreeNode<ValueType>::right() const -> const_pointer_type
{
	return static_cast<const_pointer_type>(right_);
}

template<typename ValueType>
auto AvlTreeNode<ValueType>::parent() const -> const_pointer_type
{
	return static_cast<const_pointer_type>(parent_);
}

template<typename ValueType>
auto AvlTreeNode<ValueType>::to_constant() const -> const_pointer_type
{
	return static_cast<const_pointer_type>(this);
}