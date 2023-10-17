#include "gtest/gtest.h"
#include <algorithm/Node.hpp>

TEST(NodeTestSuite, TestNodeValues)
{
	Node<int> right(2);
	Node<int> left(3);

	Node<int> node(1, &left, &right);

	EXPECT_EQ(node.value, 1);
	EXPECT_EQ(node.left->value, 3);
	EXPECT_EQ(node.right->value, 2);
}

TEST(NodeTestSuite, TestNodeBalanceFactor)
{
	using Node = Node<int>;

	Node lll(1);
	Node ll(2, &lll);
	Node l(3, &ll);

	Node r(1);

	Node node(1, &l, &r);

	EXPECT_EQ(node.balance_factor(), -2);
	EXPECT_EQ(node.height(), 4);

	Node rr(4);
	r.right = &rr;
	r.update_height();

	EXPECT_EQ(node.balance_factor(), -1);
}