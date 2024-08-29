#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <array>

struct IpAddr
{
    static inline constexpr size_t length = 4;
    IpAddr(uint8_t n0, uint8_t n1, uint8_t n2, uint8_t n3) : data{n0, n1, n2, n3}
    {}
    std::array<uint8_t, length> data;
};

class IpAddrPool
{
public:
    IpAddrPool() = default;
    ~IpAddrPool() = default;
    void pushBack(const IpAddr& addr);
    template<typename... Ts> void emplaceBack(Ts... args)
    {
        m_data.emplace_back(args...);
    }
    void sortDescending();
    IpAddrPool filter(const std::initializer_list<uint8_t>& filterValuesList);
    IpAddrPool filterAny(uint8_t filterValue);
    void print();

private:
    std::vector<IpAddr> m_data;
};