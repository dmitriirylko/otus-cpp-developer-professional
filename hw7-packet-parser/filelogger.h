#pragma once

#include <ctime>
#include <fstream>

#include "parser.h"

class FileLogger : public ISubscriber
{
public:
    FileLogger() = default;
    ~FileLogger();
    void updatePacketReady(const std::vector<std::string>& cmdPack) override;
    void updatePacketStarted() override;

private:
    std::time_t m_lastCmdRecvTime;
    std::ofstream m_fileStream;
};