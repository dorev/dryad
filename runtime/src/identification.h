#pragma once

#include "types.h"

namespace Dryad
{
    class Identifiable
    {
    public:
        const IdType id;

    public:
        Identifiable()
            : id(_sNextId++)
        {
        }

    private:
        static inline IdType _sNextId = 0;
    };

    class Named : public Identifiable
    {
    public:
        String name;

    public:
        Named(const char* itemName = "")
            : name(itemName)
        {
        }
    };
}