#include "gtest/gtest.h"
#include <algorithm>
#include <implementation/AvlTreeImplementation.hpp>
#include <memory/memory.hpp>
#include <random>

template <typename T>
int force_height(AvlTreeNode<T> *node)
{
    if (node == nullptr)
    {
        return 0;
    }

    return 1 + std::max(force_height(node->left_), force_height(node->right_));
}

TEST(AvlTreeTest, test_avl_tree_constructor)
{
    AvlTreeBase<int> tree;
    ASSERT_EQ(0, tree.size());
    ASSERT_TRUE(tree.empty());
    ASSERT_EQ(tree.root_, nullptr);
}

TEST(AvlTreeTest, test_avl_tree_copy_constructor)
{
    AvlTreeBase<int> tree;

    tree.insert(new AvlTreeNode<int>(10));
    tree.insert(new AvlTreeNode<int>(20));
    tree.insert(new AvlTreeNode<int>(50));

    ASSERT_EQ(tree.root_->value_, 20);
    ASSERT_EQ(tree.root_->height_, 2);
    ASSERT_EQ(tree.root_->balance_factor(), 0);

    AvlTreeBase<int> copy;
    copy = tree;

    ASSERT_EQ(copy.root_->value_, 20);

    copy.clear();

    ASSERT_EQ(tree.root_->value_, 20);
    ASSERT_EQ(tree.root_->height_, 2);
    ASSERT_EQ(tree.root_->balance_factor(), 0);
}

TEST(AvlTreeTest, test_avl_tree_move_constructor)
{
    AvlTreeBase<int> tree;

    for (int i = 1; i <= 10; ++i)
    {
        tree.insert(new AvlTreeNode<int>(i));
    }

    AvlTreeBase<int> moved_tree(std::move(tree));

    ASSERT_EQ(moved_tree.root_->height_, 4);
    ASSERT_EQ(moved_tree.root_->leftmost()->value_, 1);
    ASSERT_EQ(moved_tree.root_->rightmost()->value_, 10);

    ASSERT_EQ(moved_tree.root_->predecessor()->value_, 3);
    ASSERT_EQ(moved_tree.root_->successor()->value_, 5);
    ASSERT_EQ(moved_tree.root_->right_->predecessor()->value_, 7);

    ASSERT_EQ(tree.root_, nullptr);
    ASSERT_EQ(tree.number_of_nodes_, 0);
}

TEST(AvlTreeTest, test_avl_tree_insert)
{
    AvlTreeBase<int> tree;
    const std::size_t size = 1000;
    std::vector<int> values(size);
    std::iota(values.begin(), values.end(), 1);

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(values.begin(), values.end(), g);

    for (int value : values)
    {
        tree.insert(new AvlTreeNode<int>(value));
    }

    ASSERT_EQ(tree.root_->leftmost()->value_, 1);
    ASSERT_EQ(tree.root_->rightmost()->value_, size);
    ASSERT_TRUE(tree.root_->height_ < static_cast<int>(1.440 * std::log2(size + 2) - 0.328));
    ASSERT_EQ(tree.root_->height_, force_height(tree.root_));
    ASSERT_EQ(tree.size(), size);
}

TEST(AvlTreeTest, test_avl_tree_insert_big_range)
{
    AvlTreeBase<int> tree;
    const std::size_t size = 100000;

    std::vector<int> values(size);
    std::iota(values.begin(), values.end(), 1);

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(values.begin(), values.end(), g);

    for (int value : values)
    {
        tree.insert(new AvlTreeNode<int>(value));
    }

    ASSERT_EQ(tree.root_->leftmost()->value_, 1);
    ASSERT_EQ(tree.root_->rightmost()->value_, size);
    ASSERT_TRUE(tree.root_->height_ < static_cast<int>(1.440 * std::log2(size + 2) - 0.328));
    ASSERT_EQ(tree.root_->height_, force_height(tree.root_));
    ASSERT_EQ(tree.size(), size);
}

TEST(AvlTreeTest, test_avl_tree_erase)
{
    AvlTreeBase<int> tree;
    const std::size_t size = 1000;

    std::vector<int> values(size);
    std::iota(values.begin(), values.end(), 1);

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(values.begin(), values.end(), g);

    for (int value : values)
    {
        tree.insert(new AvlTreeNode<int>(value));
    }

    std::shuffle(values.begin(), values.end(), g);

    for (int i = 0; i < size / 2; ++i)
    {
        tree.erase(values[i]);
        ASSERT_EQ(tree.find(values[i]), nullptr);
    }
    std::vector<int> expected_values(values.begin() + size / 2, values.end());
    std::sort(expected_values.begin(), expected_values.end());

    for (auto val : expected_values)
    {
        ASSERT_EQ(tree.find(val)->value_, val);
    }

    ASSERT_EQ(tree.root_->leftmost()->value_, expected_values.front());
    ASSERT_EQ(tree.root_->rightmost()->value_, expected_values.back());
    ASSERT_TRUE(tree.root_->height_ < static_cast<int>(1.440 * std::log2(size + 2) - 0.328));
    ASSERT_EQ(tree.root_->height_, force_height(tree.root_));
    ASSERT_EQ(tree.size(), size - size / 2);
}

TEST(AvlTreeTest, test_avl_tree_erase_big_range)
{
    AvlTreeBase<int> tree;
    const std::size_t size = 100000;

    std::vector<int> values(size);
    std::iota(values.begin(), values.end(), 1);

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(values.begin(), values.end(), g);

    for (int value : values)
    {
        tree.insert(new AvlTreeNode<int>(value));
    }

    std::shuffle(values.begin(), values.end(), g);

    for (int i = 0; i < size / 2; ++i)
    {
        tree.erase(values[i]);
        ASSERT_EQ(tree.find(values[i]), nullptr);
    }

    std::vector<int> expected_values(values.begin() + size / 2, values.end());
    std::sort(expected_values.begin(), expected_values.end());

    for (auto val : expected_values)
    {
        ASSERT_EQ(tree.find(val)->value_, val);
    }

    ASSERT_EQ(tree.root_->leftmost()->value_, expected_values.front());
    ASSERT_EQ(tree.root_->rightmost()->value_, expected_values.back());
    ASSERT_TRUE(tree.root_->height_ <
                static_cast<int>(1.440 * std::log2(size - size / 2 + 2) - 0.328));
    ASSERT_EQ(tree.root_->height_, force_height(tree.root_));
    ASSERT_EQ(tree.size(), size - size / 2);

    tree.clear();

    ASSERT_EQ(tree.root_, nullptr);
    ASSERT_EQ(tree.number_of_nodes_, 0);
}
