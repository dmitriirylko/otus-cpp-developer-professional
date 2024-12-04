#pragma once

#include <iostream>
#include <vector>
#include <tuple>
#include <map>

#include "errorcode.h"

class Parser
{
public:

    Parser() = default;
    ~Parser() = default;
    std::tuple<ErrorCode, std::string> process(const char* data,
                                                     size_t length,
                                                     std::vector<std::string> &cmdTokens);

private:
    std::tuple<ErrorCode, std::string> parse(std::vector<std::string> &cmdTokens);
    std::string m_currentCmd;
    std::map<std::string, ErrorCode> m_cmdTypes = {{"insert", ErrorCode::INSERT},
                                                    {"truncate", ErrorCode::TRUNCATE},
                                                    {"intersection", ErrorCode::INTERSECTION},
                                                    {"symmetric_difference", ErrorCode::SYMMETRIC_DIFFERENCE}};
};