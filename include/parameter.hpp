#ifndef PARAMETER_HPP
#define PARAMETER_HPP

#include "node.hpp"
#include "type.hpp"

class Parameter: public Node{
    protected:
    Type *type;
    Parameter *parameterNext;
};

#endif
