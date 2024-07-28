#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "main.h"
#include "reader.h"
#include "ipaddrpool.h"

int main(int argc, char const *argv[])
{
    std::string filePath;
    argc == 1 ? filePath = filePathDefault : filePath = argv[1];
    try
    {
        Reader reader(filePath);
        auto ipPoolSorted = reader.getPool();
        ipPoolSorted.sortDescending();
        ipPoolSorted.print();
        IpAddrPool ipPoolFiltered_1 = ipPoolSorted.filter({1});
        ipPoolFiltered_1.print();
        IpAddrPool ipPoolFiltered_2 = ipPoolSorted.filter({46, 70});
        ipPoolFiltered_2.print();
        IpAddrPool ipPoolFiltered_3 = ipPoolSorted.filterAny(46);
        ipPoolFiltered_3.print();
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}