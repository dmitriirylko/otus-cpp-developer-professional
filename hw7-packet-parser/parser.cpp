#include "parser.h"

Parser::Parser(size_t packetSize) :
    m_defaultPacketSize{packetSize}
{
    m_packet.reserve(m_defaultPacketSize);
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
            notifyPacketReady();
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
        notifyPacketReady();
        m_packet.clear();
        return;

    case CmdType::PAYLOAD:
        if(m_packet.size() == 0)
        {
            notifyPacketStarted();
        }
        m_packet.push_back(cmd);
        if((m_packet.size() == m_defaultPacketSize) && (!m_nestingLevel))
        {
            notifyPacketReady();
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
            notifyPacketReady();
            m_packet.clear();
        }
        return;

    default:
        throw std::logic_error("There is no that type of command in parser.");
    }
}

void Parser::subscribePacketStarted(const std::shared_ptr<ISubscriber>& sub)
{
    m_subsPacketStarted.emplace_back(sub);
}

void Parser::subscribePacketReady(const std::shared_ptr<ISubscriber>& sub)
{
    m_subsPacketReady.emplace_back(sub);
}

void Parser::notifyPacketStarted()
{
    auto iter = m_subsPacketStarted.begin();
    while(iter != m_subsPacketStarted.end())
    {
        auto ptr = iter->lock();
        if(ptr)
        {
            ptr->updatePacketStarted();
            ++iter;
        }
        else
        {
            m_subsPacketReady.erase(iter++);
        }
    }
}

void Parser::notifyPacketReady()
{
    auto iter = m_subsPacketReady.begin();
    while(iter != m_subsPacketReady.end())
    {
        auto ptr = iter->lock();
        if(ptr)
        {
            ptr->updatePacketReady(m_packet);
            ++iter;
        }
        else
        {
            m_subsPacketReady.erase(iter++);
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

ParseCommand::ParseCommand(const std::shared_ptr<Parser>& parser, const std::string& cmd) :
    m_parser{parser},
    m_cmd{cmd}
{}

void ParseCommand::execute()
{
    auto ptr = m_parser.lock();
    if(ptr)
    {
        ptr->parse(m_cmd);
    }
}