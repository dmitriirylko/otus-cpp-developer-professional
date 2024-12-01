#include <iostream>
#include <thread>
#include <cstddef>

#include "options.h"
#include "asyncserver.h"

int main(int argc, char** argv)
{
    try {
        Options options{argc, argv};
        boost::asio::io_context io_context;
        Server server{io_context, options.getHost(), options.getBulkSize()};
        io_context.run();
    }
    catch (const boost::program_options::error& e) {
        std::cout << "Program options failed. Error: " << e.what() << std::endl;
        return 1;
    }
    catch (const std::invalid_argument& e) {
        std::cout << "Program options failed. Error: " << e.what() << std::endl;
        return 1;
    }

    // std::size_t bulk = 5;
    // auto h = async::connect(bulk);
    // auto h2 = async::connect(bulk);
    // async::receive(h, "1", 1);
    // async::receive(h2, "1\n", 2);
    // async::receive(h, "\n2\n3\n4\n5\n6\n{\na\n", 15);
    // async::receive(h, "b\nc\nd\n}\n89\n", 11);
    // async::disconnect(h);
    // async::disconnect(h2);

    // async::stop();

    return 0;
}
