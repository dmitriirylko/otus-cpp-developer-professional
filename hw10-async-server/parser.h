#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <list>
#include <stdexcept>
#include <cassert>

#include "asyncdefs.h"
#include "filelogger.h"

enum class CmdType
{
    PAYLOAD,
    END_OF_FILE,
    START_DYNAMIC,
    END_DYNAMIC
};

enum class QueuePackType
{
    NESTING,
    MIXED
};

/**
 * @brief Parser class is responsible for managing context: receives data from external
 *          context, parse it and put cmd packets to different queues for further processing.
 */
class Parser
{
public:
    /**
     * @brief Parser ctor.
     * @param consoleQueue Shared_ptr to console queue for weak_ptr construction.
     * @param fileQueue Shared_ptr to file queue for weak_ptr construction.
     * @param packetSize Amount of cmds in packet.
     */
    Parser(const async::ConsoleQueueShared_t& consoleQueue,
           const async::FileQueueShared_t& fileQueue,
           std::size_t packetSize);

    ~Parser() = default;

    /**
     * @brief Preliminary data processing. Iterates through data buffer and forms
     *          cmds. Characters signaling about cmd ending are '\n' and '\0'.
     * @note Method is not thread-safe. Caller should take responsible for synchronization.
     * @param data Pointer to byte array.
     * @param size Buffer size.
     */
    void receive(const char* data, size_t size);

private:
    /**
     * @brief Weak pointer to console queue.
     */
    async::ConsoleQueueWeak_t m_consoleQueue;

    /**
     * @brief Weak pointer to file queue.
     */
    async::FileQueueWeak_t m_fileQueue;

    /**
     * @brief Stores and assembles current cmd from characters.
     */
    std::string m_currentCmd;

    /**
     * @brief Stores and assembles current cmd packet. Cmds are mixed between
     *          parsers (contexts).
     */
    inline static std::vector<std::string> m_packet;
    
    /**
     * @brief Stores and assembles current cmd packet in nesting case.
     */
    std::vector<std::string> m_packetNesting;
    
    /**
     * @brief Default (without of nesting) amount of cmds in packet.
     */
    size_t m_defaultPacketSize;

    /**
     * @brief Current nesting level.
     */
    int m_nestingLevel = 0;

    /**
     * @brief Last cmd receive time.
     */
    inline static std::time_t m_lastCmdRecvTime;

    std::time_t m_lastCmdRecvTimeNesting;

    /**
     * @brief Finds type of cmd: empty cmd interprets as EOF, curly braces as start
     *          and stop markers for nesting, otherwise is payload (ordinary cmd).
     * @param cmd Command.
     * @return Cmd type.
     */
    CmdType findType(const std::string& cmd);

    /**
     * @brief Makes decisions about current cmd: accumulate cmd to packet, start/stop accumulation.
     * @param cmd Cmd.
     */
    void parse(const std::string& cmd);

    /**
     * @brief Notifies console logger thread that cmd packet is ready.
     */
    void notifyConsolePacketReady(QueuePackType type);

    /**
     * @brief Notifies file logger threads that cmd packet is ready.
     */
    void notifyFilePacketReady(QueuePackType type);
};