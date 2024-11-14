#include <iostream>

#include "config.h"
#include "duplicatefinder.h"

int main(int argc, char* argv[])
{
    try
    {
        Config::instance().setData(argc, argv);
        DuplicateFinder dupFinder;
        dupFinder.createFilePool();
        dupFinder.findDuplicates();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}