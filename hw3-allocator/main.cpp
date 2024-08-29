#include <iostream>
#include <map>

#include "poolallocator.h"
#include "forwardlist.h"

int factorial(int n)
{
    if(n == 0 || n == 1)
    {
        return 1;
    }
    return n * factorial(n - 1);
}

int main()
{
    constexpr int poolSize = 10;
    std::map<int, int> map;
    for(int i = 0; i < poolSize; ++i)
    {
        map.emplace(i, factorial(i));
    }
    std::map<int, int, std::less<int>, PoolAllocator<std::pair<const int, int>, poolSize>> mapAlloc;
    for(int i = 0; i < poolSize; ++i)
    {
        mapAlloc.emplace(i, factorial(i));
    }
    for(const auto& pair : mapAlloc)
    {
        std::cout << pair.first << " " << pair.second << std::endl;
    }
    ForwardList<int> list;
    for(int i = 0; i < poolSize; ++i)
    {
        list.pushBack(i);
    }
    ForwardList<int, PoolAllocator<int, poolSize>> listAlloc;
    for(int i = 0; i < poolSize; ++i)
    {
        listAlloc.pushBack(i);
    }
    for(const auto& elem : listAlloc)
    {
        std::cout << elem << std::endl;
    }
    return 0;
}