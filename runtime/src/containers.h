#pragma once

#include <vector>
#include <variant>
#include <map>
#include <list>

#include "types.h"
#include "templateutils.h"

namespace Dryad
{
    // Internal container types using std:: interface used in wrappers below
    template <class T>
    using VectorInternal = std::vector<T>;
    template <class K, class V>
    using MapInternal = std::map<K, V>;
    template <class K, class V>
    using MultiMapInternal = std::multimap<K, V>;
    template <class T>
    using ListInternal = std::list<T>;
    template <class... Args>
    using VariantInternal = std::variant<Args...>;

    //
    // Vector
    //
    template <class ValueType>
    class Vector
    {
    private:
        VectorInternal<ValueType> m_Vector;

    public:
        template <class... Args>
        Vector(Args... args)
            : m_Vector(std::forward<Args>(args)...)
        {
        }

        ValueType& operator[](unsigned int index)
        {
            return m_Vector[index];
        }

        const ValueType& operator[](unsigned int index) const
        {
            return m_Vector[index];
        }

        void PushBack(const ValueType& item)
        {
            m_Vector.push_back(item);
        }

        void Clear()
        {
            m_Vector.clear();
        }

        void Clean(unsigned int size = 0)
        {
            m_Vector.clear();
            m_Vector.shrink_to_fit();
            if(size != 0)
            {
                m_Vector.reserve(size);
            }
        }

        bool Erase(const ValueType& item)
        {
            for(auto itr = m_Vector.begin(); itr != m_Vector.end(); itr++)
            {
                if(item == *itr)
                {
                    m_Vector.erase(itr);
                    return true;
                }
            }
            return false;
        }

        template <class U = ValueType, class = EnableIf<!IsPointer<U>>>
        bool Contains(const U& item) const
        {
            for (const U& content : m_Vector)
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
            for (const U& content : m_Vector)
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
            return static_cast<unsigned int>(m_Vector.size());
        }

        bool Empty() const
        {
            return m_Vector.empty();
        }

        void Resize(unsigned int size)
        {
            m_Vector.resize(size);
        }

        void Reserve(unsigned int size)
        {
            m_Vector.reserve(size);
        }

        using iterator = typename std::vector<ValueType>::iterator;

        auto begin()
        {
            return m_Vector.begin();
        }

        auto end()
        {
            return m_Vector.end();
        }

        const auto begin() const
        {
            return m_Vector.cbegin();
        }

        const auto end() const
        {
            return m_Vector.cend();
        }
    };

    //
    // Map
    //
    template <class KeyType, class ValueType>
    class Map
    {
    private:
        MapInternal<KeyType, ValueType> m_Map;

    public:
        template <class... Args>
        Map(Args... args)
            : m_Map(std::forward<Args>(args)...)
        {
        }

        ValueType& operator[](const KeyType& key)
        {
            return m_Map[key];
        }

        bool Find(const KeyType& key, ValueType* outValuePtr)
        {
            auto itr = m_Map.find(key);
            if(itr != m_Map.end())
            {
                outValuePtr = &itr->second;
                return true;
            }
            return false;
        }

        UInt32 Size() const
        {
            return static_cast<unsigned int>(m_Map.size());
        }

        bool Empty() const
        {
            return m_Map.empty();
        }

        bool Remove(const KeyType& key)
        {
            return m_Map.erase(key) > 0;
        }

        using iterator = typename std::map<KeyType, ValueType>::iterator;

        auto begin()
        {
            return m_Map.begin();
        }

        auto end()
        {
            return m_Map.end();
        }

        const auto begin() const
        {
            return m_Map.cbegin();
        }

        const auto end() const
        {
            return m_Map.cend();
        }

        bool operator==(const Map& other) const
        {
            return m_Map == other.m_Map;
        }
    };

    //
    // Multimap
    //

    template <class KeyType, class ValueType>
    class MultiMap
    {
    private:
        MultiMapInternal<KeyType, ValueType> m_MultiMap;

    public:
        template <class... Args>
        MultiMap(Args... args)
            : m_MultiMap(std::forward<Args>(args)...)
        {
        }

        ValueType& operator[](const KeyType& key)
        {
            return m_MultiMap[key];
        }

        bool Find(const KeyType& key, ValueType* outValuePtr)
        {
            auto itr = m_MultiMap.find(key);
            if(itr != m_MultiMap.end())
            {
                outValuePtr = &itr->second;
                return true;
            }
            return false;
        }

        UInt32 Size() const
        {
            return static_cast<unsigned int>(m_MultiMap.size());
        }

        bool Empty() const
        {
            return m_MultiMap.empty();
        }

        bool Remove(const KeyType& key)
        {
            return m_MultiMap.erase(key) > 0;
        }

        using iterator = typename std::multimap<KeyType, ValueType>::iterator;

        auto begin()
        {
            return m_MultiMap.begin();
        }

        auto end()
        {
            return m_MultiMap.end();
        }

        const auto begin() const
        {
            return m_MultiMap.cbegin();
        }

        const auto end() const
        {
            return m_MultiMap.cend();
        }

        bool operator==(const MultiMap& other) const
        {
            return m_MultiMap == other.m_MultiMap;
        }
    };

    template <class T>
    class List
    {
        ListInternal<T> m_List;

    public:
        template <class... Args>
        List(Args... args)
            : m_List(std::forward<Args>(args)...)
        {
        }

        void Clear()
        {
            m_List.clear();
        }

        UInt32 Size() const
        {
            return static_cast<UInt32>(m_List.size());
        }

        bool Empty() const
        {
            return m_List.empty();
        }

        void PushBack(const T& value)
        {
            m_List.push_back(value);
        }

        void PushFront(const T& value)
        {
            m_List.push_front(value);
        }

        void PopFront()
        {
            m_List.pop_front();
        }

        void PopBack()
        {
            m_List.pop_back();
        }

        T& Front()
        {
            return m_List.front();
        }

        T& Back()
        {
            return m_List.back();
        }

        const T& Front() const
        {
            return m_List.front();
        }

        const T& Back() const
        {
            return m_List.back();
        }

        bool InsertBefore(const T& item, const T& reference)
        {
            for (auto it = m_List.begin(); it != m_List.end(); ++it)
            {
                if (*it == reference)
                {
                    m_List.insert(it, item);
                    return true;
                }
            }
            return false;
        }

        bool InsertAfter(const T& item, const T& reference)
        {
            for (auto it = m_List.begin(); it != m_List.end(); ++it)
            {
                if (*it == reference)
                {
                    // If the iterator is not at the end of the list, advance it one position before inserting.
                    // Otherwise, just insert at the current position which would be equivalent to inserting at the end.
                    if (std::next(it) != m_List.end())
                    {
                        ++it;
                    }
                    m_List.insert(it, item);
                    return true;
                }
            }
            return false;
        }

        auto begin()
        {
            return m_List.begin();
        }

        auto end()
        {
            return m_List.end();
        }

        const auto begin() const
        {
            return m_List.begin();
        }

        const auto end() const
        {
            return m_List.end();
        }
    };

    //
    // Variant
    //
    template <class... Args>
    class Variant
    {
    private:
        VariantInternal<Args...> m_Variant;

    public:
        template <class ValueType>
        Variant(ValueType value)
            : m_Variant(value)
        {
        }

        template <class ValueType>
        Variant& operator=(ValueType value)
        {
            m_Variant = value;
            return *this;
        }

        template <class ValueType>
        bool Contains() const
        {
            return std::holds_alternative<ValueType>(m_Variant);
        }

        template <class ValueType>
        ValueType& Get()
        {
            return std::get<ValueType>(m_Variant);
        }
    };

} // namespace Dryad