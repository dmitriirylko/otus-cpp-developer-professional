#include "consolelogger.h"

ConsoleLogger::ConsoleLogger(const async::ConsoleQueueShared_t& queue) :
    m_queue{queue}
{
    m_thread = std::thread{&ConsoleLogger::process, this};
}

ConsoleLogger::~ConsoleLogger()
{
    m_thread.join();
}

void ConsoleLogger::updatePacketReady()
{
    size_t cnt = 0;
    for(const auto& cmd : m_cmdPack)
    {
        if(cnt == 0)
        {
            std::cout << "bulk: " << cmd;
        }
        else
        {
            std::cout << ", " << cmd;
        }
        ++cnt;
    }
    std::cout << std::endl;
}

void ConsoleLogger::process()
{
    while(isContinue())
    {
        updatePacketReady();
    }
}

bool ConsoleLogger::isContinue()
{
    auto queue = m_queue.lock();
    return queue->pop(m_cmdPack);
}