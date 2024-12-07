#pragma once

#include <iostream>
#include <tuple>

#include "sqlite3.h"

#include "parser.h"
#include "errorcode.h"

enum class DbErrorCode
{
    OK,
    FAIL
};

class DbManager
{
public:
    DbManager();
    ~DbManager();
    std::tuple<ErrorCode, std::string> execute(ErrorCode cmd,
                                                 std::vector<std::string> &cmdTokens);
    void create();

private:
    std::string makeQuery(ErrorCode &cmd, std::vector<std::string> &cmdTokens);

    const char* m_dbName;
    sqlite3* m_handle;
};