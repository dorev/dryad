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


    class Graph : public EnableSharedFromThis<Graph>
    {
    public:
        class Node : public EnableSharedFromThis<Node>
        {
        public:
            virtual ClassID GetClassID() const = 0;

            template <class T>
            bool GetElement(SharedPtr<T>& outReference)
            {
                static_assert(IsBaseOf<Node, T>, "Class must derive from Graph::Node.");

                if (T::ID == GetClassID())
                {
                    outReference = SharedPtrCast<T>(SharedFromThis());
                    return true;
                }
                return false;
            }

            Vector<SharedPtr<Node>> edges;
            SharedPtr<Graph> graph;
        };

        template <class T, class... Args>
        SharedPtr<T> Emplace(Args ...args)
        {
            static_assert(IsBaseOf<Node, T>, "Class must derive from Graph::Node.");

            SharedPtr<T> element = MakeShared<T>(std::forward<Args>(args)...);
            if (Insert(element))
                return element;

            return nullptr;
        }

        template <class T>
        bool Insert(const SharedPtr<T>& element)
        {
            static_assert(IsBaseOf<Node, T>, "Class must derive from Graph::Node.");

            if (element != nullptr)
            {
                SharedPtr<Node> node = SharedPtrCast<Node>(element);
                node->graph = SharedFromThis();
                _Nodes[T::ID].Insert(node);
                return true;
            }
            return false;
        }

        template <class T>
        bool Contains(const SharedPtr<T>& node)
        {
            static_assert(IsBaseOf<Node, T>, "Class must derive from Graph::Node.");
            Set<SharedPtr<Node>>* nodeSet = nullptr;
            return _Nodes.Find(T::ID, nodeSet) && nodeSet->Contains(SharedPtrCast<Node>(node));
        }

        template <class T>
        bool Remove(const SharedPtr<T>& scoreElement)
        {
            static_assert(IsBaseOf<Node, T>, "Class must derive from Graph::Node.");

            if (scoreElement != nullptr)
            {
                Set<SharedPtr<Node>>* nodeSet = nullptr;
                if (Contains(scoreElement))
                {
                    SharedPtr<Node> node = SharedPtrCast<Node>(scoreElement);
                    if (_Nodes[T::ID].Remove(node))
                    {
                        // Remove node reference to all edge nodes
                        for (SharedPtr<Node>& edgeNode : node->edges)
                        {
                            edgeNode->edges.Erase(node);
                        }

                        // Remove all edges
                        node->edges.Clean();
                    }
                    else
                    {
                        // Unable to remove node even if it was found
                        DRYAD_ERROR("Unable to remove node from Graph even if it was found.");
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
            static_assert(IsBaseOf<Node, T>, "Class must derive from Graph::Node.");
            static_assert(IsBaseOf<Node, U>, "Class must derive from Graph::Node.");

            if (a != nullptr && b != nullptr && Contains(a) && Contains(b))
            {
                a->edges.PushBack(b);
                b->edges.PushBack(a);
                return true;
            }
            return false;
        }

        UInt32 Size() const
        {
            UInt32 size = 0;
            for (const auto& [classID, nodeSet] : _Nodes)
            {
                size += nodeSet.Size();
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

    using Node = Graph::Node;
}
