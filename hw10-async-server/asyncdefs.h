#pragma once

#include <memory>
#include <vector>
#include <string>

#include "waitingqueue.h"
#include "filelogger.h"

namespace async {
    using ConsoleQueue_t = WaitingQueue<std::vector<std::string>>;
    using FileQueue_t = WaitingQueue<struct FileLoggerCmd>;

    using ConsoleQueueShared_t = std::shared_ptr<ConsoleQueue_t>;
    using FileQueueShared_t = std::shared_ptr<FileQueue_t>;
    
    using ConsoleQueueWeak_t = std::weak_ptr<ConsoleQueue_t>;
    using FileQueueWeak_t = std::weak_ptr<FileQueue_t>;
}
