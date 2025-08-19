#pragma once

#include "types.h"
#include <algorithm>

namespace Dryad
{

    // Class ID declaration macro
    constexpr unsigned FNVHash(const char* str, unsigned hash = 2166136261)
    {
        while (*str)
            hash = (hash ^ *str++) * 16777619;

        return hash;
    }

    using ClassID = unsigned;
    #define DRYAD_NODE_CLASS_ID(class_name) \
        static constexpr ClassID ID = FNVHash(#class_name); \
        static constexpr const char* NAME = #class_name; \
        inline ClassID getClassID() const { return ID; } \

    class Graph
    {
    public:
        class Node
        {
        public:
            virtual ClassID getClassID() const = 0;

            virtual ~Node()
            {
            }

            template <class T>
            T* get()
            {
                static_assert(std::is_base_of<Node, T>::value, "Class must derive from Node.");

                if (T::ID == getClassID())
                    return static_cast<T*>(this);

                return nullptr;
            }

            bool removeEdge(Node* node)
            {
                auto edge_iterator = std::find(edges.begin(), edges.end(), node);
                if (edge_iterator != edges.end())
                {
                    edges.erase(edge_iterator);
                    return true;
                }

                return false;
            }

            // Loop through all edges of type T.
            template <class T, class function_t>
            void forEachEdge(function_t&& function)
            {
                static_assert(std::is_base_of<Node, T>::value, "Class must derive from Node.");

                for (Node* edge : edges)
                {
                    if (edge && edge->getClassID() == T::ID)
                        function(static_cast<T*>(edge));
                }
            }

            // Loop trough all edges of typpe T, breaks when returning `true`.
            template <class T, class function_t>
            void forEachEdgeBreakable(function_t&& function)
            {
                static_assert(std::is_base_of<Node, T>::value, "Class must derive from Node.");

                for (Node* edge : edges)
                {
                    if (edge && edge->getClassID() == T::ID)
                    {
                        if (function(static_cast<T*>(edge)))
                            break;
                    }
                }
            }

            template <class T>
            T* findFirstEdge()
            {
                static_assert(std::is_base_of<Node, T>::value, "Class must derive from Node.");

                for (Node* edge : edges)
                {
                    if (edge && edge->getClassID() == T::ID)
                        return static_cast<T*>(edge);
                }

                return nullptr;
            }

            Vector<Node*> edges;
            Graph* graph;
        };

        virtual ~Graph()
        {
            for (Node* node : *this)
                DRYAD_DELETE(node);
        }

        template <class T, class... args_t>
        T* create(args_t ...args)
        {
            static_assert(std::is_base_of<Node, T>::value, "Class must derive from Node.");

            T* content = DRYAD_NEW(T, std::forward<args_t>(args)...);

            if (insert(content))
            {
                return content;
            }
            else
            {
                DRYAD_ERROR("Unable to create %s node in graph.", T::NAME);
                DRYAD_DELETE(content);
                return nullptr;
            }
        }

        // Once a node is inserted, the graph takes ownership of its lifetime
        template <class T>
        bool insert(T* content)
        {
            static_assert(std::is_base_of<Node, T>::value, "Class must derive from Node.");

            if (content != nullptr && !contains(content))
            {
                Node* node = static_cast<Node*>(content);

                node->graph = this;
                return nodes[T::ID].insert(node).second;
            }

            return false;
        }

        template <class T, class function_t>
        void forEachEdge(function_t&& function)
        {
            static_assert(std::is_base_of<Node, T>::value, "Class must derive from Node.");

            for (Node* node : nodes[T::ID])
                function(static_cast<T*>(node));
        }

        template <class T, class function_t>
        void forEachEdgeBreakable(function_t&& function)
        {
            static_assert(std::is_base_of<Node, T>::value, "Class must derive from Node.");

            for (Node* node : nodes[T::ID])
            {
                if (function(static_cast<T*>(node)))
                    break;
            }
        }

        template <class T>
        bool contains(T* node)
        {
            static_assert(std::is_base_of<Node, T>::value, "Class must derive from Node.");

            if (!node)
                return false;

            auto SetIterator = nodes.find(T::ID);
            if (SetIterator == nodes.end())
                return false;

            Set<Node*>& node_set = SetIterator->second;

            return node_set.find(node) != node_set.end();
        }

        template <class T>
        bool destroy(T* content)
        {
            static_assert(std::is_base_of<Node, T>::value, "Class must derive from Node.");

            if (content)
            {
                Node* node = static_cast<Node*>(content);

                auto itr = std::find(nodes[T::ID].begin(), nodes[T::ID].end(), node);
                if (itr != nodes[T::ID].end())
                {
                    nodes[T::ID].erase(itr);

                    for (Node* edge : node->edges)
                        edge->removeEdge(node);

                    node->edges.clear();
                    node->graph = nullptr;

                    return true;
                }
            }

            return false;
        }

        template <class T, class U>
        bool link(T* a, U* b)
        {
            static_assert(std::is_base_of<Node, T>::value, "Class must derive from Node.");
            static_assert(std::is_base_of<Node, U>::value, "Class must derive from Node.");

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
            for (const auto& [classID, node_set] : nodes)
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

        template <class MapIterator, class SetIterator>
        class NodeIteratorBase
        {
        public:
            NodeIteratorBase(MapIterator mapIterator, MapIterator mapEnd)
                : mapIterator(mapIterator)
                , mapEnd(mapEnd)
            {
                if (mapIterator != mapEnd)
                {
                    setIterator = mapIterator->second.begin();
                    advanceIfNecessary();
                }
            }

            NodeIteratorBase& operator++()
            {
                ++setIterator;
                advanceIfNecessary();
                return *this;
            }

            bool operator!=(const NodeIteratorBase& other) const
            {
                return mapIterator != other.mapIterator || (mapIterator != mapEnd && setIterator != other.setIterator);
            }

        protected:
            void advanceIfNecessary()
            {
                while (mapIterator != mapEnd && setIterator == mapIterator->second.end())
                {
                    ++mapIterator;
                    if (mapIterator != mapEnd)
                    {
                        setIterator = mapIterator->second.begin();
                    }
                }
            }

            MapIterator mapIterator;
            MapIterator mapEnd;
            SetIterator setIterator;
        };

        // Regular iterator

        using MapIterator = typename Map<ClassID, Set<Node*>>::iterator;
        using SetIterator = typename Set<Node*>::iterator;

        class NodeIterator : public NodeIteratorBase<MapIterator, SetIterator>
        {
        public:
            NodeIterator(MapIterator mapIterator, MapIterator mapEnd)
                : NodeIteratorBase(mapIterator, mapEnd)
            {
            }

            Node*& operator*() { return const_cast<Node*&>(*setIterator); }
            Node** operator->() { return const_cast<Node**>(&(*setIterator)); }
        };

        NodeIterator begin() { return NodeIterator(nodes.begin(), nodes.end()); }
        NodeIterator end() { return NodeIterator(nodes.end(), nodes.end()); }

        // Const iterator

        using ConstMapIterator = typename Map<ClassID, Set<Node*>>::const_iterator;
        using ConstSetIterator = typename Set<Node*>::const_iterator;

        class ConstNodeIterator : public NodeIteratorBase<ConstMapIterator, ConstSetIterator>
        {
        public:
            ConstNodeIterator(ConstMapIterator mapIterator, ConstMapIterator mapEnd)
                : NodeIteratorBase(mapIterator, mapEnd)
            {
            }

            const Node* const& operator*() const { return *setIterator; }
            const Node* const* operator->() const { return &(*setIterator); }
        };

        ConstNodeIterator begin() const { return ConstNodeIterator(nodes.begin(), nodes.end()); }
        ConstNodeIterator end() const { return ConstNodeIterator(nodes.end(), nodes.end()); }

    private:
        Map<ClassID, Set<Node*>> nodes;
    };

    using Node = Graph::Node;

} // namespace Dryad
