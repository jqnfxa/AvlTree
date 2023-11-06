#include "gtest/gtest.h"
#include <algorithm/avl_tree.hpp>
#include <algorithm>
#include <memory/memory.hpp>
#include <random>
#include <ranges>

TEST(AvlIterableTreeTest, default_constructor)
{
    avl_tree<int> tree;
    ASSERT_EQ(0, tree.size());
}

TEST(AvlIterableTreeTest, test_avl_tree_copy_constructor)
{
    avl_tree<int> tree;

    tree.insert(10);
    tree.insert(20);
    tree.insert(50);

    ASSERT_EQ(*std::next(tree.begin()), 20);
    ASSERT_EQ(*tree.begin(), 10);
    ASSERT_EQ(*std::prev(tree.end()), 50);

    avl_tree<int> copy = tree;

    ASSERT_EQ(*copy.begin(), 10);
    ASSERT_EQ(*std::next(copy.begin()), 20);
    ASSERT_EQ(*std::prev(copy.end()), 50);

    copy.clear();

    ASSERT_EQ(*std::next(tree.begin()), 20);
    ASSERT_EQ(*tree.begin(), 10);
    ASSERT_EQ(*std::prev(tree.end()), 50);
}

TEST(AvlIterableTreeTest, test_avl_tree_move_constructor)
{
    avl_tree<int> tree;

    tree.insert(10);
    tree.insert(20);
    tree.insert(50);

    ASSERT_EQ(*std::next(tree.begin()), 20);
    ASSERT_EQ(*tree.begin(), 10);
    ASSERT_EQ(*std::prev(tree.end()), 50);

    avl_tree<int> copy(std::move(tree));

    ASSERT_EQ(*std::next(copy.begin()), 20);
    ASSERT_EQ(*copy.begin(), 10);
    ASSERT_EQ(*std::prev(copy.end()), 50);
}

TEST(AvlIterableTreeTest, test_traverse)
{
    avl_tree<int> tree;

    const std::size_t size = 10000;

    std::vector<int> values(size);
    std::iota(values.begin(), values.end(), 1);

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(values.begin(), values.end(), g);

    for (int value : values)
    {
        tree.insert(value);
    }

    std::vector<int> forward;

    for (auto item : tree)
    {
        forward.push_back(item);
    }

    ASSERT_TRUE(std::is_sorted(forward.begin(), forward.end()));

    forward.clear();

    for (auto it = tree.find(size / 2 + 7); it != tree.end(); ++it)
    {
        forward.push_back(*it);
    }
    ASSERT_TRUE(std::is_sorted(forward.begin(), forward.end()));

    std::vector<int> backward = std::move(forward);
    backward.clear();

    for (auto item : tree | std::views::reverse)
    {
        backward.push_back(item);
    }

    std::reverse(backward.begin(), backward.end());

    ASSERT_TRUE(std::is_sorted(backward.begin(), backward.end()));

    backward.clear();

    for (auto it = std::reverse_iterator(tree.find(size / 2 + 7)); it != tree.rend(); ++it)
    {
        backward.push_back(*it);
    }
    ASSERT_TRUE(std::is_sorted(backward.rbegin(), backward.rend()));
}

TEST(AvlIterableTreeTest, test_insert)
{
    avl_tree<int> tree;

    const std::size_t size = 6000;

    std::vector<int> values(size);
    std::iota(values.begin(), values.end(), 2000);

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(values.begin(), values.end(), g);

    for (int value : values)
    {
        auto [iterator, status] = tree.insert(value);

        ASSERT_EQ(iterator, tree.find(value));
        ASSERT_EQ(status, true);
        ASSERT_EQ(*tree.find(value), value);
    }

    for (int value : values)
    {
        auto [iterator, status] = tree.insert(value);

        ASSERT_EQ(iterator, tree.find(value));
        ASSERT_EQ(status, false);
        ASSERT_EQ(*tree.find(value), value);
    }

    std::vector<int> forward;

    for (auto item : tree)
    {
        forward.push_back(item);
    }

    ASSERT_TRUE(std::is_sorted(forward.begin(), forward.end()));

    forward.clear();

    for (auto it = tree.find(size / 2 + 7); it != tree.end(); ++it)
    {
        forward.push_back(*it);
    }
    ASSERT_TRUE(std::is_sorted(forward.begin(), forward.end()));

    std::vector<int> backward = std::move(forward);
    backward.clear();

    for (auto item : tree | std::views::reverse)
    {
        backward.push_back(item);
    }

    std::reverse(backward.begin(), backward.end());

    ASSERT_TRUE(std::is_sorted(backward.begin(), backward.end()));

    backward.clear();

    for (auto it = std::reverse_iterator(tree.find(size / 2 + 7)); it != tree.rend(); ++it)
    {
        backward.push_back(*it);
    }
    ASSERT_TRUE(std::is_sorted(backward.rbegin(), backward.rend()));
}

TEST(AvlIterableTreeTest, test_erase)
{
    avl_tree<int> tree;

    const std::size_t size = 6000;

    std::vector<int> values(size);
    std::iota(values.begin(), values.end(), 2000);

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(values.begin(), values.end(), g);

    for (int value : values)
    {
        tree.insert(value);
    }
    for (int i = 0; i < 10000; ++i)
    {
        auto expected = std::next(tree.find(i));
        tree.erase(i);

        if (i >= 2000 && i <= 5555)
        {
            ASSERT_EQ(tree.begin(), expected);
        }
        else
        {
            ASSERT_EQ(tree.find(i), tree.end());
        }
    }

    ASSERT_EQ(tree.size(), 0);

    std::sort(values.begin(), values.end());

    for (int value : values)
    {
        tree.insert(value);
    }
    for (int i = 0; i < 1000; ++i)
    {
        tree.erase_largest();
    }
    for (int i = 7000; i < 8000; ++i)
    {
        ASSERT_EQ(tree.find(i), tree.end());
    }
    ASSERT_EQ(std::accumulate(tree.begin(), tree.end(), 0ull),
              std::accumulate(tree.rbegin(), tree.rend(), 0ull));
}
