#include "gtest/gtest.h"
#include <algorithm/AvlTreeNode.hpp>

TEST(NodeTestSuite, TestNodeValues)
{
	using Node = AvlTreeNode<int>;

	Node right(2);
	Node left(3);

	Node node(1, &left, &right);

	EXPECT_EQ(node.value_, 1);
	EXPECT_EQ(*node.value(), 1);
	EXPECT_EQ(node.left()->value_, 3);
	EXPECT_EQ(*node.left()->value(), 3);
	EXPECT_EQ(*node.right()->value(), 2);
	EXPECT_EQ(node.left()->height_, 1);
	EXPECT_EQ(node.right()->height_, 1);
	EXPECT_EQ(node.height_, 2);
}

TEST(NodeTestSuite, TestNodeBalanceFactor)
{
	using Node = AvlTreeNode<int>;

	Node lll(1);
	Node ll(2, &lll);
	Node l(3, &ll);

	Node r(1);

	Node node(1, &l, &r);

	EXPECT_EQ(node.balance_factor(), -2);
	EXPECT_EQ(r.height_, 1);
	EXPECT_EQ(l.height_, 3);
	EXPECT_EQ(ll.height_, 2);
	EXPECT_EQ(lll.height_, 1);
	EXPECT_EQ(node.height_, 4);

	Node rr(4);
	r.right_ = &rr;
	r.right_->parent_ = &r;
	r.iterative_height_update();

	EXPECT_EQ(node.balance_factor(), -1);
	EXPECT_EQ(r.height_, 2);
}