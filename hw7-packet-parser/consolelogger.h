#pragma once

#include "parser.h"

class ConsoleLogger : public ISubscriber
{
public:
    ConsoleLogger() = default;
    ~ConsoleLogger() = default;
    void updatePacketReady(const std::vector<std::string>& cmdPack) override;

private:
};