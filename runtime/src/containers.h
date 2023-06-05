#pragma once

#include <vector>
#include <map>
#include "templateutils.h"

namespace Dryad
{
    //
    // Vector
    //
    template <class ValueType>
    class Vector
    {
    private:
        std::vector<ValueType> _vector;

    public:
        template <class... Args>
        Vector(Args... args)
            : _vector(std::forward<Args>(args)...)
        {
        }

        ValueType& operator[](unsigned int index)
        {
            return _vector[index];
        }

        const ValueType& operator[](unsigned int index) const
        {
            return _vector[index];
        }

        void PushBack(const ValueType& item)
        {
            _vector.push_back(item);
        }

        void Clear()
        {
            _vector.clear();
        }

        void Clean(unsigned int size = 0)
        {
            _vector.clear();
            _vector.shrink_to_fit();
            if(size != 0)
            {
                _vector.reserve(size);
            }
        }

        bool Erase(const ValueType& item)
        {
            for(auto itr = _vector.begin(); itr != _vector.end(); itr++)
            {
                if(item == *itr)
                {
                    _vector.erase(itr);
                    return true;
                }
            }
            return false;
        }

        template <class U = ValueType, class = EnableIf<!IsPointer<U>>>
        bool Contains(const U& item) const
        {
            for (const U& content : _vector)
            {
                if (content == item)
                {
                    return true;
                }
            }
            return false;
        }

        template <class U = ValueType, class = EnableIf<IsPointer<U>>>
        bool Contains(U item) const
        {
            for (const U& content : _vector)
            {
                if (content == item)
                {
                    return true;
                }
            }
            return false;
        }

        unsigned int Size() const
        {
            return static_cast<unsigned int>(_vector.size());
        }

        bool Empty() const
        {
            return _vector.empty();
        }

        void Resize(unsigned int size)
        {
            _vector.resize(size);
        }

        void Reserve(unsigned int size)
        {
            _vector.reserve(size);
        }

        using iterator = typename std::vector<ValueType>::iterator;

        auto begin()
        {
            return _vector.begin();
        }

        auto end()
        {
            return _vector.end();
        }

        const auto begin() const
        {
            return _vector.cbegin();
        }

        const auto end() const
        {
            return _vector.cend();
        }
    };

    //
    // Map
    //
    template <class KeyType, class ValueType>
    class Map
    {
    private:
        std::map<KeyType, ValueType> _map;

    public:
        template <class... Args>
        Map(Args... args)
            : _map(std::forward<Args>(args)...)
        {
        }

        ValueType& operator[](const KeyType& key)
        {
            return _map[key];
        }

        bool Find(const KeyType& key, ValueType* outValuePtr)
        {
            auto itr = _map.find(key);
            if(itr != _map.end())
            {
                outValuePtr = &itr->second;
                return true;
            }
            return false;
        }

        unsigned int Size() const
        {
            return static_cast<unsigned int>(_map.size());
        }

        bool Empty() const
        {
            return _map.empty();
        }

        bool Remove(const KeyType& key)
        {
            return _map.erase(key) > 0;
        }

        using iterator = typename std::map<KeyType, ValueType>::iterator;

        auto begin()
        {
            return _map.begin();
        }

        auto end()
        {
            return _map.end();
        }

        auto cbegin() const
        {
            return _map.cbegin();
        }

        auto cend() const
        {
            return _map.cend();
        }

        bool operator==(const Map& other) const
        {
            return _map == other._map;
        }
    };

    //
    // Variant
    //
    template <class... Args>
    class Variant
    {
    private:
        std::variant<Args...> _variant;

    public:
        template <class ValueType>
        Variant(ValueType value)
            : _variant(value)
        {
        }

        template <class ValueType>
        Variant& operator=(ValueType value)
        {
            _variant = value;
            return *this;
        }

        template <class ValueType>
        bool Contains() const
        {
            return std::holds_alternative<ValueType>(_variant);
        }

        template <class ValueType>
        ValueType& Get()
        {
            return std::get<ValueType>(_variant);
        }
    };

    //
    // CircularDeque
    //
    template<typename T>
    class Deque
    {
    public:
        Deque(unsigned int capacity = 8)
            : _vector(capacity)
            , _head(0)
            , _tail(0)
            , _count(0)
        {
        }

        void Clear()
        {
            _vector.Clear();
            _head = _tail = _count = 0;
        }

        void Reset(unsigned int capacity)
        {
             Clear();
             _vector.Clean();
             _vector.Resize(capacity);
        }

        unsigned int Size() const
        {
            return _count;
        }

        bool Empty() const
        {
            return _count == 0;
        }

        void PushBack(const T& value)
        {
            if (_count == _vector.Size())
            {
                ExtendBuffer();
            }
            _vector[_tail] = value;
            _tail = (_tail + 1) % _vector.Size();
            ++_count;
        }

        void PushFront(const T& value)
        {
            if (_count == _vector.Size())
            {
                ExtendBuffer();
            }
            _head = (_head + _vector.Size() - 1) % _vector.Size();
            _vector[_head] = value;
            ++_count;
        }

        void PopFront()
        {
            if (_count > 0)
            {
                _head = (_head + 1) % _vector.Size();
                --_count;
            }
        }

        void PopBack()
        {
            if (_count > 0)
            {
                _tail = (_tail + _vector.Size() - 1) % _vector.Size();
                --_count;
            }
        }

        T& Front()
        {
            return _vector[_head];
        }

        T& Back()
        {
            return _vector[(_tail + _vector.Size() - 1) % _vector.Size()];
        }

        const T& Front() const
        {
            return _vector[_head];
        }

        const T& Back() const
        {
            return _vector[(_tail + _vector.Size() - 1) % _vector.Size()];
        }

        void Insert(const T& item, unsigned int index)
        {
            _vector.insert(_vector.begin() + index, item);
        }

        bool Get(unsigned int index, T& value) const
        {
            if (index >= _count)
            {
                return false;
            }
            value = _vector[(_head + index) % _vector.Size()];
            return true;
        }

        // This could be unsafe if values are modified concurrently
        bool GetPtr(unsigned int index, T*& pointer)
        {
            if (index < _count)
            {
                T* tmp = &_vector[(_head + index) % _vector.Size()];
                if(tmp != nullptr)
                {
                    pointer = tmp;
                    return true;
                }
            }
            return false;
        }

    private:
        void ExtendBuffer()
        {
            unsigned int oldSize = _vector.Size();
            _vector.Resize(oldSize * 2);
            if (_head > _tail)
            {
                for (unsigned int i = 0; i < oldSize - _head; ++i)
                {
                    _vector[oldSize + i] = std::move(_vector[_head + i]);
                }
                _head += oldSize;
            }
        }

        Vector<T> _vector;
        unsigned int _head;
        unsigned int _tail;
        unsigned int _count;
    };
}