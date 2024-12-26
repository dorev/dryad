#pragma once

#include <atomic>
#include <memory>
#include <mutex>

namespace dryad
{
    template<typename T, size_t PoolSize>
    class LockFreePool
    {
    public:
        LockFreePool()
            : m_NullPointerIndex(-1)
            , m_Top(0)
        {
            // Initialize the indices to form a linked list of free spots.
            // The last index will point to the m_NullPointerIndex to indicate the end.
            for (size_t i = 0; i < PoolSize - 1; ++i)
            {
                m_Indices[i].store(i + 1, std::memory_order_relaxed);
            }
            m_Indices[PoolSize - 1].store(m_NullPointerIndex, std::memory_order_relaxed);
        }

        T* Create()
        {
            // Load the current top index.
            size_t currentIndex = m_Top.load(std::memory_order_relaxed);

            // Try to get a free spot. If successful, construct the object at that spot.
            while (currentIndex != m_NullPointerIndex)
            {
                if (m_Top.compare_exchange_weak(currentIndex, m_Indices[currentIndex].load(std::memory_order_relaxed)))
                {
                    return new (&m_Data[currentIndex]) T();
                }
            }

            // Pool is full
            return nullptr;
        }

        void Destroy(T* object)
        {
            // Calculate the index of the object in the pool.
            size_t index = (reinterpret_cast<uintptr_t>(object) - reinterpret_cast<uintptr_t>(m_Data)) / sizeof(T);

            // Explicitly call the object's destructor.
            object->~T();

            // Push the now-free index onto the top of the linked list of free spots.
            size_t currentTop = m_Top.load(std::memory_order_relaxed);
            do
            {
                m_Indices[index].store(currentTop, std::memory_order_relaxed);
            }
            while (!m_Top.compare_exchange_weak(currentTop, index));
        }

    private:
        static const size_t m_NullPointerIndex;

        std::atomic<size_t> m_Indices[PoolSize];
        std::atomic<size_t> m_Top;
        char m_Data[PoolSize][sizeof(T)];
    };

    template<typename T, size_t PoolSize>
    class ExpandingLockFreePool
    {
    public:
        ExpandingLockFreePool()
            : m_NullPointerIndex(-1)
            , m_Top(0)
        {
            m_Blocks.push_back(std::make_unique<Block>());
            InitializeBlock(m_Blocks.back().get());
        }

        template <typename... args_t>
        T* Create(args_t&&... args)
        {
            // Load the current top index.
            size_t currentIndex = m_Top.load(std::memory_order_relaxed);

            // If the pool is exhausted, try expanding it.
            if (currentIndex == m_NullPointerIndex)
            {
                ExpandPool();
                currentIndex = m_Top.load(std::memory_order_relaxed);
            }

            if (currentIndex == m_NullPointerIndex)
            {
                // Even after trying to expand, pool is still full.
                // Allocation might have failed?
                return nullptr;
            }

            // Try to get a free spot and construct the object at that spot.
            if (m_Top.compare_exchange_strong(currentIndex, m_Indices[currentIndex].load(std::memory_order_relaxed)))
            {
                return new (&GetStorageLocation(currentIndex)) T(std::forward<args_t>(args)...);
            }

            // Something unexpected happened.
            return nullptr;
        }

        void Destroy(T* object)
        {
            size_t blockIndex;
            for (blockIndex = 0; blockIndex < m_Blocks.size(); ++blockIndex)
            {
                char* blockStart = &m_Blocks[blockIndex]->data[0];
                char* blockEnd = blockStart + sizeof(T) * PoolSize;
                if (reinterpret_cast<char*>(object) >= blockStart && reinterpret_cast<char*>(object) < blockEnd)
                {
                    break;
                }
            }

            // If we didn't find the object in any block, there's an error.
            if (blockIndex == m_Blocks.size())
            {
                return; // or throw an exception.
            }

            size_t dataIndex = (reinterpret_cast<uintptr_t>(object) - reinterpret_cast<uintptr_t>(&m_Blocks[blockIndex]->data[0])) / sizeof(T);
            size_t totalIndex = blockIndex * PoolSize + dataIndex;

            object->~T();

            size_t currentTop = m_Top.load(std::memory_order_relaxed);
            do
            {
                m_Blocks[blockIndex]->indices[dataIndex].store(currentTop, std::memory_order_relaxed);
            }
            while (!m_Top.compare_exchange_weak(currentTop, totalIndex));
        }

    private:
        struct Block
        {
            std::atomic<size_t> indices[PoolSize];
            char data[PoolSize][sizeof(T)];
        };

        void InitializeBlock(Block* block)
        {
            size_t baseIndex = m_Blocks.size() * PoolSize;
            for (size_t i = 0; i < PoolSize - 1; ++i)
            {
                block->indices[i].store(baseIndex + i + 1, std::memory_order_relaxed);
            }
            block->indices[PoolSize - 1].store(m_NullPointerIndex, std::memory_order_relaxed);
        }

        void ExpandPool()
        {
            std::lock_guard<std::mutex> lock(m_ExpansionMutex);

            // If another thread already expanded the pool, exit.
            if (m_Top.load() == m_NullPointerIndex)
            {
                m_Blocks.push_back(std::make_unique<Block>());
                InitializeBlock(m_Blocks.back().get());

                // Reset the top to the beginning of the new block.
                m_Top.store((m_Blocks.size() - 1) * PoolSize);
            }
        }

    private:
        static const size_t m_NullPointerIndex;
        std::atomic<size_t> m_Top;
        std::vector<std::unique_ptr<Block>> m_Blocks;
        std::mutex m_ExpansionMutex;
    };
}