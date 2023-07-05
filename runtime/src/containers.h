#pragma once

#include <vector>
#include <map>
#include <list>
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

        const auto begin() const
        {
            return _map.cbegin();
        }

        const auto end() const
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

    template <class T>
    class List
    {
        std::list<T> _list;

    public:
        template <class... Args>
        List(Args... args)
            : _list(std::forward<Args>(args)...)
        {
        }

        void Clear()
        {
            _list.clear();
        }

        unsigned int Size() const
        {
            return static_cast<UInt32>(_list.size());
        }

        bool Empty() const
        {
            return _list.empty();
        }

        void PushBack(const T& value)
        {
            _list.push_back(value);
        }

        void PushFront(const T& value)
        {
            _list.push_front(value);
        }

        void PopFront()
        {
            _list.pop_front();
        }

        void PopBack()
        {
            _list.pop_back();
        }

        T& Front()
        {
            return _list.front();
        }

        T& Back()
        {
            return _list.back();
        }

        const T& Front() const
        {
            return _list.front();
        }

        const T& Back() const
        {
            return _list.back();
        }

        void Insert(const T& item, unsigned int index)
        {
            _list.insert()
        }

        auto begin()
        {
            return _list.begin();
        }

        auto end()
        {
            return _list.end();
        }

        const auto begin() const
        {
            return _list.begin();
        }

        const auto end() const
        {
            return _list.end();
        }
    };
}