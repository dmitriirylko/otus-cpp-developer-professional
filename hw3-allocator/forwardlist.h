#pragma once

#include <cassert>

template<typename T, typename Alloc = std::allocator<T>>
class ForwardList
{
private:
struct Node
    {
        Node(T item) : data{std::move(item)}
        {}
        Node *next = nullptr;
        T data;
    };
    Node *m_head = nullptr;
    Node *m_tail = nullptr;

    using AllocNode = typename std::allocator_traits<Alloc>::template rebind_alloc<Node>;
    // using AllocNode = typename Alloc::rebind<Node>;
    AllocNode alloc;

public:
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using difference_type = ptrdiff_t;
    using size_type = size_t;
    using pointer = T*;
    using const_pointer = const T*;

    class ForwardListConstIterator
    {
    private:
        friend class ForwardList;
        
        explicit ForwardListConstIterator(const Node *ptr) noexcept :
            m_pCurrentNode{ptr}
        {}

        const Node* get() const noexcept
        {
            return m_pCurrentNode;
        }
        
        const Node *m_pCurrentNode;

    public:
        using difference_type = ForwardList::difference_type;
        using value_type = ForwardList::value_type;
        using pointer = ForwardList::const_pointer;
        using reference = ForwardList::const_reference;
        using iterator_category = std::forward_iterator_tag;
        
        reference operator*() const noexcept
        {
            assert(m_pCurrentNode != nullptr);
            return m_pCurrentNode->data;
        }
        
        ForwardListConstIterator& operator++() noexcept
        {
            assert(m_pCurrentNode != nullptr);
            m_pCurrentNode = m_pCurrentNode->next;
            return *this;
        }
        
        ForwardListConstIterator operator++(int) noexcept
        {
            assert(m_pCurrentNode != nullptr);
            ForwardListConstIterator iterCopy = *this;
            m_pCurrentNode = m_pCurrentNode->next;
            return iterCopy;
        }
        
        bool operator==(ForwardListConstIterator &other) const noexcept
        {
            return m_pCurrentNode == other.m_current;
        }
        
        bool operator!=(ForwardListConstIterator &other) const noexcept
        {
            return m_pCurrentNode != other.m_current;
        }
    };

    class ForwardListIterator
    {
    private:
        friend class ForwardList;
        
        explicit ForwardListIterator(Node *ptr) noexcept :
            m_pCurrentNode{ptr}
        {}
        
        Node* get() const noexcept
        {
            return m_pCurrentNode;
        }
        
        Node *m_pCurrentNode;

    public:
        using difference_type = ForwardList::difference_type;
        using value_type = ForwardList::value_type;
        using pointer = ForwardList::const_pointer;
        using reference = ForwardList::reference;
        using iterator_category = std::forward_iterator_tag;
        
        reference operator*() const noexcept
        {
            assert(m_pCurrentNode != nullptr);
            return m_pCurrentNode->data;
        }
        
        ForwardListIterator& operator++() noexcept
        {
            assert(m_pCurrentNode != nullptr);
            m_pCurrentNode = m_pCurrentNode->next;
            return *this;
        }
        
        ForwardListIterator operator++(int) noexcept
        {
            assert(m_pCurrentNode != nullptr);
            ForwardListIterator iterCopy = *this;
            m_pCurrentNode = m_pCurrentNode->next;
            return iterCopy;
        }
        
        bool operator==(ForwardListIterator other) const noexcept
        {
            return m_pCurrentNode == other.m_pCurrentNode;
        }
        
        bool operator!=(ForwardListIterator other) const noexcept
        {
            return m_pCurrentNode != other.m_pCurrentNode;
        }
    };

    using iterator = ForwardListIterator;
    using const_iterator = ForwardListConstIterator;

    ForwardList() = default;
    
    void clear()
    {
        while(m_head)
        {
            Node *nodePtrCopy = m_head;
            m_head = m_head->next;
            std::allocator_traits<AllocNode>::destroy(alloc, nodePtrCopy);
            std::allocator_traits<AllocNode>::deallocate(alloc, nodePtrCopy, 1);
        }
        m_tail = nullptr;
    }

    ~ForwardList()
    {
        clear();
    }

    void pushFront(value_type item)
    {
        Node *newNode = std::allocator_traits<AllocNode>::allocate(alloc, 1);
        std::allocator_traits<AllocNode>::construct(alloc, newNode, item);
        if(m_head)
        {
            newNode->next = m_head;
            m_head = newNode;
        }
        else
        {
            m_head = newNode;
            m_tail = newNode;
        }
    }

    void pushBack(value_type item)
    {
        Node *newNode = std::allocator_traits<AllocNode>::allocate(alloc, 1);
        std::allocator_traits<AllocNode>::construct(alloc, newNode, item);
        if(m_tail)
        {
            m_tail->next = newNode;
            m_tail = newNode;
        }
        else
        {
            m_head = newNode;
            m_tail = newNode;
        }
    }

    void insertAfter(iterator place, value_type item)
    {
        Node *nodePtr = place.get();
        if(!nodePtr)
        {
            pushFront(std::move(item));
            return;
        }
        Node *newNode = std::allocator_traits<AllocNode>::allocate(alloc, 1);
        std::allocator_traits<AllocNode>::construct(alloc, newNode, item);
        newNode->next = nodePtr->next;
        nodePtr->next = newNode;
    }

    iterator begin() const noexcept
    {
        return iterator{m_head};
    }

    iterator end() const noexcept
    {
        return iterator{nullptr};
    }

    const_iterator cbegin() const noexcept
    {
        return const_iterator{m_head};
    }

    const_iterator cend() const noexcept
    {
        return const_iterator{nullptr};
    }

    iterator find(const_reference item) const noexcept
    {
        for(auto it = begin(); it != end(); ++it)
        {
            if(*it == item)
            {
                return it;
            }
        }
        return iterator{nullptr};
    }
};