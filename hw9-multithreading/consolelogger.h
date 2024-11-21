#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <thread>

#include "asyncdefs.h"
#include "waitingqueue.h"

class ConsoleLogger
{
public:
    ConsoleLogger(const async::ConsoleQueueShared_t& queue);
    ~ConsoleLogger();
    void process();

private:
    void updatePacketReady();
    bool isContinue();
    
    async::ConsoleQueueWeak_t m_queue;
    std::vector<std::string> m_cmdPack;
    std::thread m_thread;
};