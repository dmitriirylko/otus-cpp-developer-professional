#include "filelogger.h"

FileLogger::FileLogger(const std::shared_ptr<WaitingQueue<FileLoggerCmd>>& queue) :
    m_queue{queue},
    m_counter{0}
{
    m_thread = std::thread{&FileLogger::process, this};
}

void FileLogger::updatePacketReady()
{
    m_stringStream << std::this_thread::get_id();

    std::string filename = "bulk" +
                           std::to_string(m_cmd.recvTime) +
                           "-" +
                           m_stringStream.str() +
                           "-" +
                           std::to_string(m_counter) +
                           ".log";
    m_fileStream.open(filename, std::ios::out | std::ios::trunc);
    if(m_fileStream.is_open())
    {    
        size_t cnt = 0;

        for(const auto& cmd : m_cmd.pack)
        {
            if(cnt == 0) m_fileStream << "bulk: " << cmd;
            else m_fileStream << ", " << cmd;
            ++cnt;
        }
        m_fileStream.close();
    }
    else
    {
        throw std::runtime_error("Could not open file for logging.");
    }
    ++m_counter;
}

FileLogger::~FileLogger()
{
    if(m_fileStream.is_open())
    {
        m_fileStream.close();
    }
    m_thread.join();
}

void FileLogger::process()
{
    while(isContinue())
    {
        updatePacketReady();
    }
}

bool FileLogger::isContinue()
{
    auto queue = m_queue.lock();
    return queue->pop(m_cmd);
}