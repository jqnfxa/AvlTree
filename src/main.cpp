#include <iostream>
#include <numeric>
#include <random>
#include <ranges>
#include <set>
#include "profiler/Timer.hpp"
#include "algorithm/AvlTree.hpp"

// TODO tests for iterators
// TODO tests for tree insert
// TODO tests for tree find
// TODO tests for tree erase
// TODO speed up
// TODO std::side_view compability
// TODO concepts for templates

template <typename Iterator>
std::ostream &print(std::ostream &out, Iterator begin, Iterator end)
{
	for (auto it = begin; it != end; ++it)
	{
		out << *it << ' ';

		if (it != std::prev(end))
		{
			out << ' ';
		}
	}
	return out << std::endl;
}

void benchmark_set(const std::vector<int> &test, const std::vector<int> &find, const std::vector<int> &erase)
{
	std::cerr << "Set\n";
	std::set<int, std::greater<>> tree;
	std::vector<int> res;
	res.reserve(test.size());

	{
		Timer timer(std::cerr, "insert " + std::to_string(test.size()));

		for (auto &item : test)
		{
			tree.insert(item);
		}
	}
	{
		Timer timer(std::cerr, "find " + std::to_string(find.size()));

		for (auto &item : find)
		{
			if (*tree.find(item) == item)
			{
				res.push_back(item);
			}
		}
	}
	int k;
	{
		Timer timer(std::cerr, "traverse forward " + std::to_string(tree.size()));

		for (auto i : tree)
		{
			if (i > 15)
			{
				k++;
			}
		}
	}
	std::cerr << k << std::endl;
	{
		Timer timer(std::cerr, "traverse backward " + std::to_string(tree.size()));

		for (int it : std::ranges::reverse_view(tree))
		{
			if (it > 15)
			{
				k++;
			}
		}
	}
	std::cerr << k << std::endl;
	{
		Timer timer(std::cerr, "erase " + std::to_string(erase.size()));

		for (auto &item : erase)
		{
			tree.erase(item);
		}
	}

	std::cerr << "\n\n";
}

void benchmark_naked_tree(const std::vector<int> &test, const std::vector<int> &find, const std::vector<int> &erase)
{
	std::cerr << "Naked tree\n";

	AvlTreeBase<int, std::greater<>> tree;
	std::vector<int> res;
	res.reserve(test.size());

	{
		Timer timer(std::cerr, "insert " + std::to_string(test.size()));

		for (auto &item : test)
		{
			tree.insert(new AvlTreeNode<int>(item));
		}
	}
	{
		Timer timer(std::cerr, "find " + std::to_string(find.size()));

		for (auto &item : find)
		{
			if (tree.find(item)->value_ == item)
			{
				res.push_back(item);
			}
		}
	}
	{
		Timer timer(std::cerr, "erase " + std::to_string(erase.size()));

		for (auto &item : erase)
		{
			tree.erase(item);
		}
	}

	std::cerr << "\n\n";
}

void benchmark_tree(const std::vector<int> &test, const std::vector<int> &find, const std::vector<int> &erase)
{
	std::cerr << "Tree\n";

	AvlTree<int, std::greater<>> tree;
	std::vector<int> res;
	res.reserve(test.size());

	{
		Timer timer(std::cerr, "insert " + std::to_string(test.size()));

		for (auto &item : test)
		{
			tree.insert(item);
		}
	}
	{
		Timer timer(std::cerr, "find " + std::to_string(find.size()));

		for (auto &item : find)
		{
			if (*tree.find(item) == item)
			{
				res.push_back(item);
			}
		}
	}
	int k = 0;
	{
		Timer timer(std::cerr, "traverse forward " + std::to_string(tree.size()));

		for (auto i : tree)
		{
			if (i > 15)
			{
				k++;
			}
		}
	}
	std::cerr << k << std::endl;
	{
		Timer timer(std::cerr, "traverse backward " + std::to_string(test.size()));

		for (auto it = tree.rbegin(); it != tree.rend(); ++it)
		{
			if (*it > 15)
			{
				k++;
			}
		}
	}
	std::cerr << k << std::endl;
	{
		Timer timer(std::cerr, "erase " + std::to_string(erase.size()));

		for (auto &item : erase)
		{
			tree.erase(item);
		}
	}

	std::cerr << "\n\n";
}

int main()
{
	int size = 2e6 + 2;

	std::vector<int> range(size);
	std::iota(range.begin(), range.end(), -size / 2);

	std::shuffle(range.begin(), range.end(), std::mt19937(std::random_device()()));

	for (int i = 0; i < size * 2; ++i)
	{
		range.push_back(range[rand() % range.size()]);
	}

	std::shuffle(range.begin(), range.end(), std::mt19937(std::random_device()()));

	std::vector<int> find = range;

	std::shuffle(find.begin(), find.end(), std::mt19937(std::random_device()()));

	std::vector<int> erase = find;

	std::shuffle(erase.begin(), erase.end(), std::mt19937(std::random_device()()));

	for (int i = 0; i < size * 2; ++i)
	{
		erase.push_back((2 * size + 1) * (-1) * (rand() % 2));
	}

	std::shuffle(erase.begin(), erase.end(), std::mt19937(std::random_device()()));

	benchmark_set(range, find, erase);
	benchmark_naked_tree(range, find, erase);
	benchmark_tree(range, find, erase);

	return 0;
}
