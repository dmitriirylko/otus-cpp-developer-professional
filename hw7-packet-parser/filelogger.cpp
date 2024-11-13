#include "filelogger.h"

void FileLogger::updatePacketReady(const std::vector<std::string>& cmdPack)
{
    std::string filename = "bulk" + std::to_string(m_lastCmdRecvTime) + ".log";
    m_fileStream.open(filename, std::ios::out | std::ios::trunc);
    if(m_fileStream.is_open())
    {    
        size_t cnt = 0;
        for(const auto& cmd : cmdPack)
        {
            if(cnt == 0)
            {
                m_fileStream << "bulk: " << cmd;
            }
            else
            {
                m_fileStream << ", " << cmd;
            }
            ++cnt;
        }
        m_fileStream.close();
    }
    else
    {
        throw std::runtime_error("Could not open file for logging.");
    }
}

void FileLogger::updatePacketStarted()
{
    std::time(&m_lastCmdRecvTime);
}

FileLogger::~FileLogger()
{
    if(m_fileStream.is_open())
    {
        m_fileStream.close();
    }
}