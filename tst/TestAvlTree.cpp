#include "gtest/gtest.h"
#include <algorithm/AvlTree.hpp>
#include <unordered_set>
#include <vector>
#include <numeric>

TEST(AvlTreeTestSuite, TestPushFrontSmallRange)
{
	AvlTree<int> tree;

	for (int i = -5; i < 5; ++i)
	{
		ASSERT_EQ(*tree.insert(i).first, i);
		EXPECT_TRUE(tree.force_balance_check());
	}
}

TEST(AvlTreeTestSuite, TestSize)
{
	AvlTree<int> tree;

	EXPECT_EQ(tree.size(), 0);
	EXPECT_TRUE(tree.empty());

	for (int i = 1; i < 10000; ++i)
	{
		tree.insert(i);
		EXPECT_EQ(tree.size(), i);
		EXPECT_FALSE(tree.empty());
		EXPECT_TRUE(tree.force_balance_check());
	}

	tree.clear();
	EXPECT_EQ(tree.size(), 0);
	EXPECT_TRUE(tree.empty());
}

TEST(AvlTreeTestSuite, TestPushFrontMediumRange)
{
	AvlTree<int> tree;

	for (int i = -1000; i < 1000; ++i)
	{
		ASSERT_EQ(*tree.insert(i).first, i);
		EXPECT_TRUE(tree.force_balance_check());
	}
}

TEST(AvlTreeTestSuite, TestPushFrontBigRange)
{
	AvlTree<int> tree;

	for (int i = -10000; i < 10000; ++i)
	{
		ASSERT_EQ(*tree.insert(i).first, i);
		EXPECT_TRUE(tree.force_balance_check());
	}
}

TEST(AvlTreeTestSuite, TestPushBackSmallRange)
{
	AvlTree<int> tree;

	for (int i = 5; i >= -5; --i)
	{
		ASSERT_EQ(*tree.insert(i).first, i);
		EXPECT_TRUE(tree.force_balance_check());
	}
}

TEST(AvlTreeTestSuite, TestPushBackMediumRange)
{
	AvlTree<int> tree;

	for (int i = 1000; i >= -1000; --i)
	{
		ASSERT_EQ(*tree.insert(i).first, i);
		EXPECT_TRUE(tree.force_balance_check());
	}
}

TEST(AvlTreeTestSuite, TestPushBackBigRange)
{
	AvlTree<int> tree;

	for (int i = 10000; i >= -10000; --i)
	{
		ASSERT_EQ(*tree.insert(i).first, i);
		EXPECT_TRUE(tree.force_balance_check());
	}
}

TEST(AvlTreeTestSuite, TestFindSmall)
{
	AvlTree<int> tree;

	std::vector<int> range(20);
	std::iota(range.begin(), range.end(), -10);
	std::random_shuffle(range.begin(), range.end());

	for (auto &item: range)
	{
		auto inserted = tree.insert(item);

		//std::cerr << *tree.begin() << ' ';
		//std::cerr << std::endl;

		auto found = tree.find(item);
		ASSERT_EQ(found, inserted.first);
		ASSERT_EQ(*found, item);
		EXPECT_TRUE(tree.force_balance_check());
	}

	// -6 0 1 5 4 6

	std::random_shuffle(range.begin(), range.end());

	for (auto &item: range)
	{
		ASSERT_EQ(*tree.find(item), item);
		EXPECT_TRUE(tree.force_balance_check());
	}
}

TEST(AvlTreeTestSuite, TestFindMedium)
{
	AvlTree<int> tree;

	std::vector<int> range(2000);
	std::iota(range.begin(), range.end(), -1000);
	std::random_shuffle(range.begin(), range.end());

	for (auto &item: range)
	{
		auto inserted = tree.insert(item);
		ASSERT_EQ(tree.find(item), inserted.first);
		ASSERT_EQ(*tree.find(item), item);
		EXPECT_TRUE(tree.force_balance_check());
	}

	std::random_shuffle(range.begin(), range.end());

	for (auto &item: range)
	{
		ASSERT_EQ(*tree.find(item), item);
		EXPECT_TRUE(tree.force_balance_check());
	}
}

TEST(AvlTreeTestSuite, TestFindBig)
{
	AvlTree<int> tree;

	std::vector<int> range(20000);
	std::iota(range.begin(), range.end(), -10000);
	std::random_shuffle(range.begin(), range.end());

	for (auto &item: range)
	{
		auto inserted = tree.insert(item);
		ASSERT_EQ(tree.find(item), inserted.first);
		ASSERT_EQ(*tree.find(item), item);
		EXPECT_TRUE(tree.force_balance_check());
	}

	std::random_shuffle(range.begin(), range.end());

	for (auto &item: range)
	{
		ASSERT_EQ(*tree.find(item), item);
		EXPECT_TRUE(tree.force_balance_check());
	}
}

TEST(AvlTreeTestSuite, TestPopBack)
{
	AvlTree<int> tree;

	for (int i = -2; i <= 2; ++i)
	{
		tree.insert(i);
		EXPECT_TRUE(tree.force_balance_check());
	}

	int size = tree.size();

	for (int i = 2; i >= -2; --i)
	{
		tree.erase(i);
		size--;
		EXPECT_EQ(tree.size(), size);
		EXPECT_TRUE(tree.force_balance_check());
		EXPECT_EQ(tree.find(i), tree.end());
	}
}

TEST(AvlTreeTestSuite, TestPopBackBig)
{
	AvlTree<int> tree;

	for (int i = -2000; i <= 2000; ++i)
	{
		tree.insert(i);
		EXPECT_TRUE(tree.force_balance_check());
	}

	int size = tree.size();
	for (int i = 2000; i >= -2000; --i)
	{
		tree.erase(i);
		size--;
		EXPECT_EQ(tree.size(), size);
		EXPECT_TRUE(tree.force_balance_check());
		EXPECT_EQ(tree.find(i), tree.end());
	}
}

TEST(AvlTreeTestSuite, TestPopFront)
{
	AvlTree<int> tree;

	for (int i = 2; i >= -2; --i)
	{
		tree.insert(i);
		EXPECT_TRUE(tree.force_balance_check());
	}

	int size = tree.size();
	for (int i = -2; i <= 2; ++i)
	{
		tree.erase(i);
		size--;
		EXPECT_EQ(tree.size(), size);
		EXPECT_TRUE(tree.force_balance_check());
		EXPECT_EQ(tree.find(i), tree.end());
	}
}

TEST(AvlTreeTestSuite, TestPopFrontBig)
{
	AvlTree<int> tree;

	for (int i = 2000; i >= -2000; --i)
	{
		tree.insert(i);
		EXPECT_TRUE(tree.force_balance_check());
	}

	int size = tree.size();
	for (int i = -2000; i <= 2000; ++i)
	{
		tree.erase(i);
		size--;
		EXPECT_EQ(tree.size(), size);
		EXPECT_TRUE(tree.force_balance_check());
		EXPECT_EQ(tree.find(i), tree.end());
	}
}

TEST(AvlTreeTestSuite, TestEraseSmall)
{
	AvlTree<int> tree;

	std::vector<int> range(20);
	std::iota(range.begin(), range.end(), -10);
	std::random_shuffle(range.begin(), range.end());

	for (auto &item: range)
	{
		auto inserted = tree.insert(item);
		ASSERT_EQ(tree.find(item), inserted.first);
		ASSERT_EQ(*tree.find(item), item);
		EXPECT_TRUE(tree.force_balance_check());
	}

	std::random_shuffle(range.begin(), range.end());

	for (auto &item: range)
	{
		tree.erase(item);
		ASSERT_EQ(tree.find(item), tree.end());
		EXPECT_TRUE(tree.force_balance_check());
	}
}

TEST(AvlTreeTestSuite, TestEraseMedium)
{
	AvlTree<int> tree;

	std::vector<int> range(2000);
	std::iota(range.begin(), range.end(), -1000);
	std::random_shuffle(range.begin(), range.end());

	for (auto &item: range)
	{
		auto inserted = tree.insert(item);
		ASSERT_EQ(tree.find(item), inserted.first);
		ASSERT_EQ(*tree.find(item), item);
		EXPECT_TRUE(tree.force_balance_check());
	}

	std::random_shuffle(range.begin(), range.end());

	for (auto &item: range)
	{
		tree.erase(item);
		ASSERT_EQ(tree.find(item), tree.end());
		EXPECT_TRUE(tree.force_balance_check());
	}
}

TEST(AvlTreeTestSuite, TestEraseMediumIterator)
{
	AvlTree<int> tree;

	std::vector<int> range(2000);
	std::iota(range.begin(), range.end(), -1000);
	std::random_shuffle(range.begin(), range.end());

	for (auto &item: range)
	{
		auto inserted = tree.insert(item);
		ASSERT_EQ(tree.find(item), inserted.first);
		ASSERT_EQ(*tree.find(item), item);
		EXPECT_TRUE(tree.force_balance_check());
	}

	std::random_shuffle(range.begin(), range.end());

	for (auto &item: range)
	{
		tree.erase(tree.find(item));
		ASSERT_EQ(tree.find(item), tree.end());
		EXPECT_TRUE(tree.force_balance_check());
	}
}