#include "expression.hpp"
#include <iostream>


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

void BinaryOperatorExpression::print() const{
    leftExpression->print();
    std::cout<< this->getOperator();
    rightExpression->print();
}

std::string AssignmentOperator::getOperator() const{
    return "=";
}

std::string AdditionOperator::getOperator() const{
    return "+";
}