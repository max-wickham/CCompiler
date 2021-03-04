#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include "node.hpp"
#include "type.hpp"
#include "statement.hpp"
#include "parameter.hpp"
#include <string>

class Function: public Node{
    protected:
    Type *type;
    std::string id;
    Statement *statement;
    Parameter *firstParameter;
    public:
    Function(const Type *type, const std::string& id, Statement* statement, Parameter firstParameter);
};

#endif