#include "reader.h"

Reader::Reader(const std::string &filePath) : m_filePath{filePath}
{
    m_fileStream.open(m_filePath, std::ifstream::in);
    if(m_fileStream.fail())
    {
        throw std::invalid_argument("Fail opening file");
	}
    std::cin.rdbuf(m_fileStream.rdbuf());
}

Reader::~Reader()
{
    if(m_fileStream.is_open())
    {
        m_fileStream.close();
    }
}

std::vector<std::string> Reader::split(const std::string &str, char d)
{
    std::vector<std::string> r;
    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);
    while(stop != std::string::npos)
    {
        r.push_back(str.substr(start, stop - start));
        start = stop + 1;
        stop = str.find_first_of(d, start);
    }
    r.push_back(str.substr(start));
    return r;
}

IpAddrPool Reader::getPool()
{
    IpAddrPool ipPool;
    std::array<int, IpAddr::length> ipAddrNumbers;
    for(std::string line; std::getline(std::cin, line);)
    {
        auto splittedLine = split(line, '\t');
        if(splittedLine.size() != 3)
        {
            throw std::runtime_error("Can not parse line (split by '\\t'): " + line);
        }
        auto splittedIpAddr = split(splittedLine.at(0), '.');
        if(splittedIpAddr.size() != 4)
        {
            throw std::runtime_error("Can not parse ip addr string (split by '.'): " + splittedLine.at(0));
        }
        try
        {
            for(size_t i = 0; i < IpAddr::length; ++i)
            {
                ipAddrNumbers[i] = std::stoi(splittedIpAddr[i]);
                if(ipAddrNumbers[i] > std::numeric_limits<uint8_t>::max() ||
                   ipAddrNumbers[i] < std::numeric_limits<uint8_t>::min())
                {
                    throw std::runtime_error("");
                }
            }
            ipPool.emplaceBack(static_cast<uint8_t>(ipAddrNumbers[0]),
                               static_cast<uint8_t>(ipAddrNumbers[1]),
                               static_cast<uint8_t>(ipAddrNumbers[2]),
                               static_cast<uint8_t>(ipAddrNumbers[3]));
        }
        catch(const std::exception& e)
        {
            throw std::runtime_error("Can not parse ip addr (invalid conversion from string to uint8_t): " + splittedLine.at(0));
        }
    }
    return ipPool;
}