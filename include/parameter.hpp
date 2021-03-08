#ifndef PARAMETER_HPP
#define PARAMETER_HPP

#include "node.hpp"
#include "type.hpp"
#include "expression.hpp"
#include "decleration.hpp"

#include <string>

class ParameterDefinition: public Node{
    protected:
    Decleration *decleration;
    ParameterDefinition *parameterNext;
    public:
    ParameterDefinition(Decleration *decleration, ParameterDefinition *parameterNext = nullptr);
    //add the parameters names, which should already be on the stack into the current scope
    void createScope(Bindings *bindings, ReturnRegisters &returnRegisters);
};

class Parameter: public Node{
    protected:
    Expression *expression;
    Parameter *parameterNext;
    public:
    Parameter(Expression *expression, Parameter *parameterNext = nullptr);
    void calculateTotalMem(int &total);
    void placeOnStack(Bindings *bindings, int &totalMem);
    void createLabel(std::string &label);
};

#endif
