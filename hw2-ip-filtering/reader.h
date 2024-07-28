#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <iostream>

#include "ipaddrpool.h"

class Reader
{
public:
    Reader() = delete;
    Reader(const std::string &filePath);
    ~Reader();
    IpAddrPool getPool();

private:
    std::vector<std::string> split(const std::string &str, char d);

    std::string m_filePath;
    std::ifstream m_fileStream;
};