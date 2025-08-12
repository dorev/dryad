#pragma once

#include <algorithm>

#include "types.h"

namespace Dryad
{
    //
    // Class ID declaration macro
    //

    constexpr unsigned FNVHash(const char* str, unsigned hash = 2166136261)
    {
        while (*str)
        {
            hash = (hash ^ *str++) * 16777619;
        }

        return hash;
    }

    using ClassID = unsigned;
    #define DRYAD_CLASS_ID(className) \
        static constexpr ClassID ID = FNVHash(#className); \
        inline ClassID getClassID() const { return ID; }


    class ScoreGraph
    {
    public:
        class Node
        {
        public:
            virtual ClassID getClassID() const = 0;

            template <class T>
            bool CastTo(T& outReference)
            {
                static_assert(std::is_base_of<Node, T>, "Class must derive from ScoreGraph::Node.");
                if (T::ID == getClassID())
                {
                    outReference = *(static_cast<T*>(this));
                    return true;
                }
                return false;
            }

            Vector<SharedPtr<Node>> edges;
        };

        template <class T>
        bool Insert(const SharedPtr<T>& element)
        {
            static_assert(std::is_base_of<Node, T>, "Class must derive from ScoreGraph::Node.");

            if (element != nullptr)
            {
                _Nodes[T::ID].insert(SharedPtrCast<Node>(element));
                return true;
            }

            return false;
        }

        template <class T>
        bool Contains(const SharedPtr<T>& node)
        {
            static_assert(std::is_base_of<Node, T>, "Class must derive from ScoreGraph::Node.");

            auto mapIt = _Nodes.find(T::ID);

            if (mapIt != _Nodes.end())
            {
                const auto& nodeSet = mapIt->second;
                return nodeSet.find(SharedPtrCast<Node>(node)) != nodeSet.end();
            }

            return false;
        }

        template <class T>
        bool Remove(const SharedPtr<T>& scoreElement)
        {
            static_assert(std::is_base_of<Node, T>, "Class must derive from ScoreGraph::Node.");
            if (scoreElement != nullptr)
            {
                if (Contains(scoreElement))
                {
                    SharedPtr<Node> node = SharedPtrCast<Node>(scoreElement);

                    if (_Nodes[T::ID].erase(node))
                    {
                        // Remove node reference to all edge nodes
                        for (SharedPtr<Node>& edgeNode : node->edges)
                        {
                            auto& edges = edgeNode->edges;
                            edges.erase(std::remove(edges.begin(), edges.end(), node), edges.end());
                        }

                        // Remove all edges
                        node->edges.clear();
                        node->edges.shrink_to_fit();
                    }
                    else
                    {
                        // Unable to remove node even if it was found
                        DRYAD_ERROR("Unable to remove node from ScoreGraph even if it was found.");
                        return false;
                    }
                }

                return true;
            }

            return false;
        }

        template <class T, class U>
        bool LinkNodes(SharedPtr<T>& a, SharedPtr<U>& b)
        {
            static_assert(std::is_base_of<Node, T>, "Class must derive from ScoreGraph::Node.");
            static_assert(std::is_base_of<Node, U>, "Class must derive from ScoreGraph::Node.");

            if (a != nullptr && b != nullptr && Contains(a) && Contains(b))
            {
                a->edges.push_back(b);
                b->edges.push_back(a);

                return true;
            }

            return false;
        }

        unsigned Size() const
        {
            unsigned size = 0;

            for (const auto& [classID, nodeSet] : _Nodes)
            {
                DRYAD_UNUSED(classID);
                size += static_cast<unsigned>(nodeSet.size());
            }

            return size;
        }

        bool Empty() const
        {
            return Size() == 0;
        }

        //
        // Iterator
        //

        template <class MapIterator, class SetIterator>
        class NodeIteratorBase
        {
        public:
            NodeIteratorBase(MapIterator mapIt, MapIterator mapEnd)
                : _MapIterator(mapIt)
                , _MapEnd(mapEnd)
            {
                if (_MapIterator != _MapEnd)
                {
                    _SetIterator = _MapIterator->second.begin();
                    AdvanceIfNecessary();
                }
            }

            NodeIteratorBase& operator++()
            {
                ++_SetIterator;
                AdvanceIfNecessary();
                return *this;
            }

            bool operator!=(const NodeIteratorBase& other) const
            {
                return _MapIterator != other._MapIterator || (_MapIterator != _MapEnd && _SetIterator != other._SetIterator);
            }

        protected:
            void AdvanceIfNecessary()
            {
                while (_MapIterator != _MapEnd && _SetIterator == _MapIterator->second.end())
                {
                    ++_MapIterator;

                    if (_MapIterator != _MapEnd)
                    {
                        _SetIterator = _MapIterator->second.begin();
                    }
                }
            }

            MapIterator _MapIterator;
            MapIterator _MapEnd;
            SetIterator _SetIterator;
        };

        // Regular iterator

        using MapIterator = typename Map<ClassID, Set<SharedPtr<Node>>>::iterator;
        using SetIterator = typename Set<SharedPtr<Node>>::iterator;

        class NodeIterator : public NodeIteratorBase<MapIterator, SetIterator>
        {
        public:
            NodeIterator(MapIterator mapIterator, MapIterator mapEnd)
                : NodeIteratorBase(mapIterator, mapEnd)
            {
            }

            SharedPtr<Node>& operator*() { return const_cast<SharedPtr<Node>&>(*_SetIterator); }
            SharedPtr<Node>* operator->() { return const_cast<SharedPtr<Node>*>(&(*_SetIterator)); }
        };

        NodeIterator begin() { return NodeIterator(_Nodes.begin(), _Nodes.end()); }
        NodeIterator end() { return NodeIterator(_Nodes.end(), _Nodes.end()); }

        // Const iterator

        using ConstMapIterator = typename Map<ClassID, Set<SharedPtr<Node>>>::const_iterator;
        using ConstSetIterator = typename Set<SharedPtr<Node>>::const_iterator;

        class ConstNodeIterator : public NodeIteratorBase<ConstMapIterator, ConstSetIterator>
        {
        public:
            ConstNodeIterator(ConstMapIterator mapIterator, ConstMapIterator mapEnd)
                : NodeIteratorBase(mapIterator, mapEnd)
            {
            }

            const SharedPtr<Node>& operator*() const { return *_SetIterator; }
            const SharedPtr<Node>* operator->() const { return &(*_SetIterator); }
        };

        ConstNodeIterator begin() const { return ConstNodeIterator(_Nodes.begin(), _Nodes.end()); }
        ConstNodeIterator end() const { return ConstNodeIterator(_Nodes.end(), _Nodes.end()); }

    private:
        Map<ClassID, Set<SharedPtr<Node>>> _Nodes;
    };

    using ScoreNode = ScoreGraph::Node;
}
