#include <iostream>

#include "lib.h"

int main()
{
    std::cout << "Version: " << version() << std::endl;
    auto closure = []()
    {
        return isVersionGreater(version(), "0.0.0") ? std::string("true") : std::string("false");
    };
    std::cout << "Is version greater than 0.0.0: " << closure() << std::endl;
    std::cout << "Hello, world!" << std::endl;
    
    return 0;
}