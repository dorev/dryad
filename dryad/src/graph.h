#pragma once

#include "types.h"

//
// Class ID declaration macro
//

constexpr unsigned fnv_hash(const char* str, unsigned hash = 2166136261)
{
    while (*str)
        hash = (hash ^ *str++) * 16777619;

    return hash;
}

using dryad_class_id = unsigned;
#define DRYAD_CLASS_ID(className) \
    static constexpr dryad_class_id ID = fnv_hash(#className); \
    inline dryad_class_id get_class_id() const { return ID; }

class dryad_graph
{
public:
    class dryad_node
    {
    public:
        virtual dryad_class_id get_class_id() const = 0;

        virtual ~dryad_node()
        {
        }

        template <class T>
        T* get_content()
        {
            static_assert(std::is_base_of<dryad_node, T>::value, "Class must derive from dryad_node.");

            if (T::ID == get_class_id())
                return static_cast<T*>(this);

            return nullptr;
        }

        virtual void remove_edge(dryad_node* node)
        {
            auto edge_iterator = std::find(edges.begin(), edges.end(), node);
            if (edge_iterator != edges.end())
            {
                edges.erase(edge_iterator);
                return;
            }
        }

        template <class T, class function_t>
        void for_each(function_t&& function)
        {
            static_assert(std::is_base_of<dryad_node, T>::value, "Class must derive from dryad_node.");

            for (dryad_node* edge : edges)
            {
                if (edge && edge->get_class_id() == T::ID)
                    function(static_cast<T*>(edge));
            }
        }

        dryad_vector<dryad_node*> edges;
        dryad_graph* graph;
    };

    virtual dryad_graph::~dryad_graph()
    {
        for (dryad_node* node : *this)
            DRYAD_DELETE(node);
    }

    template <class T, class... args_t>
    T* create(args_t ...args)
    {
        static_assert(std::is_base_of<dryad_node, T>::value, "Class must derive from dryad_node.");

        T* content = DRYAD_NEW(T, std::forward<args_t>(args)...);

        if (insert(content))
            return content;
        else
            DRYAD_DELETE(content);

        return nullptr;
    }

    // Once a node is inserted, the graph takes ownership of its lifetime
    template <class T>
    bool insert(T* content)
    {
        static_assert(std::is_base_of<dryad_node, T>::value, "Class must derive from dryad_node.");

        if (content != nullptr && !contains(content))
        {
            dryad_node* node = static_cast<dryad_node*>(content);

            node->graph = this;
            return _nodes[T::ID].insert(node).second;
        }

        return false;
    }

    template <class T>
    bool contains(T* node)
    {
        static_assert(std::is_base_of<dryad_node, T>::value, "Class must derive from dryad_node.");

        if (!node)
            return false;

        auto set_iterator = _nodes.find(T::ID);
        if (set_iterator == _nodes.end())
            return false;

        dryad_set<dryad_node*>& node_set = set_iterator->second;

        return node_set.find(node) != node_set.end();
    }

    template <class T>
    bool destroy(T* content)
    {
        static_assert(std::is_base_of<dryad_node, T>::value, "Class must derive from dryad_node.");

        if (content)
        {
            dryad_node* node = static_cast<dryad_node*>(content);

            auto itr = std::find(_nodes[T::ID].begin(), _nodes[T::ID].end(), node);
            if (itr != _nodes[T::ID].end())
            {
                _nodes[T::ID].erase(itr);

                for (dryad_node* edge : node->edges)
                    edge->remove_edge(node);

                DRYAD_DELETE(node);

                return true;
            }
        }

        return false;
    }

    template <class T, class U>
    bool link(T* a, U* b)
    {
        static_assert(std::is_base_of<dryad_node, T>::value, "Class must derive from dryad_node.");
        static_assert(std::is_base_of<dryad_node, U>::value, "Class must derive from dryad_node.");

        if (a != nullptr && b != nullptr && contains(a) && contains(b))
        {
            if (std::find(a->edges.begin(), a->edges.end(), b) == a->edges.end())
                a->edges.push_back(b);

            if (std::find(b->edges.begin(), b->edges.end(), a) == b->edges.end())
                b->edges.push_back(a);

            return true;
        }

        return false;
    }

    size_t size() const
    {
        size_t size = 0;
        for (const auto& [classID, node_set] : _nodes)
            size += node_set.size();

        return size;
    }

    bool empty() const
    {
        return size() == 0;
    }

    //
    // Iterator
    //

    template <class map_iterator, class set_iterator>
    class node_iterator_base
    {
    public:
        node_iterator_base(map_iterator mapIt, map_iterator mapEnd)
            : _map_iterator(mapIt)
            , _map_end(mapEnd)
        {
            if (_map_iterator != _map_end)
            {
                _set_iterator = _map_iterator->second.begin();
                advance_if_necessary();
            }
        }

        node_iterator_base& operator++()
        {
            ++_set_iterator;
            advance_if_necessary();
            return *this;
        }

        bool operator!=(const node_iterator_base& other) const
        {
            return _map_iterator != other._map_iterator || (_map_iterator != _map_end && _set_iterator != other._set_iterator);
        }

    protected:
        void advance_if_necessary()
        {
            while (_map_iterator != _map_end && _set_iterator == _map_iterator->second.end())
            {
                ++_map_iterator;
                if (_map_iterator != _map_end)
                {
                    _set_iterator = _map_iterator->second.begin();
                }
            }
        }

        map_iterator _map_iterator;
        map_iterator _map_end;
        set_iterator _set_iterator;
    };

    // Regular iterator

    using map_iterator = typename dryad_map<dryad_class_id, dryad_set<dryad_node*>>::iterator;
    using set_iterator = typename dryad_set<dryad_node*>::iterator;

    class node_iterator : public node_iterator_base<map_iterator, set_iterator>
    {
    public:
        node_iterator(map_iterator mapIterator, map_iterator mapEnd)
            : node_iterator_base(mapIterator, mapEnd)
        {
        }

        dryad_node*& operator*() { return const_cast<dryad_node*&>(*_set_iterator); }
        dryad_node** operator->() { return const_cast<dryad_node**>(&(*_set_iterator)); }
    };

    node_iterator begin() { return node_iterator(_nodes.begin(), _nodes.end()); }
    node_iterator end() { return node_iterator(_nodes.end(), _nodes.end()); }

    // Const iterator
    using const_map_iterator = typename dryad_map<dryad_class_id, dryad_set<dryad_node*>>::const_iterator;
    using const_set_iterator = typename dryad_set<dryad_node*>::const_iterator;

    class const_node_iterator : public node_iterator_base<const_map_iterator, const_set_iterator>
    {
    public:
        const_node_iterator(const_map_iterator mapIterator, const_map_iterator mapEnd)
            : node_iterator_base(mapIterator, mapEnd)
        {
        }

        const dryad_node* const& operator*() const { return *_set_iterator; }
        const dryad_node* const* operator->() const { return &(*_set_iterator); }
    };

    const_node_iterator begin() const { return const_node_iterator(_nodes.begin(), _nodes.end()); }
    const_node_iterator end() const { return const_node_iterator(_nodes.end(), _nodes.end()); }

private:
    dryad_map<dryad_class_id, dryad_set<dryad_node*>> _nodes;
};

using dryad_node = dryad_graph::dryad_node;
