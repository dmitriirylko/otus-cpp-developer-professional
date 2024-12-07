#include <iostream>
#include <string>

#include "asyncserver.h"
#include "dbmanager.h"

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        std::cerr << "Invalid number of arguments." << std::endl;
        return 1;
    }
    boost::asio::io_context io_context;
    size_t port;
    std::stringstream sstream(argv[1]);
    sstream >> port;
    Server server{io_context, port};
    {
        DbManager dbManager;
        dbManager.create();
    }
    io_context.run();

    return 0;
}