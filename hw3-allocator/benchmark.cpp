#include <chrono>
#include <list>
#include <iostream>
#include "poolallocator.h"

int main()
{
    constexpr int iterNum = 50000;

    std::list<int> defaultListDefaultAlloc;
    auto t1 = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < iterNum; ++i)
    {
        defaultListDefaultAlloc.emplace_back(i);
    }
    auto t2 = std::chrono::high_resolution_clock::now();
    auto dt21 = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
    std::cout << "Default allocator time = " << dt21 << " ns" << std::endl;

    std::list<int, PoolAllocator<int, iterNum>> defaultListCustomAlloc;
    auto t3 = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < iterNum; ++i)
    {
        defaultListCustomAlloc.emplace_back(i);
    }
    auto t4 = std::chrono::high_resolution_clock::now();
    auto dt43 = std::chrono::duration_cast<std::chrono::nanoseconds>(t4 - t3).count();
    std::cout << "Custom allocator time = " << dt43 << " ns" << std::endl;
    
    double relDiff = 100.0 * ((double)(dt21 - dt43)) / ((double)(dt21));
    std::cout << "Relative difference = " << relDiff << "%" << std::endl;
}