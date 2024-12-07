#include <sstream>
#include <algorithm>
#include <cassert>

#include "parser.h"

namespace ErrorMsgs
{
    constexpr const char* INVALID_QUERY_LENGTH = "invalid query length";
    constexpr const char* INVALID_CMD_TYPE = "invalid command type";
    constexpr const char* INVALID_ARGUMENTS_AMOUNT = "invalid arguments amount";
};

std::tuple<ErrorCode, std::string> Parser::process(const char* data,
                                                    size_t length,
                                                    std::vector<std::string> &cmdTokens)
{
    for(size_t i = 0; i < length; ++i)
    {
        /* End of cmd. */
        if(data[i] == '\n' || data[i] == '\0')
        {
            auto res = parse(cmdTokens);
            m_currentCmd.clear();
            return res;
        }
        /* Append character to cmd. */
        else
        {
            m_currentCmd.push_back(data[i]);
        }
    }
    return {ErrorCode::SKIP, ""};
}

std::tuple<ErrorCode, std::string> Parser::parse(std::vector<std::string> &cmdTokens)
{
    cmdTokens.clear();
    std::istringstream iss(m_currentCmd);
    std::string s;

    while(getline(iss, s, ' ')) cmdTokens.push_back(s);
    
    std::string firstToken;
    for_each(cmdTokens[0].begin(), cmdTokens[0].end(), [&firstToken](char& c){
        firstToken.push_back(tolower(c));
    });

    ErrorCode cmdType;
    try
    {
        /* Throws std::out_of_range if there is no such element in map, i.e. cmd is invalid. */
        cmdType =  m_cmdTypes.at(firstToken);
        /* Check number of passed arguments. If arguments amount doesn't match particular command
           then throw std::invalid_argument. */
        switch (cmdType)
        {
        case ErrorCode::INSERT:
            if(cmdTokens.size() != 4) throw std::invalid_argument(ErrorMsgs::INVALID_ARGUMENTS_AMOUNT);
            break;

        case ErrorCode::TRUNCATE:
            if(cmdTokens.size() != 2) throw std::invalid_argument(ErrorMsgs::INVALID_ARGUMENTS_AMOUNT);
            break;

        case ErrorCode::INTERSECTION:
            if(cmdTokens.size() != 1) throw std::invalid_argument(ErrorMsgs::INVALID_ARGUMENTS_AMOUNT);
            break;

        case ErrorCode::SYMMETRIC_DIFFERENCE:
            if(cmdTokens.size() != 1) throw std::invalid_argument(ErrorMsgs::INVALID_ARGUMENTS_AMOUNT);
            break;

        default:
            assert(0 && "There is no such type of command");
        }
    }
    catch(const std::out_of_range &e)
    {
        return {ErrorCode::ERROR, ErrorMsgs::INVALID_CMD_TYPE};
    }
    catch(const std::invalid_argument &e)
    {
        return {ErrorCode::ERROR, e.what()};
    }
    
    return {cmdType, ""};
}