#include "expression.hpp"
#include "node.hpp"
class Bindings;
#include "type.hpp"
#include "parameter.hpp"
#include <iostream>

FunctionCall::FunctionCall(std::string id, Parameter *parameter){
    this->id = id;
    this->parameter = parameter;
}

void FunctionCall::printASM(Bindings *bindings){
    //first put the parameters into memory in order
    std::string params = "";
    if(parameter != nullptr){
        //create the label string needed to find where to jump to
        parameter->createLabel(params, bindings);
        //calculate the total needed memory
        int mem = 0;
        parameter->calculateTotalMem(mem, bindings);
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
    //add stack pointer to the frame pointer
    std::cout << "addu $sp,$sp,$fp" << std::endl;
    //then jump to the function code
    std::cout << "j     " << id << "(" << params << ")";
    //then set the return jump label
    std::cout << bindings->createLabel("functionReturn") << ":";
    //put the return values into the stack
    bindings->getFunction(id)->processReturn(bindings);
}

void AssignmentOperator::printASM(Bindings *bindings){
    //evaluate the expression
    rightExpression->printASM(bindings);
    leftExpression->getType(bindings)->saveVariable(bindings,((Variable*)leftExpression)->getName());
}

Type*  AssignmentOperator::getType(Bindings *bindings){
    return leftExpression->getType(bindings);
}


void AdditionOperator::printASM(Bindings *bindings){
    //first evaluate left hand expression and place in register, the print asm places the value at the top of the stack
    leftExpression->printASM(bindings);
    leftExpression->getType(bindings)->placeInRegister(bindings, RegisterType::leftReg);
    //then evalute right hand expression and place in register
    rightExpression->printASM(bindings);
    rightExpression->getType(bindings)->placeInRegister(bindings, RegisterType::rightReg);
    //next add the two registers and place in evaluation register
    std::string evalReg = leftExpression->getType(bindings)->getRegister(RegisterType::evaluateReg);
    std::string leftReg = leftExpression->getType(bindings)->getRegister(RegisterType::leftReg);
    std::string rightReg = leftExpression->getType(bindings)->getRegister(RegisterType::rightReg);
    std::cout << "addu   " << evalReg << "," << leftReg << "," << rightReg;
    //move the value from the evaluation register onto the stack
    leftExpression->getType(bindings)->extractFromRegister(bindings,RegisterType::evaluateReg);
}

Type* AdditionOperator::getType(Bindings *bindings){
    return leftExpression->getType(bindings);
}

void SubtractionOperator::printASM(Bindings *bindings){
    //first evaluate left hand expression and place in register, the print asm places the value at the top of the stack
    leftExpression->printASM(bindings);
    leftExpression->getType(bindings)->placeInRegister(bindings, RegisterType::leftReg);
    //then evalute right hand expression and place in register
    rightExpression->printASM(bindings);
    rightExpression->getType(bindings)->placeInRegister(bindings, RegisterType::rightReg);
    //next add the two registers and place in evaluation register
    std::string evalReg = leftExpression->getType(bindings)->getRegister(RegisterType::evaluateReg);
    std::string leftReg = leftExpression->getType(bindings)->getRegister(RegisterType::leftReg);
    std::string rightReg = leftExpression->getType(bindings)->getRegister(RegisterType::rightReg);
    std::cout << "subbu   " << evalReg << "," << leftReg << "," << rightReg;
    //move the value from the evaluation register onto the stack
    leftExpression->getType(bindings)->extractFromRegister(bindings,RegisterType::evaluateReg);
}

Variable::Variable(std::string id){
    this->id = id;
}

Type* Variable::getType(Bindings *bindings){
    return bindings->getVariable(id);
}

std::string Variable::getName(){
    return id;
}

void Variable::printASM(Bindings *bindings){
    bindings->getVariable(id)->placeVariableOnStack(bindings,id);
}

NumberConstant::NumberConstant(int value){
    this->value = value;
}

void NumberConstant::printASM(Bindings *bindings){
    int upper = (value && 4294901760) >> 16;
    int lower = (value && 65535);
    std::cout << "lui    $v0," << upper << std::endl;
    std::cout << "addi    $v0," << lower << std::endl;
    std::cout << "sw    $v0, " << bindings->currentOffset() << "($fp)" << std::endl;
}

Type*  NumberConstant::getType(Bindings *bindings){
    return new Int();
}

BinaryOperatorExpression::BinaryOperatorExpression(Expression *leftExpression, Expression *rightExpression){
    this->leftExpression = leftExpression;
    this->rightExpression = rightExpression;
}