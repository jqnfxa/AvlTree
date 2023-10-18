#include "gtest/gtest.h"
#include <algorithm/AvlTree.hpp>

TEST(AvlTreeTestSuite, TestSize)
{
	AvlTree<int> tree;

	EXPECT_EQ(tree.size(), 0);
	EXPECT_TRUE(tree.empty());

	for (int i = 1; i < 100; ++i)
	{
		tree.insert(i);
		EXPECT_EQ(tree.size(), i);
		EXPECT_FALSE(tree.empty());
	}

	tree.clear();
	EXPECT_EQ(tree.size(), 0);
	EXPECT_TRUE(tree.empty());
}

TEST(AvlTreeTestSuite, TestInsertSmall)
{
	AvlTree<int> tree;

	for (int i = -5; i < 5; ++i)
	{
		ASSERT_EQ(*tree.insert(i), i);
	}
}

TEST(AvlTreeTestSuite, TestInsertMedium)
{
	AvlTree<int> tree;

	for (int i = -1000; i < 1000; ++i)
	{
		ASSERT_EQ(*tree.insert(i), i);
	}
}


TEST(AvlTreeTestSuite, TestInsertBig)
{
	AvlTree<int> tree;

	for (int i = -10000; i < 10000; ++i)
	{
		ASSERT_EQ(*tree.insert(i), i);
	}
}

TEST(AvlTreeTestSuite, TestFindSmall)
{
	AvlTree<int> tree;

	for (int i = -100; i < 100; ++i)
	{
		auto inserted = tree.insert(i);
		ASSERT_EQ(tree.find(i), inserted);
		ASSERT_EQ(*tree.find(i), i);
	}
}

TEST(AvlTreeTestSuite, TestFindMedium)
{
	AvlTree<int> tree;

	for (int i = -1000; i < 1000; ++i)
	{
		auto inserted = tree.insert(i);
		ASSERT_EQ(tree.find(i), inserted);
		ASSERT_EQ(*tree.find(i), i);
	}
}

TEST(AvlTreeTestSuite, TestFindBig)
{
	AvlTree<int> tree;

	for (int i = -10000; i < 10000; ++i)
	{
		auto inserted = tree.insert(i);
		ASSERT_EQ(tree.find(i), inserted);
		ASSERT_EQ(*tree.find(i), i);
	}
}
/*
TEST(AvlTreeTestSuite, TestEraseSmall)
{
	AvlTree<int> tree;

	for (int i = -10; i < 10; ++i)
	{
		tree.insert(i);
	}

	auto prev_size = tree.size();

	for (int i = 9; i >= -10; --i)
	{
		tree.erase(i);

		// check size
		EXPECT_EQ(tree.size() + 1, prev_size);

		// check other elements
		int c = 0;

		for (int j = 9; j >= -10; --j)
		{
			auto *ptr = tree.find(j);

			if (ptr != nullptr)
			{
				EXPECT_EQ(ptr->value, j);

				c++;
			}
		}

		ASSERT_EQ(tree.size(), c);

		prev_size = tree.size();
	}
}

TEST(AvlTreeTestSuite, TestEraseMedium)
{
	AvlTree<int> tree;

	for (int i = -100; i < 100; ++i)
	{
		tree.insert(i);
	}

	auto prev_size = tree.size();

	for (int i = 99; i >= -100; --i)
	{
		tree.erase(i);

		// check size
		EXPECT_EQ(tree.size() + 1, prev_size);

		// check other elements
		int c = 0;

		for (int j = 99; j >= -100; --j)
		{
			auto *ptr = tree.find(j);

			if (ptr != nullptr)
			{
				EXPECT_EQ(ptr->value, j);

				c++;
			}
		}

		ASSERT_EQ(tree.size(), c);

		prev_size = tree.size();
	}
}

TEST(AvlTreeTestSuite, TestEraseBig)
{
	AvlTree<int> tree;

	for (int i = -1000; i < 1000; ++i)
	{
		tree.insert(i);
	}

	auto prev_size = tree.size();

	for (int i = 999; i >= -1000; --i)
	{
		tree.erase(i);

		// check size
		EXPECT_EQ(tree.size() + 1, prev_size);

		// check other elements
		int c = 0;

		for (int j = 999; j >= -1000; --j)
		{
			auto *ptr = tree.find(j);

			if (ptr != nullptr)
			{
				EXPECT_EQ(ptr->value, j);

				c++;
			}
		}

		ASSERT_EQ(tree.size(), c);

		prev_size = tree.size();
	}
}*/