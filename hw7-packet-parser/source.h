#pragma once

#include <string>
#include <iostream>
#include <memory>

#include "parser.h"

/**
 * @brief Interface class for command source classes. Sources for
 *        commands can be standard input stream, files, network
 *        and so on.
 */
class ISource
{
public:
    virtual std::string getCmd() = 0;
    virtual ~ISource() = default;
};

class StdInputSource : public ISource
{
public:
    StdInputSource() = default;
    ~StdInputSource() = default;
    std::string getCmd() override;
};

class Source
{
public:
    Source(std::unique_ptr<ISource> sourcePtr);
    ~Source() = default;
    std::string getCmd();

private:
    std::unique_ptr<ISource> m_pSource;
};