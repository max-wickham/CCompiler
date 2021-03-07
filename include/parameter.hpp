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
    //add the parameters names, which should already be on the stack into the current scope
    void createScope(Bindings *bindings);
};

class Parameter: public Node{
    protected:
    Expression *expression;
    Parameter *parameterNext;
    public:
    Parameter(Expression *expression, Parameter *parameterNext = nullptr);
};

#endif
