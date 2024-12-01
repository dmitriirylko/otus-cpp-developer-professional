#pragma once

#include <ctime>
#include <fstream>
#include <sstream>
#include <thread>

#include "asyncdefs.h"
#include "waitingqueue.h"

struct FileLoggerCmd
{
    std::time_t recvTime;
    std::vector<std::string> pack;
};

class FileLogger
{
public:
    FileLogger(const std::shared_ptr<WaitingQueue<FileLoggerCmd>>& queue);
    ~FileLogger();
    void updatePacketReady();

private:
    void process();
    bool isContinue();

    std::ofstream m_fileStream;
    FileLoggerCmd m_cmd;
    std::weak_ptr<WaitingQueue<FileLoggerCmd>> m_queue;
    std::thread m_thread;
    size_t m_counter;
};