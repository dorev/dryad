#pragma once

#include <variant>
#include <vector>
#include <map>
#include <deque>
#include <string>
#include <memory>

namespace Dryad
{
    //
    // Aliases for standard types
    //

    using Byte = char;
    using Int8 = char;
    using UInt8 = unsigned char;
    using UInt32 = unsigned int;
    using String = std::string;
    template <class T>
    using Vector = std::vector<T>;
    template <class K, class V>
    using Map = std::map<K, V>;
    template <class T>
    using Deque = std::deque<T>;
    template <class T>
    using SharedPtr = std::shared_ptr<T>;
    template <class... T>
    using Variant = std::variant<T...>;

    //
    // Dryad basic types
    //

    using Tempo = float;
    using Time = float;
    using IdType = UInt32;
    using NoteValue = UInt8;
    using ScaleOffsets = NoteValue[7];

    struct Fraction
    {
        UInt32 numerator;
        UInt32 denominator;
    };

    struct MusicTime : Fraction
    {
        Time time;
        Tempo tempo;
        bool withinTempoChange;
        Fraction tempoChangeProgress;
    };

    using Duration = MusicTime;
    using Position = MusicTime;
    using TimeSignature = Fraction;

    //
    // Forward declarations
    //

    class Node;
    using NodePtr = SharedPtr<Node>;
    class Edge;
    using EdgePtr = SharedPtr<Edge>;
    class Graph;
    using GraphPtr = SharedPtr<Graph>;
    class Motif;
    using MotifPtr = SharedPtr<Motif>;
    class Interlude;
    using InterludePtr = SharedPtr<Interlude>;
    class Scale;
    using ScalePtr = SharedPtr<Scale>;
    class Session;
    using SessionPtr = SharedPtr<Session>;
    class HarmonicContext;
    using HarmonicContextPtr = SharedPtr<HarmonicContext>;
}