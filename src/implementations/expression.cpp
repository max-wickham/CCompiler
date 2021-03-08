#include "expression.hpp"
#include "parameter.hpp"
#include <iostream>


FunctionCall::FunctionCall(std::string id, Parameter *parameter = nullptr){
    this->id = id;
    this->parameter = parameter;
}

void FunctionCall::printASM(Bindings *bindings){
    //first put the parameters into memory in order
    std::string params = "";
    if(parameter != nullptr){
        //create the label string needed to find where to jump to
        parameter->createLabel(params);
        //calculate the total needed memory
        int mem = 0;
        parameter->calculateTotalMem(mem);
        //place the parameters on the stack
        parameter->placeOnStack(bindings, mem);
        //set the new bindings offset
        bindings->setOffset(bindings->currentOffset() + mem);
    }
    //set the sp to the current bindings offset
    int upper = (bindings->currentOffset() && 4294901760) >> 16;
    int lower = (bindings->currentOffset() && 65535);
    std::cout << "lui    $sp," << upper << std::endl;
    std::cout << "addi    $sp," << lower << std::endl;
    //then jump to the function code
    std::cout << "j     " << id << "(" << params << ")";
    //then set the return jump label
    std::cout << bindings->createLabel("functionReturn") << ":";
}

Variable::Variable(std::string id){
    this->id = id;
}

NumberConstant::NumberConstant(int value){
    this->value = value;
}

BinaryOperatorExpression::BinaryOperatorExpression(Expression *leftExpression, Expression *rightExpression){
    this->leftExpression = leftExpression;
    this->rightExpression = rightExpression;
}