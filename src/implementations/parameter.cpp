#include "parameter.hpp"

ParameterDefinition::ParameterDefinition(Decleration *decleration, ParameterDefinition *parameterNext = nullptr){
    this->decleration = decleration;
    this->parameterNext = parameterNext;
}

void ParameterDefinition::createScope(Bindings *bindings){
    //set the memory positions of the parameters in the bindings
    bindings->addVariable(decleration);
    if(parameterNext != nullptr){
        parameterNext->createScope(bindings);
    }
}