#include "ipaddrpool.h"

void IpAddrPool::pushBack(const IpAddr& addr)
{
    m_data.push_back(addr);
}

void IpAddrPool::sortDescending()
{
    std::sort(m_data.begin(), m_data.end(), [](const IpAddr& lhs, const IpAddr& rhs){
        return lhs.data > rhs.data;
    });
}

IpAddrPool IpAddrPool::filter(const std::initializer_list<uint8_t>& filterValuesList)
{
    IpAddrPool subPool;
    if(filterValuesList.size() > IpAddr::length)
    {
        throw std::length_error("Initializer list is out of range");
    }
    for(const auto& addrToFilter : m_data)
    {
        uint8_t i = 0;
        bool isPassed = true;
        for(const auto filterValue : filterValuesList)
        {
            if(addrToFilter.data[i] != filterValue)
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
        for(int i = 0; i < IpAddr::length; ++i)
        {
            if(addrToFilter.data[i] == filterValue)
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
        std::cout << (int)ipAddr.data[0] << "."
                  << (int)ipAddr.data[1] << "."
                  << (int)ipAddr.data[2] << "."
                  << (int)ipAddr.data[3] << std::endl;
    }
}