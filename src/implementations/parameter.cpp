#include "parameter.hpp"

ParameterDefinition::ParameterDefinition(Decleration *decleration, ParameterDefinition *parameterNext = nullptr){
    this->decleration = decleration;
    this->parameterNext = parameterNext;
}

void ParameterDefinition::createScope(Bindings *bindings, ReturnRegisters &returnRegisters){
    //print the asm and load into the bindings the variable
    decleration->type->loadParameter(returnRegisters, bindings);
    //add the variable to the bindings
    if(parameterNext != nullptr){
        parameterNext->createScope(bindings, returnRegisters);
    }
}

Parameter::Parameter(Expression *expression, Parameter *parameterNext = nullptr){
    this->expression = expression;
    this->parameterNext = parameterNext;
}

void Parameter::placeOnStack(Bindings *bindings){
    //evaluate the expression then place it on the stack
}