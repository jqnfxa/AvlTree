#pragma once

#include <cstddef>
#include <algorithm>

/**
 * @brief An AVL tree node.
 * @tparam ValueType The type of the value stored in the tree node.
 */
template <typename ValueType>
class AvlTreeNode {
public:
	using value_type = ValueType;
	using const_reference = const value_type &;
	using difference_type = std::ptrdiff_t;
	using self = AvlTreeNode<value_type>;
	using pointer_type = self *;

	/**
     * @brief Default constructor.
     */
	AvlTreeNode() = default;

	/**
     * @brief Constructor with a value and optional left, right, and parent pointers.
     * @param val The value of the node.
     * @param left The left child of the node (default is nullptr).
     * @param right The right child of the node (default is nullptr).
     * @param parent The parent of the node (default is nullptr).
     */
	explicit AvlTreeNode(const_reference val,
						 const pointer_type &left = nullptr,
						 const pointer_type &right = nullptr,
						 const pointer_type &parent = nullptr);

	/**
     * @brief Move assignment constructor.
     * @param other The node to be moved.
     */
	AvlTreeNode(self &&other) noexcept;

	/**
     * @brief Copy assignment constructor.
     * @param other The node to be copied.
     */
	AvlTreeNode(const self &other) noexcept;

	/**
     * @brief Move assignment operator.
     * @param other The node to be moved.
     * @return A reference to the moved node.
     */
	self &operator=(self &&other) & noexcept;

	/**
     * @brief Copy assignment operator.
     * @param other The node to be moved.
     * @return A reference to the copied node.
     */
	self &operator=(const self &other) & noexcept;

	/**
     * @brief Get the predecessor of the node.
     * @return The predecessor of the node.
     */
	[[nodiscard]] pointer_type predecessor() & noexcept;

	/**
     * @brief Get the successor of the node.
     * @return The successor of the node.
     */
	[[nodiscard]] pointer_type successor() & noexcept;

	/**
     * @brief Get the height of the left subtree.
     * @return The height of the left subtree.
     */
	[[nodiscard]] difference_type left_height() const & noexcept;

	/**
     * @brief Get the height of the right subtree.
     * @return The height of the right subtree.
     */
	[[nodiscard]] difference_type right_height() const & noexcept;

	/**
     * @brief Get the balance factor of the node.
     * @return The balance factor of the node.
     */
	[[nodiscard]] difference_type balance_factor() const & noexcept;

	/**
     * @brief Check if the node is a placeholder.
     * @return True if the node is a placeholder.
     */
	[[nodiscard]] bool is_placeholder() const & noexcept;

	/**
	* @brief Update the height of the node and its ancestors iteratively.
	*/
	void iterative_height_update() & noexcept;

	/**
     * @brief Update the height of the node standalone.
     */
	void update_height_standalone() & noexcept;

	/**
     * @brief Reset the node.
     */
	void reset() & noexcept;

	/**
     * @brief Unlink the placeholder.
     */
	void unlink_placeholder() & noexcept;

	/**
     * @brief Restore the placeholder.
     */
	void restore_placeholder() & noexcept;

public:
	pointer_type left_;
	pointer_type right_;
	pointer_type parent_;

	value_type value_;
	difference_type height_{};
};

template <typename ValueType>
AvlTreeNode<ValueType>::AvlTreeNode(AvlTreeNode::self &&other) noexcept
{
	reset();

	left_ = std::move(other.base_.left_);
	right_ = std::move(other.base_.right_);
	value_ = std::move(other.value_);
	height_ = std::move(other.base_.height_);

	other.reset();
}

template <typename ValueType>
AvlTreeNode<ValueType>::AvlTreeNode(const AvlTreeNode::self &other) noexcept
{
	value_ = other.value_;
	left_ = this;
	right_ = this;
	parent_ = this;
}

template <typename ValueType>
AvlTreeNode<ValueType>::AvlTreeNode(const_reference val,
									const pointer_type &left,
									const pointer_type &right,
									const pointer_type &parent)
	: left_(left),
	  right_(right),
	  parent_(parent),
	  value_(val),
	  height_(1)
{
	iterative_height_update();
}

template <typename ValueType>
auto AvlTreeNode<ValueType>::operator=(AvlTreeNode &&other) & noexcept -> self &
{
	if (this != &other)
	{
		reset();

		left_ = std::move(other.base_.left_);
		right_ = std::move(other.base_.right_);
		value_ = std::move(other.value_);
		height_ = std::move(other.base_.height_);

		other.reset();
	}

	return *this;
}

template <typename ValueType>
auto AvlTreeNode<ValueType>::operator=(const AvlTreeNode &other) & noexcept -> self &
{
	if (this != &other)
	{
		value_ = other.value_;
		left_ = this;
		right_ = this;
		parent_ = this;
	}

	return *this;
}

template <typename ValueType>
auto AvlTreeNode<ValueType>::predecessor() & noexcept -> pointer_type
{
	if (left_ == nullptr)
	{
		return const_cast<pointer_type>(this);
	}

	pointer_type predecessor = left_;

	while (predecessor != nullptr && predecessor->right_ != nullptr)
	{
		predecessor = predecessor->right_;
	}

	return predecessor;
}

template <typename ValueType>
auto AvlTreeNode<ValueType>::successor() & noexcept -> pointer_type
{
	if (right_ == nullptr)
	{
		return const_cast<pointer_type>(this);
	}

	pointer_type successor = right_;

	while (successor != nullptr && successor->left_ != nullptr)
	{
		successor = successor->left_;
	}

	return successor;
}

template <typename ValueType>
auto AvlTreeNode<ValueType>::left_height() const & noexcept -> difference_type
{
	return left_ == nullptr ? 0 : left_->height_;
}

template <typename ValueType>
auto AvlTreeNode<ValueType>::right_height() const & noexcept -> difference_type
{
	return right_ == nullptr ? 0 : right_->height_;
}

template <typename ValueType>
auto AvlTreeNode<ValueType>::balance_factor() const & noexcept -> difference_type
{
	return right_height() - left_height();
}

template <typename ValueType>
void AvlTreeNode<ValueType>::iterative_height_update() & noexcept
{
	// Update self height first
	update_height_standalone();

	pointer_type node = parent_;

	difference_type pre = -1, post = 1;

	while (node != nullptr && pre != post)
	{
		pre = node->height_;
		node->update_height_standalone();
		post = node->height_;

		node = node->parent_;
	}
}

template <typename ValueType>
void AvlTreeNode<ValueType>::update_height_standalone() & noexcept
{
	height_ = 1 + std::max(left_height(), right_height());
}

template <typename ValueType>
bool AvlTreeNode<ValueType>::is_placeholder() const & noexcept
{
	return parent_ == this;
}

template <typename ValueType>
void AvlTreeNode<ValueType>::reset() & noexcept
{
	parent_ = this;
	left_ = parent_;
	right_ = parent_;
	height_ = 0;
}

template <typename ValueType>
void AvlTreeNode<ValueType>::unlink_placeholder() & noexcept
{
	if (!left_->is_placeholder())
	{
		left_->left_ = nullptr;
	}
	if (!right_->is_placeholder())
	{
		right_->right_ = nullptr;
	}
}

template <typename ValueType>
void AvlTreeNode<ValueType>::restore_placeholder() & noexcept
{
	left_->left_ = parent_;
	right_->right_ = parent_;
}