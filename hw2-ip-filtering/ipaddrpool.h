#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <array>

using IpAddr = std::array<uint8_t, 4>;

class IpAddrPool
{
public:
    IpAddrPool() = default;
    ~IpAddrPool() = default;
    void pushBack(const std::array<uint8_t,4>& addr);
    void pushBack(const uint8_t n0,
                  const uint8_t n1,
                  const uint8_t n2,
                  const uint8_t n3);
    void sortDescending();
    IpAddrPool filter(std::initializer_list<uint8_t> filterValuesList);
    IpAddrPool filterAny(uint8_t filterValue);
    void print();

private:
    std::vector<IpAddr> m_data;
};