#include "ipaddrpool.h"

void IpAddrPool::pushBack(const std::array<uint8_t,4>& addr)
{
    m_data.push_back(addr);
}

void IpAddrPool::pushBack(const uint8_t n0,
                            const uint8_t n1,
                            const uint8_t n2,
                            const uint8_t n3)
{
    m_data.push_back({n0, n1, n2, n3});
}

void IpAddrPool::sortDescending()
{
    std::sort(m_data.begin(), m_data.end(), [](const IpAddr& lhs, const IpAddr& rhs){
        for(int i = 0; i < 4; ++i)
        {
            if(lhs[i] > rhs[i])
            {
                return true;
            }
            else if(lhs[i] < rhs[i])
            {
                return false;
            }
        }
        return false;
    });
}

IpAddrPool IpAddrPool::filter(std::initializer_list<uint8_t> filterValuesList)
{
    IpAddrPool subPool;
    if(filterValuesList.size() > 4)
    {
        throw std::length_error("Initializer list is out of range");
    }
    for(const auto& addrToFilter : m_data)
    {
        uint8_t i = 0;
        bool isPassed = true;
        for(const auto filterValue : filterValuesList)
        {
            if(addrToFilter[i] != filterValue)
            {
                isPassed = false;
                break;
            }
            ++i;
        }
        if(isPassed)
        {
            subPool.pushBack(addrToFilter);
        }
    }
    return subPool;
}

IpAddrPool IpAddrPool::filterAny(uint8_t filterValue)
{
    IpAddrPool subPool;
    for(const auto& addrToFilter : m_data)
    {
        for(int i = 0; i < 4; ++i)
        {
            if(addrToFilter[i] == filterValue)
            {
                subPool.pushBack(addrToFilter);
                break;
            }
        }
    }
    return subPool;
}

void IpAddrPool::print()
{
    for(const IpAddr& ipAddr: m_data)
    {
        std::cout << (int)ipAddr[0] << "."
                  << (int)ipAddr[1] << "."
                  << (int)ipAddr[2] << "."
                  << (int)ipAddr[3] << std::endl;
    }
}