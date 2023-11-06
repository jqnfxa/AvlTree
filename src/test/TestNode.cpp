#include "gtest/gtest.h"
#include <algorithm/AvlTreeNode.hpp>
#include <memory/memory.hpp>

TEST(AvlTreeNodeTest, test_constructor)
{
    using int_node = AvlTreeNode<int>;

    int_node *root = new int_node(10);
    int_node *left = new int_node(5);

    root->left_ = left;
    root->update_parent_for_left_child();
    root->iterative_height_update();

    ASSERT_EQ(root->left_->parent_, root);
    ASSERT_EQ(left->parent_, root);
    ASSERT_EQ(root->parent_, nullptr);
    ASSERT_EQ(left->left_, nullptr);
    ASSERT_EQ(left->right_, nullptr);
    ASSERT_EQ(root->left_->left_, nullptr);
    ASSERT_EQ(root->left_->right_, nullptr);
    ASSERT_EQ(root->right_, nullptr);
    ASSERT_EQ(root->height_, 2);
    ASSERT_EQ(left->height_, 1);

    left->left_ = new int_node(3);
    left->update_parent_for_left_child();
    left->iterative_height_update();

    ASSERT_EQ(left->left_->parent_, left);
    ASSERT_EQ(left->parent_, root);
    ASSERT_EQ(root->height_, 3);
    ASSERT_EQ(left->height_, 2);
    ASSERT_EQ(left->left_->height_, 1);

    safe_delete(root);
    safe_delete(left->left_);
    safe_delete(left);
}
