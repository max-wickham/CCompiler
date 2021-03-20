#include "parameter.hpp"
#include "node.hpp"
#include "expression.hpp"
#include "type.hpp"
// #include "expression.hpp"
#include "decleration.hpp"
#include <string>
#include <iostream>

ParameterDefinition::ParameterDefinition(Decleration *decleration, ParameterDefinition *parameterNext){
    this->decleration = decleration;
    this->parameterNext = parameterNext;
}

void ParameterDefinition::createScope(Bindings *bindings, ReturnRegisters &returnRegisters){
    //print the asm and load into the bindings the variable
    decleration->type->loadParameter(returnRegisters, bindings);
    //add the variable to the bindings
    bindings->addVariable(decleration);
    if(parameterNext != nullptr){
        parameterNext->createScope(bindings, returnRegisters);
    }
}
void ParameterDefinition::createLabel(std::string &label, Bindings *bindings){
    label += decleration->type->getName();
    if(parameterNext != nullptr){
        label += ",";
        parameterNext->createLabel(label,bindings);
    }
}

Parameter::Parameter(Expression *expression, Parameter *parameterNext){
    this->expression = expression;
    this->parameterNext = parameterNext;
}

void Parameter::calculateTotalMem(int &total, Bindings *bindings){
    //add the total for the current 
    if (dynamic_cast<StringConstant*>(expression) != nullptr){
        expression->printASM(bindings);
        offset = bindings->currentOffset();
        bindings->setOffset(bindings->currentOffset() - 4);
    }
    total += this->expression->getType(bindings)->getSize();
    if(parameterNext != nullptr){
        parameterNext->calculateTotalMem(total,bindings);
    }
}

void Parameter::placeOnStack(Bindings *bindings, int &totalMem, ReturnRegisters &returnRegisters){
    int size = expression->getType(bindings)->getSize();
    //change the bindings offset to that the expression is evaluated to the right place
    bindings->setOffset(bindings->currentOffset() - totalMem);
    //evaluate the expression so it is placed at the top of the stack
    if (dynamic_cast<StringConstant*>(expression) != nullptr){
        //load the value at the offset to the current offset
        std::cout << "lw    $t4, " << offset << "($fp)" << std::endl;
        std::cout << "sw    $t4, " << bindings->currentOffset() << "($fp)" << std::endl;
    } 
    else{
        expression->printASM(bindings);
    }
    //place the value in a register if needed

    expression->getType(bindings)->saveParameter(returnRegisters,bindings);
    //change the top of the stack bacj to where it was so that the next parameter can correctly place itself
    bindings->setOffset(bindings->currentOffset() + totalMem);
    //reduce the total mem needed by the used size
    totalMem -= size;
    if(parameterNext != nullptr){
        parameterNext->placeOnStack(bindings, totalMem, returnRegisters);
    }
}

void Parameter::createLabel(std::string &label, Bindings *bindings){
    label += expression->getType(bindings)->getName();
    if(parameterNext != nullptr){
        label += ",";
        parameterNext->createLabel(label,bindings);
    }
}