#include <iostream>

#include "source.h"
#include "parser.h"
#include "consolelogger.h"
#include "filelogger.h"

int main(int argc, char* argv[])
{
    size_t packetSize;
    if(argc > 1)
    {
        packetSize =  std::strtoul(argv[1], nullptr, 0);
    }
    else
    {
        packetSize = 3;
    }
    Source source(std::make_unique<StdInputSource>());
    auto parser = std::make_shared<Parser>(packetSize);
    auto consoleLogger = std::make_shared<ConsoleLogger>();
    auto fileLogger = std::make_shared<FileLogger>();
    parser->subscribePacketReady(consoleLogger);
    parser->subscribePacketReady(fileLogger);
    parser->subscribePacketStarted(fileLogger);
    std::string str;
    do
    {
        str = source.getCmd();
        ParseCommand command(parser, str);
        command.execute();
    }
    while(!str.empty());

    return 0;
}