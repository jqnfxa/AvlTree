#include <algorithm>
#include <benchmark/benchmark.hpp>
#include <implementation/avl_tree.hpp>
#include <iostream>
#include <list>
#include <numeric>
#include <random>
#include <set>

void run_benchmark(int size)
{
    std::vector<int> test(size);
    std::iota(test.begin(), test.end(), 1);
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(test.begin(), test.end(), g);
    std::shuffle(test.begin(), test.end(), g);

    std::cerr << "n: " << size << std::endl;

    std::cerr << "Avl tree" << std::endl;
    benchmark_insert<int, avl_tree<int>>(test);
    benchmark_erase<int, avl_tree<int>>(test);
    benchmark_find<int, avl_tree<int>>(test);
    benchmark_traverse<int, avl_tree<int>>(test);
    std::cerr << std::endl;

    std::cerr << "std::set" << std::endl;
    benchmark_insert<int, std::set<int>>(test);
    benchmark_erase<int, std::set<int>>(test);
    benchmark_find<int, std::set<int>>(test);
    benchmark_traverse<int, std::set<int>>(test);
    std::cerr << std::endl;
}

int main()
{
    for (int i = 1e3; i <= 1e5; i += 1e3)
    {
        run_benchmark(i);
    }
    return 0;
}
