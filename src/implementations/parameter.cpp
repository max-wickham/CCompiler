#include "parameter.hpp"

ParameterDefinition::ParameterDefinition(Decleration *decleration, ParameterDefinition *parameterNext = nullptr){
    this->decleration = decleration;
    this->parameterNext = parameterNext;
}

void ParameterDefinition::printASM(Bindings *bindings){

}