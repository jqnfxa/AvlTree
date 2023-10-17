#include <iostream>
#include <numeric>
#include <random>
#include <set>
#include "profiler/Timer.hpp"
#include "algorithm/AvlTree.hpp"

// TODO end plug
// TODO smallest node
// TODO greatest node
// TODO end plug contain nullptr, but previous value is greatest

// TODO reverse iterators?
// TODO take AvlTree in iterator constructor?

template <typename Iterator>
std::ostream &print(std::ostream &out, Iterator begin, Iterator end)
{
	for (auto it = begin; it != end; ++it)
	{
		out << *it << ' ';

		/*if (it != std::prev(end))
		{
			out << ' ';
		}*/
	}
	return out << std::endl;
}

int main()
{
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
	}
	return 0;
}