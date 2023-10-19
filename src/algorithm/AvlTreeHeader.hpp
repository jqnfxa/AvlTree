#pragma once

#include <cstddef>
#include "AvlTreeNode.hpp"

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