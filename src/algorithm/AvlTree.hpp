#pragma once

#include <cstdint>
#include "AvlTreeImplementation.hpp"
#include "AvlTreeIterator.hpp"

/**
 * @brief An AVL tree.
 * @tparam ValueType The type of the value stored in the tree.
 * @tparam Comparator The type of the comparator used to order the values in the tree (default is std::less<ValueType>).
 */
template <typename ValueType, class Comparator = std::less<ValueType>>
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

	/**
     * @brief Default constructor.
     */
	AvlTree();

	/**
     * @brief Destructor.
     */
	~AvlTree();

	/**
     * @brief Move constructor.
     * @param other The tree to be moved.
     */
	AvlTree(AvlTree &&other) noexcept;

	/**
     * @brief Copy constructor.
     * @param other The tree to be copied.
     */
	AvlTree(const AvlTree &other) noexcept;

	/**
     * @brief Move assignment operator.
     * @param other The tree to be moved.
     * @return Reference to the moved tree.
     */
	self &operator=(AvlTree &&other) noexcept;

	/**
     * @brief Copy assignment operator.
     * @param other The tree to be copied.
     * @return Reference to the copied tree.
     */
	self &operator=(const AvlTree &other) noexcept;

	/**
     * @brief Get the beginning iterator of the tree.
     * @return The beginning iterator of the tree.
     */
	iterator begin() const & noexcept;

	/**
     * @brief Get the ending iterator of the tree.
     * @return The ending iterator of the tree.
     */
	iterator end() const & noexcept;

	/**
     * @brief Get the reverse beginning iterator of the tree.
     * @return The reverse beginning iterator of the tree.
     */
	reverse_iterator rbegin() const & noexcept;

	/**
     * @brief Get the reverse ending iterator of the tree.
     * @return The reverse ending iterator of the tree.
     */
	reverse_iterator rend() const & noexcept;

	/**
     * @brief Get the size of the tree.
     * @return The size of the tree.
     */
	[[nodiscard]] size_type size() const & noexcept;

	/**
     * @brief Check if the tree is empty.
     * @return True if the tree is empty.
     */
	[[nodiscard]] bool empty() const & noexcept;

	/**
     * @brief Clear the tree.
     */
	void clear() & noexcept;

	/**
     * @brief Insert a value into the tree.
     * @param value The value to be inserted.
     * @return A pair consisting of an iterator to the inserted element (or to the element that prevented the insertion) and a bool denoting whether the insertion took place.
     */
	std::pair<iterator, bool> insert(const value_type &value) & noexcept;

	/**
     * @brief Insert a value into the tree.
     * @param value The value to be inserted.
     * @return A pair consisting of an iterator to the inserted element (or to the element that prevented the insertion) and a bool denoting whether the insertion took place.
     */
	std::pair<iterator, bool> insert(value_type &&value) & noexcept;

	/**
     * @brief Remove the node with the smallest value from the tree.
     */
	void erase_smallest() & noexcept;

	/**
     * @brief Remove the node with the largest value from the tree.
     */
	void erase_largest() & noexcept;

	/**
 	 * @brief Remove a value from the tree.
 	 * @param value The value to be removed.
 	 */
	void erase(const value_type &value) & noexcept;

	/**
 	 * @brief Remove an element from the tree.
 	 * @param pos An iterator pointing to the element to be removed.
 	 */
	void erase(const iterator &pos) & noexcept;

	/**
 	 * @brief Find an element with a specific value in the tree.
 	 * @param value The value of the element to be found.
 	 * @return An iterator pointing to the element with the specific value.
 	 */
	iterator find(const value_type &value) & noexcept;

	/**
 	 * @brief Find an element with a specific value in the tree.
 	 * @param value The value of the element to be found.
 	 * @return An iterator pointing to the element with the specific value.
 	 */
	iterator find(value_type &&value) & noexcept;

private:
	/**
	 * @brief Clone the tree.
	 * @param other The tree to be cloned.
	 */
	void clone(const self &other) const & noexcept;

	BaseImpl base_;
	AvlTreeNode<value_type> header_;
};

template <typename ValueType, class Comparator>
void AvlTree<ValueType, Comparator>::clone(const self &other) const & noexcept
{
	base_ = other.base_;
	header_.reset();

	auto node = base_.root_;

	while (node != nullptr && node->left_ != nullptr)
	{
		node = node->left_;
	}

	if (node == nullptr)
	{
		header_.left_ = header_.parent_;
	}
	else
	{
		header_.left_ = node;
	}

	node = base_.root_;

	while (node != nullptr && node->right_ != nullptr)
	{
		node = node->right_;
	}

	if (node == nullptr)
	{
		header_.right_ = header_.parent_;
	}
	else
	{
		header_.right_ = node;
	}

	header_.height_ = other.header_.height_;
	header_.restore_placeholder();
}

template <typename ValueType, class Comparator>
AvlTree<ValueType, Comparator>::AvlTree() : base_(), header_(0xffffff)
{
	header_.reset();
}

template <typename ValueType, class Comparator>
AvlTree<ValueType, Comparator>::~AvlTree()
{
	clear();
}

template <typename ValueType, class Comparator>
AvlTree<ValueType, Comparator>::AvlTree(AvlTree &&other) noexcept
{
	base_ = std::move(other.base_);
	header_ = std::move(other.header_);
}

template <typename ValueType, class Comparator>
AvlTree<ValueType, Comparator>::AvlTree(const AvlTree &other) noexcept
{
	clone(other);
}

template <typename ValueType, class Comparator>
auto AvlTree<ValueType, Comparator>::operator=(AvlTree &&other) noexcept -> self &
{
	if (this != &other)
	{
		base_ = std::move(other.base_);
		header_ = std::move(other.header_);
	}

	return *this;
}

template <typename ValueType, class Comparator>
auto AvlTree<ValueType, Comparator>::operator=(const AvlTree &other) noexcept -> self &
{
	if (this != &other)
	{
		clear();
		clone(other);
	}

	return *this;
}

template <typename ValueType, class Comparator>
auto AvlTree<ValueType, Comparator>::begin() const & noexcept -> iterator
{
	return iterator(header_.left_);
}

template <typename ValueType, class Comparator>
auto AvlTree<ValueType, Comparator>::end() const & noexcept -> iterator
{
	return iterator(header_.parent_);
}

template <typename ValueType, class Comparator>
auto AvlTree<ValueType, Comparator>::rbegin() const & noexcept -> reverse_iterator
{
	return reverse_iterator(end());
}

template <typename ValueType, class Comparator>
auto AvlTree<ValueType, Comparator>::rend() const & noexcept -> reverse_iterator
{
	return reverse_iterator(begin());
}

template <typename ValueType, class Comparator>
auto AvlTree<ValueType, Comparator>::size() const & noexcept -> size_type
{
	return base_.size();
}

template <typename ValueType, class Comparator>
bool AvlTree<ValueType, Comparator>::empty() const & noexcept
{
	return base_.empty();
}

template <typename ValueType, class Comparator>
void AvlTree<ValueType, Comparator>::clear() & noexcept
{
	header_.unlink_placeholder();
	header_.reset();
	base_.clear();
}

template <typename ValueType, class Comparator>
auto AvlTree<ValueType, Comparator>::insert(const value_type &value) & noexcept -> std::pair<iterator, bool>
{
	// Value already exists
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

		// We want to update the smallest or the largest values, therefore update header
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

template <typename ValueType, class Comparator>
auto AvlTree<ValueType, Comparator>::insert(value_type &&value) & noexcept -> std::pair<iterator, bool>
{
	return insert(static_cast<ValueType &>(value));
}

template <typename ValueType, class Comparator>
void AvlTree<ValueType, Comparator>::erase_smallest() & noexcept
{
	erase(iterator(header_.left_));
}

template <typename ValueType, class Comparator>
void AvlTree<ValueType, Comparator>::erase_largest() & noexcept
{
	erase(iterator(header_.right_));
}

template <typename ValueType, class Comparator>
void AvlTree<ValueType, Comparator>::erase(const value_type &value) & noexcept
{
	erase(find(value));
}

template <typename ValueType, class Comparator>
void AvlTree<ValueType, Comparator>::erase(const iterator &pos) & noexcept
{
	// Cannot erase placeholder
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

	// We want to update the smallest or the largest values, therefore update header
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

template <typename ValueType, class Comparator>
auto AvlTree<ValueType, Comparator>::find(const value_type &value) & noexcept -> iterator
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

template <typename ValueType, class Comparator>
auto AvlTree<ValueType, Comparator>::find(value_type &&value) & noexcept -> iterator
{
	return find(static_cast<value_type &>(value));
}