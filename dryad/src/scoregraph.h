#pragma once

#include "containers.h"
#include "types.h"

namespace Dryad
{
    //
    // Class ID declaration macro
    //

    constexpr UInt32 FNVHash(const char* str, UInt32 hash = 2166136261)
    {
        while (*str)
        {
            hash = (hash ^ *str++) * 16777619;
        }
        return hash;
    }

    using ClassID = UInt32;
    #define DRYAD_CLASS_ID(className) \
        static constexpr ClassID ID = FNVHash(#className); \
        inline ClassID GetClassID() const { return ID; }


    class ScoreGraph
    {
    public:
        class Node;
        class Link;

        class Node
        {
        public:
            virtual ClassID GetClassID() const = 0;
            Vector<SharedPtr<Node>> edges;
        };

        template <class T>
        void Insert(const SharedPtr<T>& element)
        {
            static_assert(IsBaseOf<Node, T>, "Class must derive from ScoreGraph::Node.");
            if (element != nullptr)
            {
                _Nodes[T::ID].Insert(SharedPtrCast<Node>(element));
            }
        }

        template <class T>
        bool Remove(const SharedPtr<T>& node)
        {
            static_assert(IsBaseOf<Node, T>, "Class must derive from ScoreGraph::Node.");
            if (node != nullptr)
            {
                // Remove node from graph
                return _Nodes[T::ID].Remove(SharedPtrCast<Node>(node));

                // Remove node reference to all edge nodes
                for (SharedPtr<Node>& edgeNode : node.edges)
                {
                    edgeNode.edges.Remove(node);
                }
            }
            return false;
        }

        template <class T, class U>
        bool LinkNodes(SharedPtr<T>& a, SharedPtr<U>& b)
        {
            static_assert(IsBaseOf<Node, T>, "Class must derive from ScoreGraph::Node.");
            static_assert(IsBaseOf<Node, U>, "Class must derive from ScoreGraph::Node.");
            if (a != nullptr && b != nullptr)
            {
                a->edges.PushBack(Link(b));
                b->edges.PushBack(Link(a));
                return true;
            }
            return false;
        }

        UInt32 Size() const
        {
            return _Nodes.Size();
        }

        bool Empty() const
        {
            return _Nodes.Empty();
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
            NodeIterator(MapIterator mapIt, MapIterator mapEnd)
                : NodeIteratorBase(mapIt, mapEnd)
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
            ConstNodeIterator(ConstMapIterator mapIt, ConstMapIterator mapEnd)
                : NodeIteratorBase(mapIt, mapEnd)
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
