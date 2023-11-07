#include <chrono>
#include <iostream>
#include <ranges>
#include <vector>

template <typename T, class tree_t>
std::ostream &benchmark_insert(const std::vector<T> &test_case, std::ostream &out = std::cerr)
{
    std::vector<T> test(test_case);
    tree_t tree;

    auto start = std::chrono::high_resolution_clock::now();
    for (auto i : test)
    {
        tree.insert(i);
    }
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;
    out << "Insertion random objects: " << elapsed.count() * 1000 << std::endl;

    tree.clear();

    std::sort(test.begin(), test.end());

    start = std::chrono::high_resolution_clock::now();
    for (auto i : test)
    {
        tree.insert(i);
    }
    end = std::chrono::high_resolution_clock::now();

    elapsed = end - start;
    out << "Insertion increasing objects: " << elapsed.count() * 1000 << std::endl;

    tree.clear();

    std::sort(test.rbegin(), test.rend());

    start = std::chrono::high_resolution_clock::now();
    for (auto i : test)
    {
        tree.insert(i);
    }
    end = std::chrono::high_resolution_clock::now();

    elapsed = end - start;
    out << "Insertion decreasing objects: " << elapsed.count() * 1000 << std::endl;

    return out;
}

template <typename T, class tree_t>
std::ostream &benchmark_erase(const std::vector<T> &test_case, std::ostream &out = std::cerr)
{
    std::vector<T> test(test_case);
    tree_t tree;
    for (auto i : test)
    {
        tree.insert(i);
    }

    auto start = std::chrono::high_resolution_clock::now();
    for (auto i : test)
    {
        tree.erase(i);
    }
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;
    out << "Erase random objects: " << elapsed.count() * 1000 << std::endl;

    tree.clear();
    for (auto i : test)
    {
        tree.insert(i);
    }

    std::sort(test.begin(), test.end());

    start = std::chrono::high_resolution_clock::now();
    for (auto i : test)
    {
        tree.erase(i);
    }
    end = std::chrono::high_resolution_clock::now();

    elapsed = end - start;
    out << "Erase smallest: " << elapsed.count() * 1000 << std::endl;

    tree.clear();
    for (auto i : test)
    {
        tree.insert(i);
    }

    std::sort(test.rbegin(), test.rend());

    start = std::chrono::high_resolution_clock::now();
    for (auto i : test)
    {
        tree.erase(i);
    }
    end = std::chrono::high_resolution_clock::now();

    elapsed = end - start;
    out << "Erase largest: " << elapsed.count() * 1000 << std::endl;

    return out;
}

template <typename T, class tree_t>
std::ostream &benchmark_find(const std::vector<T> &test_case, std::ostream &out = std::cerr)
{
    std::vector<T> test(test_case);
    tree_t tree;
    for (auto i : test)
    {
        tree.insert(i);
    }

    auto start = std::chrono::high_resolution_clock::now();
    for (auto i : test)
    {
        tree.find(i);
    }
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;
    out << "Find random objects: " << elapsed.count() * 1000 << std::endl;

    tree.clear();
    for (auto i : test)
    {
        tree.insert(i);
    }

    std::sort(test.begin(), test.end());

    start = std::chrono::high_resolution_clock::now();
    for (auto i : test)
    {
        tree.find(i);
    }
    end = std::chrono::high_resolution_clock::now();

    elapsed = end - start;
    out << "Find increasing sequence: " << elapsed.count() * 1000 << std::endl;

    tree.clear();
    for (auto i : test)
    {
        tree.insert(i);
    }

    std::sort(test.rbegin(), test.rend());

    start = std::chrono::high_resolution_clock::now();
    for (auto i : test)
    {
        tree.find(i);
    }
    end = std::chrono::high_resolution_clock::now();

    elapsed = end - start;
    out << "Find decreasing sequence: " << elapsed.count() * 1000 << std::endl;

    tree.clear();
    for (auto i : test)
    {
        tree.insert(i);
    }

    std::sort(test.begin(), test.end());

    auto fake_value = test.back() + 1;

    start = std::chrono::high_resolution_clock::now();
    for (auto i : test)
    {
        tree.find(fake_value);
    }
    end = std::chrono::high_resolution_clock::now();

    elapsed = end - start;
    out << "Find not existing element: " << elapsed.count() * 1000 << std::endl;

    return out;
}

template <typename T, class tree_t>
std::ostream &benchmark_traverse(const std::vector<T> &test_case, std::ostream &out = std::cerr)
{
    std::vector<T> test(test_case);
    tree_t tree;
    for (auto i : test)
    {
        tree.insert(i);
    }

    auto start = std::chrono::high_resolution_clock::now();
    int k = 0;
    for (auto i : tree)
    {
        ++k;
    }
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;
    out << "Traverse forward: " << elapsed.count() * 1000 << std::endl;

    start = std::chrono::high_resolution_clock::now();
    k = 0;
    for (auto i : tree | std::views::reverse)
    {
        ++k;
    }
    end = std::chrono::high_resolution_clock::now();

    elapsed = end - start;
    out << "Traverse backward: " << elapsed.count() * 1000 << std::endl;
    return out;
}
