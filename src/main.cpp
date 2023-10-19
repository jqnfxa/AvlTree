#include <iostream>
#include <numeric>
#include <random>
#include <set>
#include "profiler/Timer.hpp"
#include "algorithm/AvlTree.hpp"

// TODO reverse iterators
// TODO remove inheritance?
// TODO tests for iterators
// TODO test for const iterators
// TODO tests for tree insert
// TODO tests for tree find
// TODO implement tree erase
// TODO tests for tree erase
// TODO what should we do with erase function? How AvlTree will have access to iterator node?
// TODO const iterators are same as default iterators?

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

int main()
{
	AvlTree<std::string> tree;

	std::string beautiful_string = "fuck you";

	tree.insert(beautiful_string);
	tree.insert(std::string("all"));

	print(std::cerr, tree.begin(), tree.end());

	//std::string str = *tree.end();
	//std::cout << str;

	/*
	std::set<int> k;
	k.insert(1);


	auto begin = k.begin();
	auto prev_begin = std::prev(begin);

	std::cout << std::distance(prev_begin, begin) << '\n';
	std::cout << std::distance(prev_begin, k.end()) << '\n';

	auto end = k.end();
	auto next_end = std::next(end);

	std::cout << std::distance(end, next_end) << '\n';

	std::cout << std::distance(k.rend(), k.end()) << '\n';
*/
	/*
	AvlTree<int> tree;

	int size = 1e6;
	//std::vector<int> range = {13, 35, 38, 95, 54, 79, 67};

	//  = {13, 35, 38, 95, 54, 79, 67}; //
	std::vector<int> range(size);
	std::iota(range.begin(), range.end(), 1);
	std::shuffle(range.begin(), range.end(), std::mt19937(std::random_device()()));
	std::shuffle(range.begin(), range.end(), std::mt19937(std::random_device()()));

	{
		Timer timer(std::cerr, "insert " + std::to_string(size));

		for(auto &item : range)
		{
			tree.insert(item);

			//print(std::cerr, tree.begin(), tree.end());
		}
	}

	std::vector<int> values;
	values.reserve(size);
	{
		Timer timer(std::cerr, "find " + std::to_string(size));

		for(auto &item : range)
		{
			values.push_back(*tree.find(item));
		}
	}
	{
		Timer timer(std::cerr, "erase " + std::to_string(size));

		for(auto &item : range)
		{
			tree.erase(item);

			//print(std::cerr, tree.begin(), tree.end());
		}
	}*/
	return 0;
}
