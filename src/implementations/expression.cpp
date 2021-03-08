#include "expression.hpp"
#include "parameter.hpp"
#include <iostream>


FunctionCall::FunctionCall(std::string id, Parameter *parameter = nullptr){
    this->id = id;
    this->parameter = parameter;
}

void FunctionCall::printASM(Bindings *bindings, std::string returnVariable){
    //first put the parameters into memory in order,
    if(parameter != nullptr){
        
    }
    //then set the return jump label
    //then jump to the function code
    //then place the return value in the return variable
}

void FunctionCall::printASM(Bindings *bindings, std::string returnRegister){
    //first put the parameters into memory in order,
    //then set the return jump label
    //then jump to the function code
    //then place the value in the return register
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