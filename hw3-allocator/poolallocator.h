#pragma once

#include <stack>
#include <queue>
#include <memory>

// #define LOGGING

/**
 * @brief Pool from where memory will be allocated.
 */
class Pool
{
public:
    /**
     * @brief Pool constructor.
     * @param typeSize Size of data structure which is going to be allocated in pool.
     * @param elementsNum Number of elements pool have to store in one memory block.
     */
    explicit Pool(std::size_t typeSize, std::size_t elementsNum = 10) :
        m_typeSize{typeSize},
        m_elementsNum{elementsNum}
    {
        addAddrs();
    }
    /**
     * @brief Gets pointer out of the pool for further allocation. If there are no
     *          vacant addresses in pool it tries to allocate new memory block.
     * @param n Number of elements to allocate.
     * @return Pointer out of the pool for allocation.
     */
    void* allocate(size_t n)
    {
        if(n > m_elementsNum)
        {
            throw std::bad_alloc();
        }
        if(m_currentPtr >= m_finalPtr)
        {
            addAddrs();
        }
        uint8_t* currentPtrCopy = m_currentPtr;
        m_currentPtr += n * m_typeSize;

        #ifdef LOGGING
        std::cout << (void*)currentPtrCopy << std::endl;
        #endif
        return currentPtrCopy;
    }
    /**
     * @brief Deallocates memory from pool.
     * @note TO DO.
     */
    void deallocate(void *ptr)
    {}
    /**
     * @brief Changes size of data structure which is going to be allocated in pool.
     *          Rebinding after any allocation is unsupported.
     * @param Size of data structure which is going to be allocated in pool.
     */
    void rebind(std::size_t typeSize)
    {
        m_typeSize = typeSize;
    }
    void addAddrs()
    {
        size_t poolSize = m_typeSize * m_elementsNum;
        m_block.push_back(std::make_unique<uint8_t[]>(poolSize));
        m_currentPtr = m_block.back().get();
        m_finalPtr = m_block.back().get() + poolSize;
        #ifdef LOGGING
        std::cout << " add_addrs ";
        #endif
    }

private:
    /**
     * @brief Size of data structure which is going to be allocated in pool.
     */
    std::size_t m_typeSize;
    /**
     * @brief Number of elements pool have to store in memory block.
     */
    size_t m_elementsNum;
    /**
     * @brief Block from where memory will be allocated.
     */
    // std::unique_ptr<uint8_t[]> m_block;
    std::vector<std::unique_ptr<uint8_t[]>> m_block;
    /**
     * @brief Next free pointer in the pool where memory can be allocated.
     */
    uint8_t* m_currentPtr = nullptr;
    /**
     * @brief Pointer to the element next to the last element of pool.
     */
    uint8_t* m_finalPtr = nullptr;
};

template<typename T, size_t elementsNum = 10>
class PoolAllocator
{
public:
    using value_type = T;
    using pointer = T*;
    using size_type = std::size_t;
    using is_always_equal = std::false_type;
    using propagate_on_container_copy_assignment = std::true_type;
    using propagate_on_container_move_assignment = std::true_type;
    using propagate_on_container_swap = std::true_type;

    template <typename U>
    struct rebind
    {
        using other = PoolAllocator<U, elementsNum>;
    };

    PoolAllocator() : m_pool{std::make_shared<Pool>(sizeof(T), elementsNum)}
    {
        #ifdef LOGGING
        std::cout << this << " constructor, sizeof(T): " << sizeof(T) << '\n';
        #endif
    }

    template<typename U>
    PoolAllocator(const PoolAllocator<U, elementsNum>& other) noexcept : m_pool{other.m_pool}
    {
        #ifdef LOGGING
        std::cout << this << " copy constructor (U), sizeof(T): " << sizeof(T) << '\n';
        #endif
        m_pool->rebind(sizeof(T));
    }

    PoolAllocator(PoolAllocator&& other) noexcept : m_pool{std::move(other.m_pool)}
    {
        #ifdef LOGGING
        std::cout << this << " move constructor, sizeof(T): " << sizeof(T) << '\n';
        #endif
    }

    PoolAllocator& operator=(PoolAllocator&& other) noexcept
    {
        m_pool = std::move(other.m_pool);
        #ifdef LOGGING
        std::cout << this << " move assignment, sizeof(T): " << sizeof(T) << '\n';
        #endif
        return *this;
    }

    PoolAllocator(const PoolAllocator& other) noexcept : m_pool{other.m_pool}
    {
        #ifdef LOGGING
        std::cout << this << " copy constructor, sizeof(T): " << sizeof(T) << '\n';
        #endif
    }

    PoolAllocator& operator=(const PoolAllocator& other) noexcept
    {
        m_pool = other.m_pool;
        #ifdef LOGGING
        std::cout << this << " copy assignment, sizeof(T): " << sizeof(T) << '\n';
        #endif
        return *this;
    }

    pointer allocate(size_type n)
    {
        #ifdef LOGGING
        std::cout << this << " A [" << n << "]: ";
        #endif
        return static_cast<pointer>(m_pool->allocate(n));
    }

    void deallocate(pointer ptr, size_type n)
    {
        #ifdef LOGGING
        std::cout << this << " D [" << n << "]: " << ptr << std::endl;
        #endif
        m_pool->deallocate(ptr);
    }
    template <typename U, typename... Args>
    void construct(U *p, Args &&...args)
    {
        #ifdef LOGGING
        std::cout << this << " C [" << 1 << "]: " << p << std::endl;
        #endif
        new (p) U(std::forward<Args>(args)...);
    };

    template <typename U>
    void destroy(U *p)
    {
        #ifdef LOGGING
        std::cout << this << " ~ [" << 1 << "]: " << p << std::endl;
        #endif
        p->~U();
    }

    bool operator==(const PoolAllocator& other)
    {
        return m_pool.get() == other.m_pool.get();
    }

    bool operator!=(const PoolAllocator& other)
    {
        return m_pool.get() != other.m_pool.get();
    }
private:
    std::shared_ptr<Pool> m_pool;
    template <typename U, size_t>
    friend class PoolAllocator;
};