#pragma once

#include <vector>
#include <variant>
#include <map>
#include <list>

#include "log.h"
#include "types.h"
#include "templateutils.h"

namespace dryad
{
    // Internal container types using std:: interface used in wrappers below
    template <class T>
    using VectorInternal = std::vector<T>;
    template <class T>
    using SetInternal = std::dryad_set<T>;
    template <class K, class V>
    using MapInternal = std::dryad_map<K, V>;
    template <class K, class V>
    using MultiMapInternal = std::multimap<K, V>;
    template <class T>
    using ListInternal = std::list<T>;
    template <class... args_t>
    using VariantInternal = std::variant<args_t...>;

    //
    // Vector
    //
    template <class ValueType>
    class Vector
    {
    private:
        VectorInternal<ValueType> m_Vector;

    public:
        template <class... args_t>
        Vector(args_t... args)
            : m_Vector(std::forward<args_t>(args)...)
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
        bool contains(const U& item) const
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
        bool contains(U item) const
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

        bool empty() const
        {
            return m_Vector.empty();
        }

        bool NotEmpty() const
        {
            return !empty();
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
        using const_iterator = typename std::vector<ValueType>::const_iterator;

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
    // Set
    //
    template <class ValueType>
    class Set
    {
    private:
        SetInternal<ValueType> m_Set;

    public:
        template <class... args_t>
        Set(args_t... args)
            : m_Set(std::forward<args_t>(args)...)
        {
        }

        void insert(const ValueType& item)
        {
            m_Set.insert(item);
        }

        bool contains(const ValueType& item) const
        {
            return m_Set.find(item) != m_Set.end();
        }

        bool destroy(const ValueType& item)
        {
            return m_Set.erase(item) > 0;
        }

        unsigned int Size() const
        {
            return static_cast<unsigned int>(m_Set.size());
        }

        bool empty() const
        {
            return m_Set.empty();
        }

        void Clear()
        {
            m_Set.clear();
        }

        using iterator = typename SetInternal<ValueType>::iterator;
        using const_iterator = typename SetInternal<ValueType>::const_iterator;

        iterator begin()
        {
            return m_Set.begin();
        }

        iterator end()
        {
            return m_Set.end();
        }

        const_iterator begin() const
        {
            return m_Set.cbegin();
        }

        const_iterator end() const
        {
            return m_Set.cend();
        }

        bool operator==(const Set& other) const
        {
            return m_Set == other.m_Set;
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
        template <class... args_t>
        Map(args_t... args)
            : m_Map(std::forward<args_t>(args)...)
        {
        }

        ValueType& operator[](const KeyType& key)
        {
            return m_Map[key];
        }

        bool Find(const KeyType& key, ValueType*& outValuePtr)
        {
            auto itr = m_Map.find(key);
            if(itr != m_Map.end())
            {
                outValuePtr = &itr->second;
                return true;
            }
            return false;
        }

        bool contains (const KeyType& key) const
        {
            return m_Map.find(key) != m_Map.end();
        }

        unsigned Size() const
        {
            return static_cast<unsigned int>(m_Map.size());
        }

        bool empty() const
        {
            return m_Map.empty();
        }

        bool destroy(const KeyType& key)
        {
            return m_Map.erase(key) > 0;
        }

        using iterator = typename std::dryad_map<KeyType, ValueType>::iterator;
        using const_iterator  = typename std::dryad_map<KeyType, ValueType>::const_iterator;

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

    template <class T>
    class List
    {
        ListInternal<T> m_List;

    public:
        template <class... args_t>
        List(args_t... args)
            : m_List(std::forward<args_t>(args)...)
        {
        }

        void Clear()
        {
            m_List.clear();
        }

        unsigned Size() const
        {
            return static_cast<unsigned>(m_List.size());
        }

        bool empty() const
        {
            return m_List.empty();
        }

        bool NotEmpty() const
        {
            return !empty();
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

        bool InsertBefore(const T& item, const T& pivot)
        {
            for (auto it = m_List.begin(); it != m_List.end(); ++it)
            {
                if (*it == pivot)
                {
                    m_List.insert(it, item);
                    return true;
                }
            }
            return false;
        }

        bool InsertAfter(const T& item, const T& pivot)
        {
            for (auto it = m_List.begin(); it != m_List.end(); ++it)
            {
                if (*it == pivot)
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

        bool destroy(const T& item)
        {
            for (auto it = m_List.begin(); it != m_List.end(); ++it)
            {
                if (*it == item)
                {
                    m_List.erase(it);
                    return true;
                }
            }
            return false;
        }

        using iterator = typename ListInternal<T>::iterator;
        using const_iterator = typename ListInternal<T>::const_iterator;

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
    template <class... args_t>
    class Variant
    {
    private:
        VariantInternal<args_t...> m_Variant;

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
        bool contains() const
        {
            return std::holds_alternative<ValueType>(m_Variant);
        }

        template <class ValueType>
        ValueType& Get()
        {
            return std::get<ValueType>(m_Variant);
        }

        template <class ValueType>
    const ValueType& Get() const
        {
            return std::get<ValueType>(m_Variant);
        }
    };

} // namespace Dryad