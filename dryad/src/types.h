#pragma once

#include <string>
#include <memory>

namespace Dryad
{
    //
    // Aliases for common types
    //

    using Byte = char;
    using Int8 = char;
    using UInt8 = unsigned char;
    using UInt32 = unsigned int;
    using Int32 = int;
    using String = std::string;

    //
    // Shared pointer alias
    //

    template <class T>
    using SharedPtr = std::shared_ptr<T>;
    
    template <class T, class... Args>
    auto MakeShared(Args&&... args) -> SharedPtr<T>
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    template <class T, class U>
    SharedPtr<T> SharedPtrCast(const SharedPtr<U>& ptr) \
    {
        return std::static_pointer_cast<T>(ptr);
    }

    template <class T>
    class EnableSharedFromThis : public std::enable_shared_from_this<T>
    {
    public:
        SharedPtr<T> SharedFromThis()
        {
            return this->shared_from_this();
        }

        SharedPtr<const T> SharedFromThis() const
        {
            return this->shared_from_this();
        }
    };

    //
    // Dryad basic types
    //

    using IdType = UInt32;
    using NoteValue = UInt8;
    using NoteRelativeValue = Int8;
    using NoteVelocity = UInt8;
    using Tempo = float;
    using RealTime = float;
    using ScoreTime = UInt32;

    //
    // Function wrappers
    //

    template<typename ReturnType, typename... Args>
    class FreeFunction
    {
    public:
        using FunctionType = ReturnType(*)(Args...);

        FreeFunction(FunctionType function)
            : m_Function(function)
        {
        }

        ReturnType operator()(Args... args)
        {
            return m_Function(std::forward<Args>(args)...);
        }

    private:
        FunctionType m_Function;
    };

    template<typename ClassType, typename ReturnType, typename... Args>
    class MemberFunction
    {
    public:
        using MemberFunctionType = ReturnType(ClassType::*)(Args...);
        MemberFunction(ClassType* object, MemberFunctionType memberFunction)
            : m_Object(object)
            , m_MemberFunction(memberFunction)
        {
        }

        ReturnType operator()(Args... args)
        {
            return (m_Object->*m_MemberFunction)(std::forward<Args>(args)...);
        }

    private:
        ClassType* m_Object;
        MemberFunctionType m_MemberFunction;
    };

    // Deduction guide
    template<typename ClassType, typename ReturnType, typename... Args>
    MemberFunction(ClassType*, ReturnType(ClassType::*)(Args...))->MemberFunction<ClassType, ReturnType, Args...>;
}