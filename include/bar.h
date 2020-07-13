#pragma once

#include "dryadcommon.h"

namespace dryad
{

class degree_node;

class bar
{
public:

    bar();

    inline void insert(degree_node* degree) { _degrees.push_back(degree); }
    inline size_t size() { return _degrees.size(); }

private:

    std::vector<degree_node*> _degrees;

};

}