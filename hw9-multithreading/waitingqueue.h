#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <iostream>
#include <atomic>

template<typename T>
struct WaitingQueue
{
    WaitingQueue() :
        m_headPtr{new Node},
        m_tailPtr{m_headPtr.get()},
        m_stopped{false}
    {}

    WaitingQueue(const WaitingQueue&) = delete;
    WaitingQueue(WaitingQueue &&) = delete;
    WaitingQueue& operator=(const WaitingQueue&) = delete;
    WaitingQueue& operator=(const WaitingQueue&&) = delete;

    bool pop(T &entry)
    {
        return popHead(entry);
    }

    bool tryPop(T &entry)
    {
        std::unique_ptr<Node> prevHead = tryPopHead(entry);
        return prevHead.get() != nullptr;
    }

    template<typename TT>
    void push(TT &&value)
    {
        std::unique_ptr<Node> node{new Node};
        {
            std::lock_guard<std::mutex> lock{m_tailMutex};
            if(m_stopped) return;
            m_tailPtr->value = std::forward<TT>(value);
            m_tailPtr->next = std::move(node);
            m_tailPtr = m_tailPtr->next.get();
        }
        m_conditional.notify_one();
    }

    bool empty()
    {
        std::lock_guard<std::mutex> lock{m_headMutex};
        return m_headPtr.get() == tail();
    }

    void stop()
    {
        std::lock_guard<std::mutex> lock1{m_headMutex};
        std::lock_guard<std::mutex> lock2{m_tailMutex};
        m_stopped = true;
        m_conditional.notify_all();
    }

private:
    struct Node
    {
        T value;
        std::unique_ptr<Node> next;
    };

    Node* tail()
    {
        std::lock_guard<std::mutex> lock{m_tailMutex};
        return m_tailPtr;
    }

    std::unique_ptr<Node> takeHeadUnsafe()
    {
        std::unique_ptr<Node> prevHead = std::move(m_headPtr);
        m_headPtr = std::move(prevHead->next);
        return prevHead;
    }

    std::unique_ptr<Node> tryPopHead(T &entry)
    {
        std::lock_guard<std::mutex> lock{m_headMutex};
        if(m_headPtr.get() == tail()) return{};
        entry = std::move(m_headPtr->value);
        return takeHeadUnsafe();
    }

    bool popHead(T &entry)
    {
        std::unique_lock<std::mutex> lock{m_headMutex};
        m_conditional.wait(lock, [&](){
            if(m_stopped) return true;
            return (m_headPtr.get() != tail());
        });
        
        if(m_stopped && m_headPtr.get() == tail())
        {
            return false;
        }

        entry = std::move(m_headPtr->value);
        takeHeadUnsafe();
        return true;
    }

    std::mutex m_headMutex;
    std::mutex m_tailMutex;
    std::condition_variable m_conditional;
    std::unique_ptr<Node> m_headPtr;
    Node *m_tailPtr;
    bool m_stopped;
};