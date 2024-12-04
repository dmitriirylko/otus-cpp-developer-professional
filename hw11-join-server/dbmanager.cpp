#include <cstring>
#include <sstream>

#include "dbmanager.h"

DbManager::DbManager() :
    m_dbName{"join_server_db.sqlite"},
    m_handle{nullptr}
{
    if(sqlite3_open(m_dbName, &m_handle))
    {
        std::cout << "Can't open db: " << sqlite3_errmsg(m_handle) << std::endl;
        sqlite3_close(m_handle);
    }
    else
    {
        std::cout << "Db opened successfully" << std::endl;
    }
}

DbManager::~DbManager()
{
    std::cout << "Db closed" << std::endl;
    sqlite3_close(m_handle);
}

std::tuple<ErrorCode, std::string> DbManager::execute(ErrorCode cmd,
                                                      std::vector<std::string> &cmdTokens)
{
    std::string query = makeQuery(cmd, cmdTokens);

    auto print_results = [](void *load, int columns, char **data, char **names) -> int {
        std::string *sp = static_cast<std::string*>(load);
        sp->append("< ");
        for(int i = 0; i < columns; ++i)
        {
            std::cout << names[i] << " = " << (data[i] ? data[i] : "NULL") << std::endl;
            
            if(strcmp(names[i], "id") != 0) sp->push_back(',');
            if(data[i]) sp->append(data[i]);
        }
        sp->push_back('\n');
        std::cout << std::endl;
        return 0;
    };

    char *errMsg;
    std::string execLoad;
    int rc = sqlite3_exec(m_handle, query.c_str(), print_results, &execLoad, &errMsg);
    if(rc != SQLITE_OK)
    {
        std::cout << "Can not execute query: " << errMsg << std::endl;
        std::string errStr(errMsg);
        sqlite3_free(errMsg);
        sqlite3_close(m_handle);
        return {ErrorCode::ERROR, errStr};
    }
    else if(!execLoad.empty())
    {
        return {ErrorCode::LOAD, std::move(execLoad)};
    }
    return {ErrorCode::OK, ""};
}

std::string DbManager::makeQuery(ErrorCode &cmd, std::vector<std::string> &cmdTokens)
{
    std::string query;
    size_t queryLength;
    switch (cmd)
    {
    case ErrorCode::INSERT:
        queryLength = snprintf(NULL, 0, "INSERT INTO %s VALUES(%s, '%s')",
                                cmdTokens[1].c_str(),
                                cmdTokens[2].c_str(),
                                cmdTokens[3].c_str()) + 1;
        query = std::string(queryLength, 0x00);
        snprintf(&query[0], queryLength, "INSERT INTO %s VALUES(%s, '%s')",
                    cmdTokens[1].c_str(),
                    cmdTokens[2].c_str(),
                    cmdTokens[3].c_str());
        break;

    case ErrorCode::TRUNCATE:
        queryLength = snprintf(NULL, 0, "DELETE FROM %s;VACUUM",
                                cmdTokens[1].c_str()) + 1;
        query = std::string(queryLength, 0x00);
        snprintf(&query[0], queryLength, "DELETE FROM %s;VACUUM",
                    cmdTokens[1].c_str());
        break;

    case ErrorCode::INTERSECTION:
        query = "SELECT A.id, A.name, B.name FROM A, B where A.id = B.id;";
        break;

    case ErrorCode::SYMMETRIC_DIFFERENCE:
        query = "SELECT id, name, NULL FROM A WHERE id NOT IN (SELECT id FROM B) UNION SELECT id, NULL, name FROM B WHERE id NOT IN (SELECT id FROM A);";
        break;

    default:
        break;
    }
    return query;
}

void DbManager::create()
{
    char *errMsg;
    std::string query = "CREATE TABLE IF NOT EXISTS A (id INTEGER PRIMARY KEY, name VARCHAR(30));CREATE TABLE IF NOT EXISTS B (id INTEGER PRIMARY KEY, name VARCHAR(30));";
    int rc = sqlite3_exec(m_handle, query.c_str(), 0, 0, &errMsg);
    if(rc != SQLITE_OK)
    {
        std::cout << "Can not execute query: " << errMsg << std::endl;
        std::string errStr(errMsg);
        sqlite3_free(errMsg);
        sqlite3_close(m_handle);
    }
}