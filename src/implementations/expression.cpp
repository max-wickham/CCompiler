#include "expression.hpp"
#include "parameter.hpp"
#include "type.hpp"
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
        parameter->createLabel(params, bindings);
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
    //add stack pointer to the frame pointer
    std::cout << "addu $sp,$sp,$fp" << std::endl;
    //then jump to the function code
    std::cout << "j     " << id << "(" << params << ")";
    //then set the return jump label
    std::cout << bindings->createLabel("functionReturn") << ":";
    //put the return values into the stack
    bindings->getFunction(id)->processReturn(bindings);
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

NumberConstant::NumberConstant(int value){
    this->value = value;
}

Type*  NumberConstant::getType(Bindings *bindings){
    return new Int();
}

BinaryOperatorExpression::BinaryOperatorExpression(Expression *leftExpression, Expression *rightExpression){
    this->leftExpression = leftExpression;
    this->rightExpression = rightExpression;
}