#pragma once

#include <vector>
#include <map>

namespace Dryad
{
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
                outValuePtr = &(itr.second);
                return true;
            }
            return false;
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
}