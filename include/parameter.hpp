#ifndef PARAMETER_HPP
#define PARAMETER_HPP

#include "node.hpp"
#include "type.hpp"
#include "expression.hpp"
#include "decleration.hpp"

class ParameterDefinition: public Node{
    protected:
    Decleration *decleration;
    ParameterDefinition *parameterNext;
    public:
    ParameterDefinition(Decleration *decleration, ParameterDefinition *parameterNext = nullptr);
    void printASM(Bindings *bindings);
};

class Parameter: public Node{
    protected:
    Expression *expression;
    Parameter *parameterNext;
    public:
    Parameter(Expression *expression, Parameter *parameterNext = nullptr);
};

#endif
