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
    for(std::string line; std::getline(std::cin, line);)
    {
        auto splittedLine = split(line, '\t');
        auto splittedIpAddr = split(splittedLine.at(0), '.');
        ipPool.pushBack({
            static_cast<uint8_t>(std::stoi(splittedIpAddr[0])),
            static_cast<uint8_t>(std::stoi(splittedIpAddr[1])),
            static_cast<uint8_t>(std::stoi(splittedIpAddr[2])),
            static_cast<uint8_t>(std::stoi(splittedIpAddr[3]))
        });
    }
    return ipPool;
}