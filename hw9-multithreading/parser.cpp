#include "parser.h"

Parser::Parser(const async::ConsoleQueueShared_t& consoleQueue,
               const async::FileQueueShared_t& fileQueue,
               size_t packetSize) :
    m_consoleQueue{consoleQueue},
    m_fileQueue{fileQueue},
    m_defaultPacketSize{packetSize}
{
    m_packet.reserve(m_defaultPacketSize);
}

void Parser::receive(const char* data, size_t size)
{
    for(size_t i = 0; i < size; ++i)
    {
        /* End of cmd. */
        if(data[i] == '\n' || data[i] == '\0')
        {
            parse(m_currentCmd);
            m_currentCmd.clear();
        }
        /* Append character to cmd. */
        else
        {
            m_currentCmd.push_back(data[i]);
        }
    }
}

CmdType Parser::findType(const std::string& cmd)
{
    if(cmd.empty())
    {
        return CmdType::END_OF_FILE;
    }
    else if(cmd == "{")
    {
        return CmdType::START_DYNAMIC;
    }
    else if(cmd == "}")
    {
        return CmdType::END_DYNAMIC;
    }
    else
    {
        return CmdType::PAYLOAD;
    }
}

void Parser::parse(const std::string& cmd)
{
    CmdType cmdType = findType(cmd);
    
    switch (cmdType)
    {
    case CmdType::START_DYNAMIC:
        ++m_nestingLevel;
        if(m_nestingLevel > 1)
        {
            return;
        }
        if(m_packet.size())
        {
            notifyConsolePacketReady();
            notifyFilePacketReady();
        }
        m_packet.clear();
        return;

    case CmdType::END_DYNAMIC:
        if(m_nestingLevel == 0)
        {
            return;
        }
        --m_nestingLevel;
        if(m_nestingLevel)
        {
            return;
        }
        notifyConsolePacketReady();
        notifyFilePacketReady();
        m_packet.clear();
        return;

    case CmdType::PAYLOAD:
        if(m_packet.size() == 0)
        {
            std::time(&m_lastCmdRecvTime);
        }
        m_packet.push_back(cmd);
        if((m_packet.size() == m_defaultPacketSize) && (!m_nestingLevel))
        {
            notifyConsolePacketReady();
            notifyFilePacketReady();
            m_packet.clear();
        }
        return;

    case CmdType::END_OF_FILE:
        if(m_nestingLevel > 0)
        {
            m_nestingLevel = 0;
            m_packet.clear();
            return;
        }
        if(m_packet.size())
        {
            notifyConsolePacketReady();
            notifyFilePacketReady();
            m_packet.clear();
        }
        return;

    default:
        throw std::logic_error("There is no that type of command in parser.");
    }
}

void Parser::notifyConsolePacketReady()
{
    auto queue = m_consoleQueue.lock();
    if(queue) queue->push(m_packet);
}

void Parser::notifyFilePacketReady()
{
    auto queue = m_fileQueue.lock();
    if(queue) queue->push(FileLoggerCmd{m_lastCmdRecvTime, m_packet});
}