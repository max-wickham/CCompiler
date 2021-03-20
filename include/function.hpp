#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include "node.hpp"
#include "type.hpp"
#include "statement.hpp"
#include "parameter.hpp"
#include "bindings.hpp"
#include <string>
#include <map>

class Function: public Node{
    protected:
    Type *type;
    std::string id;
    Statement *statement;
    ParameterDefinition *firstParameter;
    public:
    Function(Decleration *decleration, Statement* statement,
        ParameterDefinition *firstParameter);
    void printASM(Bindings *bindings);
    void addToMap(std::map<std::string, Type*> &functionMap);
};

#endif