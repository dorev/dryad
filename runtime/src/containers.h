#pragma once

#include <vector>
#include <map>

namespace Dryad
{
    //
    // Vector
    //
    template <class ValueType>
    class Vector
    {
    private:
        std::vector<ValueType> vector;

    public:
        template <class... Args>
        Vector(Args... args)
            : vector(std::forward<Args>(args)...)
        {
        }

        void PushBack(const ValueType& item)
        {
            vector.push_back(item);
        }

        void Clear(bool deallocate = false)
        {
            vector.clear();
            if(deallocate)
            {
                vector.shrink_to_fit();
            }
        }

        void Clean()
        {
            Clear(true);
        }

        bool Erase(const ValueType& item)
        {
            for(auto itr = vector.begin(); itr != vector.end(); itr++)
            {
                if(item == *itr)
                {
                    vector.erase(itr);
                    return true;
                }
            }
            return false;
        }

        unsigned int Size() const
        {
            return vector.size();
        }

        void Resize(unsigned int size)
        {
            vector.resize(size);
        }

        using iterator = typename std::vector<ValueType>::iterator;

        auto begin()
        {
            return vector.begin();
        }

        auto end()
        {
            return vector.end();
        }
    };

    //
    // Map
    //
    template <class KeyType, class ValueType>
    class Map
    {
    private:
        std::map<KeyType, ValueType> map;

    public:
        template <class... Args>
        Map(Args... args)
            : map(std::forward<Args>(args)...)
        {
        }

        ValueType& operator[](const KeyType& key)
        {
            return map[key];
        }

        bool Find(const KeyType& key, ValueType* outValuePtr)
        {
            auto itr = map.find(key);
            if(itr != map.end())
            {
                outValuePtr = &itr->second;
                return true;
            }
            return false;
        }

        bool Remove(const KeyType& key)
        {
            return map.erase(key) > 0;
        }

        using iterator = typename std::map<KeyType, ValueType>::iterator;

        auto begin()
        {
            return map.begin();
        }

        auto end()
        {
            return map.end();
        }
    };

    //
    // Variant
    //
    template <class... Args>
    class Variant
    {
    private:
        std::variant<Args...> variant;

    public:
        template <class ValueType>
        Variant(ValueType value)
            : variant(value)
        {
        }

        template <class ValueType>
        Variant& operator=(ValueType value)
        {
            variant = value;
            return *this;
        }

        template <class ValueType>
        bool Contains() const
        {
            return std::holds_alternative<ValueType>(variant);
        }

        template <class ValueType>
        ValueType& Get()
        {
            return std::get<ValueType>(variant);
        }
    };

    //
    // CircularDeque
    //
    template<typename T>
    class CircularDeque
    {
    public:
        CircularDeque(unsigned int capacity = 8)
            : vector(capacity)
            , head(0)
            , tail(0)
            , count(0)
        {
        }

        void Clear()
        {
            vector.Clear();
            head = tail = count = 0;
        }

        void Reset(unsigned int capacity)
        {
             Clear();
             vector.Clean();
             vector.Resize(capacity);
        }

        unsigned int Size() const
        {
            return count;
        }

        void PushBack(const T& value)
        {
            if (count == vector.Size())
            {
                ExtendBuffer();
            }
            vector[tail] = value;
            tail = (tail + 1) % vector.Size();
            ++count;
        }

        void PushFront(const T& value)
        {
            if (count == vector.Size())
            {
                ExtendBuffer();
            }
            head = (head + vector.Size() - 1) % vector.Size();
            vector[head] = value;
            ++count;
        }

        void PopFront()
        {
            if (count > 0)
            {
                head = (head + 1) % vector.Size();
                --count;
            }
        }

        void PopBack()
        {
            if (count > 0)
            {
                tail = (tail + vector.Size() - 1) % vector.Size();
                --count;
            }
        }

        T& Front()
        {
            return vector[head];
        }

        T& Back()
        {
            return vector[(tail + vector.Size() - 1) % vector.Size()];
        }

        const T& Front() const
        {
            return vector[head];
        }

        const T& Back() const
        {
            return vector[(tail + vector.Size() - 1) % vector.Size()];
        }

    private:
        void ExtendBuffer()
        {
            unsigned int oldSize = vector.Size();
            vector.Resize(oldSize * 2);
            if (head > tail)
            {
                for (unsigned int i = 0; i < oldSize - head; ++i)
                {
                    vector[oldSize + i] = std::move(vector[head + i]);
                }
                head += oldSize;
            }
        }

        Vector<T> vector;
        unsigned int head;
        unsigned int tail;
        unsigned int count;
    };
}