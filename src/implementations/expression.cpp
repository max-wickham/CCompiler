#include "expression.hpp"
#include "node.hpp"
#include "type.hpp"
#include "parameter.hpp"
#include <iostream>
class Bindings;

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

void EqualityOperator::printASM(Bindings *bindings){
    //evalute the left expression and place on stack
    std::string labelEnd = bindings->createLabel("emd");
    std::string labelPass = bindings->createLabel("pass"); 
    Type* leftType = leftExpression->getType(bindings);
    Type* rightType = rightExpression->getType(bindings);
    leftExpression->printASM(bindings);
    leftType->placeInRegister(bindings, RegisterType::leftReg);
    rightExpression->printASM(bindings);
    rightType->placeInRegister(bindings, RegisterType::rightReg);
    leftType->beq(bindings,RegisterType::leftReg, RegisterType::rightReg, labelPass);
    //std::cout << "beq   " << leftType->getRegister(RegisterType::leftReg) <<
    //    ", " << rightType->getRegister(RegisterType::rightReg) << labelPass <<std::endl;
    std::cout << "li    " << "$v0, 0" <<std::endl;
    std::cout << "j     " << labelEnd <<std::endl;
    std::cout << ".global " << labelPass <<std::endl;
    std::cout << labelPass << ":" <<std::endl;
    std::cout << "li    " << "$v0, 0" <<std::endl;
    std::cout << ".global " << labelEnd <<std::endl;
    std::cout << labelEnd << ":" <<std::endl;
    std::cout << "sw    $v0, " << bindings->currentOffset() << "($fp)" << std::endl;
}

void LessThanOperator::printASM(Bindings *bindings){
    Type* leftType = leftExpression->getType(bindings);
    Type* rightType = rightExpression->getType(bindings);
    leftExpression->printASM(bindings);
    leftType->placeInRegister(bindings, RegisterType::leftReg);
    rightExpression->printASM(bindings);
    rightType->placeInRegister(bindings, RegisterType::rightReg);
    std::cout << "slt    $v0," << leftType->getRegister(RegisterType::leftReg) << "," <<
        rightType->getRegister(RegisterType::rightReg) <<std::endl;
    std::cout << "sw    $v0, " << bindings->currentOffset() << "($fp)" << std::endl;
}
//TODO change beq
void LessThanEqualOperator::printASM(Bindings *bindings){
    std::string labelEnd = bindings->createLabel("end");
    std::string labelPass = bindings->createLabel("pass"); 
    Type* leftType = leftExpression->getType(bindings);
    Type* rightType = rightExpression->getType(bindings);
    leftExpression->printASM(bindings);
    leftType->placeInRegister(bindings, RegisterType::leftReg);
    rightExpression->printASM(bindings);
    rightType->placeInRegister(bindings, RegisterType::rightReg);
    std::cout << "slt    $v0," << leftType->getRegister(RegisterType::leftReg) << "," <<
        rightType->getRegister(RegisterType::rightReg) <<std::endl;
    std::cout << "li   $v1,1, "<< labelEnd <<std::endl;
    std::cout << "beq   $v0,$v1, "<< labelEnd <<std::endl;
    std::cout << "beq   " << leftType->getRegister(RegisterType::leftReg) <<
        ", " << rightType->getRegister(RegisterType::rightReg) << labelPass <<std::endl;
    std::cout << "li   $v0,0, "<< labelEnd <<std::endl;
    std::cout << "j     " << labelEnd <<std::endl;
    std::cout << ".global " << labelPass <<std::endl;
    std::cout << labelPass << ":" <<std::endl;
    std::cout << "li   $v0,1, "<< labelEnd <<std::endl;
    std::cout << ".global " << labelEnd << std::endl;
    std::cout << labelEnd << ":" <<std::endl;    
    std::cout << "sw    $v0, " << bindings->currentOffset() << "($fp)" << std::endl;
}

void AssignmentOperator::printASM(Bindings *bindings){
    //evaluate the expression
    //std::cout << "start assignment"<< std::endl;
    rightExpression->printASM(bindings);
    leftExpression->getType(bindings)->saveVariable(bindings,((Variable*)leftExpression)->getName());
    //std::cout << "end assignment"<< std::endl;
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
    std::string rightReg = rightExpression->getType(bindings)->getRegister(RegisterType::rightReg);
    std::string addu = ((OperandType*)(leftExpression->getType(bindings)))->getAdditionOperator();
    std::cout << addu << "   " << evalReg << "," << leftReg << "," << rightReg <<std::endl;
    //move the value from the evaluation register onto the stack
    leftExpression->getType(bindings)->extractFromRegister(bindings,RegisterType::evaluateReg);
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
    std::string subbu = ((OperandType*)(leftExpression->getType(bindings)))->getSubtractionOperator();
    std::cout << subbu << "   " << evalReg << "," << leftReg << "," << rightReg;
    //move the value from the evaluation register onto the stack
    leftExpression->getType(bindings)->extractFromRegister(bindings,RegisterType::evaluateReg);
}

void MultiplicationOperator::printASM(Bindings *bindings){
    //first evaluate left hand expression and place in register, the print asm places the value at the top of the stack
    leftExpression->printASM(bindings);
    leftExpression->getType(bindings)->placeInRegister(bindings, RegisterType::leftReg);
    //then evalute right hand expression and place in register
    rightExpression->printASM(bindings);
    rightExpression->getType(bindings)->placeInRegister(bindings, RegisterType::rightReg);
    //next add the two registers and place in evaluation register
    std::string evalReg = leftExpression->getType(bindings)->getRegister(RegisterType::evaluateReg);
    std::string leftReg = leftExpression->getType(bindings)->getRegister(RegisterType::leftReg);
    std::string rightReg = rightExpression->getType(bindings)->getRegister(RegisterType::rightReg);
    std::string multu = ((OperandType*)(leftExpression->getType(bindings)))->getMultiplicationOperator();
    std::cout << multu << "   " << evalReg << "," << leftReg << "," << rightReg <<std::endl;
    //move the value from the evaluation register onto the stack
    ((OperandType*)(leftExpression->getType(bindings)))->extractFromMultRegister(bindings);
}

void DivisionOperator::printASM(Bindings *bindings){
    //first evaluate left hand expression and place in register, the print asm places the value at the top of the stack
    leftExpression->printASM(bindings);
    leftExpression->getType(bindings)->placeInRegister(bindings, RegisterType::leftReg);
    //then evalute right hand expression and place in register
    rightExpression->printASM(bindings);
    rightExpression->getType(bindings)->placeInRegister(bindings, RegisterType::rightReg);
    //next add the two registers and place in evaluation register
    std::string evalReg = leftExpression->getType(bindings)->getRegister(RegisterType::evaluateReg);
    std::string leftReg = leftExpression->getType(bindings)->getRegister(RegisterType::leftReg);
    std::string rightReg = rightExpression->getType(bindings)->getRegister(RegisterType::rightReg);
    std::string divu = ((OperandType*)(leftExpression->getType(bindings)))->getDivisionOperator();
    std::cout << divu << "   " << evalReg << "," << leftReg << "," << rightReg <<std::endl;
    //move the value from the evaluation register onto the stack
    ((OperandType*)(leftExpression->getType(bindings)))->extractFromDivRegister(bindings);
}

void ModuloOperator::printASM(Bindings *bindings){
    //first evaluate left hand expression and place in register, the print asm places the value at the top of the stack
    leftExpression->printASM(bindings);
    leftExpression->getType(bindings)->placeInRegister(bindings, RegisterType::leftReg);
    //then evalute right hand expression and place in register
    rightExpression->printASM(bindings);
    rightExpression->getType(bindings)->placeInRegister(bindings, RegisterType::rightReg);
    //next add the two registers and place in evaluation register
    std::string evalReg = leftExpression->getType(bindings)->getRegister(RegisterType::evaluateReg);
    std::string leftReg = leftExpression->getType(bindings)->getRegister(RegisterType::leftReg);
    std::string rightReg = rightExpression->getType(bindings)->getRegister(RegisterType::rightReg);
    std::cout << "divu   " << evalReg << "," << leftReg << "," << rightReg <<std::endl;
    //move the value from the evaluation register onto the stack
    ((IntegralType*)(leftExpression->getType(bindings)))->extractFromModuloRegister(bindings);
}
//TODO beq
void LogicalAndOperator::printASM(Bindings *bindings){
    //first evaluate left hand expression and place in register, the print asm places the value at the top of the stack
    std::string endLabel = bindings->createLabel("end");
    std::string failLabel = bindings->createLabel("fail");
    FloatConstant *zero = new FloatConstant(0);
    EqualityOperator* equalityOperator = new EqualityOperator(leftExpression,zero);
    equalityOperator->printASM(bindings);
    std::cout << "lw    $v0, " << bindings->currentOffset() << "($fp)" << std::endl;
    std::cout << "beq   $v0,$zer0, " << failLabel << std::endl;
    std::cout << "nop" << std::endl;
    delete equalityOperator;
    equalityOperator = new EqualityOperator(rightExpression,zero);
    equalityOperator->printASM(bindings);
    std::cout << "lw    $v0, " << bindings->currentOffset() << "($fp)" << std::endl;
    std::cout << "beq   $v0,$zer0, " << failLabel << std::endl;
    std::cout << "li    $v0,1" << std::endl;
    std::cout << "sw    $v0, " << bindings->currentOffset() << "($fp)" << std::endl;
    std::cout << "j   " << endLabel << std::endl;
    std::cout << "nop" << std::endl;
    std::cout << ".global " << failLabel << std::endl;
    std::cout << failLabel << ":" << std::endl;
    std::cout << "li    $v0,0" << std::endl;
    std::cout << "sw    $v0, " << bindings->currentOffset() << "($fp)" << std::endl;
    std::cout << ".global " << endLabel << std::endl;
    std::cout << endLabel << ":" << std::endl;
}
//TODO beq
void LogicalOrOperator::printASM(Bindings *bindings){
    //first evaluate left hand expression and place in register, the print asm places the value at the top of the stack
    std::string endLabel = bindings->createLabel("end");
    std::string failLabel = bindings->createLabel("fail");
    std::string passLabel = bindings->createLabel("pass");
    FloatConstant *zero = new FloatConstant(0);
    EqualityOperator* equalityOperator = new EqualityOperator(leftExpression,zero);
    equalityOperator->printASM(bindings);
    std::cout << "lw    $v0, " << bindings->currentOffset() << "($fp)" << std::endl;
    std::cout << "beq   $v0,$zer0, " << failLabel << std::endl;
    std::cout << "nop" << std::endl;
    std::cout << "j   " << passLabel << std::endl;
    std::cout << "nop" << std::endl;
    delete equalityOperator;
    equalityOperator = new EqualityOperator(rightExpression,zero);
    equalityOperator->printASM(bindings);
    delete equalityOperator;
    std::cout << "lw    $v0, " << bindings->currentOffset() << "($fp)" << std::endl;
    std::cout << "beq   $v0,$zer0, " << failLabel << std::endl;
    std::cout << "nop" << std::endl;
    std::cout << ".global " << passLabel << std::endl;
    std::cout << passLabel << ":" << std::endl;
    std::cout << "li    $v0,1" << std::endl;
    std::cout << "sw    $v0, " << bindings->currentOffset() << "($fp)" << std::endl;
    std::cout << "j   " << endLabel << std::endl;
    std::cout << "nop" << std::endl;
    std::cout << ".global " << failLabel << std::endl;
    std::cout << failLabel << ":" << std::endl;
    std::cout << "li    $v0,0" << std::endl;
    std::cout << "sw    $v0, " << bindings->currentOffset() << "($fp)" << std::endl;
    std::cout << ".global " << endLabel << std::endl;
    std::cout << endLabel << ":" << std::endl;
} 

void BitwiseOrOperator::printASM(Bindings *bindings){
    //first evaluate left hand expression and place in register, the print asm places the value at the top of the stack
    leftExpression->printASM(bindings);
    leftExpression->getType(bindings)->placeInRegister(bindings, RegisterType::leftReg);
    //then evalute right hand expression and place in register
    rightExpression->printASM(bindings);
    rightExpression->getType(bindings)->placeInRegister(bindings, RegisterType::rightReg);
    //next add the two registers and place in evaluation register
    std::string evalReg = leftExpression->getType(bindings)->getRegister(RegisterType::evaluateReg);
    std::string leftReg = leftExpression->getType(bindings)->getRegister(RegisterType::leftReg);
    std::string rightReg = rightExpression->getType(bindings)->getRegister(RegisterType::rightReg);
    std::cout << "or   " << evalReg << "," << leftReg << "," << rightReg <<std::endl;
    //move the value from the evaluation register onto the stack
    leftExpression->getType(bindings)->extractFromRegister(bindings,RegisterType::evaluateReg);
} 

void BitwiseAndOperator::printASM(Bindings *bindings){
    //first evaluate left hand expression and place in register, the print asm places the value at the top of the stack
    leftExpression->printASM(bindings);
    leftExpression->getType(bindings)->placeInRegister(bindings, RegisterType::leftReg);
    //then evalute right hand expression and place in register
    rightExpression->printASM(bindings);
    rightExpression->getType(bindings)->placeInRegister(bindings, RegisterType::rightReg);
    //next add the two registers and place in evaluation register
    std::string evalReg = leftExpression->getType(bindings)->getRegister(RegisterType::evaluateReg);
    std::string leftReg = leftExpression->getType(bindings)->getRegister(RegisterType::leftReg);
    std::string rightReg = rightExpression->getType(bindings)->getRegister(RegisterType::rightReg);
    std::cout << "and   " << evalReg << "," << leftReg << "," << rightReg <<std::endl;
    //move the value from the evaluation register onto the stack
    leftExpression->getType(bindings)->extractFromRegister(bindings,RegisterType::evaluateReg);
} 

void BitwiseXorOperator::printASM(Bindings *bindings){
    //first evaluate left hand expression and place in register, the print asm places the value at the top of the stack
    leftExpression->printASM(bindings);
    leftExpression->getType(bindings)->placeInRegister(bindings, RegisterType::leftReg);
    //then evalute right hand expression and place in register
    rightExpression->printASM(bindings);
    rightExpression->getType(bindings)->placeInRegister(bindings, RegisterType::rightReg);
    //next add the two registers and place in evaluation register
    std::string evalReg = leftExpression->getType(bindings)->getRegister(RegisterType::evaluateReg);
    std::string leftReg = leftExpression->getType(bindings)->getRegister(RegisterType::leftReg);
    std::string rightReg = rightExpression->getType(bindings)->getRegister(RegisterType::rightReg);
    std::cout << "xor   " << evalReg << "," << leftReg << "," << rightReg <<std::endl;
    //move the value from the evaluation register onto the stack
    leftExpression->getType(bindings)->extractFromRegister(bindings,RegisterType::evaluateReg);
}

void ShiftLeftOperator::printASM(Bindings *bindings){
    Type *type = leftExpression->getType(bindings);
    leftExpression->printASM(bindings);
    leftExpression->getType(bindings)->placeInRegister(bindings, RegisterType::leftReg);
    rightExpression->printASM(bindings);
    rightExpression->getType(bindings)->placeInRegister(bindings, RegisterType::rightReg);
    std::cout << "sllv  " << type->getRegister(RegisterType::evaluateReg) <<
    ", " << type->getRegister(RegisterType::leftReg) << ", " <<  type->getRegister(RegisterType::rightReg)
    << std::endl;
}

void ShiftRightOperator::printASM(Bindings *bindings){
    Type *type = leftExpression->getType(bindings);
    leftExpression->printASM(bindings);
    leftExpression->getType(bindings)->placeInRegister(bindings, RegisterType::leftReg);
    rightExpression->printASM(bindings);
    rightExpression->getType(bindings)->placeInRegister(bindings, RegisterType::rightReg);
    if(type->isSigned() == false){
        std::cout << "srav  " << type->getRegister(RegisterType::evaluateReg) <<
        ", " << type->getRegister(RegisterType::leftReg) << ", " <<  type->getRegister(RegisterType::rightReg)
        << std::endl;
    }else{
        std::cout << "srlv  " << type->getRegister(RegisterType::evaluateReg) <<
        ", " << type->getRegister(RegisterType::leftReg) <<  ", " << type->getRegister(RegisterType::rightReg)
        << std::endl;
    }
}

void BitwiseNotOperator::printASM(Bindings *bindings){
    //first evaluate left hand expression and place in register, the print asm places the value at the top of the stack
    expression->printASM(bindings);
    expression->getType(bindings)->placeInRegister(bindings, RegisterType::leftReg);
    //next add the two registers and place in evaluation register
    std::string evalReg = expression->getType(bindings)->getRegister(RegisterType::evaluateReg);
    std::string leftReg = expression->getType(bindings)->getRegister(RegisterType::leftReg);
    std::cout << "nor   " << evalReg << "," << leftReg << ",$zero" <<std::endl;
    //move the value from the evaluation register onto the stack
    expression->getType(bindings)->extractFromRegister(bindings,RegisterType::evaluateReg);
}

void NotOperator::printASM(Bindings *bindings){
    //first evaluate left hand expression and place in register, the print asm places the value at the top of the stack
    expression->printASM(bindings);
    expression->getType(bindings)->placeInRegister(bindings, RegisterType::leftReg);
    //next add the two registers and place in evaluation register
    std::string evalReg = expression->getType(bindings)->getRegister(RegisterType::evaluateReg);
    std::string leftReg = expression->getType(bindings)->getRegister(RegisterType::leftReg);
    std::cout << "xori   " << evalReg << "," << leftReg << ",1" <<std::endl;
    //move the value from the evaluation register onto the stack
    expression->getType(bindings)->extractFromRegister(bindings,RegisterType::evaluateReg);
}

void DefreferenceOperator::printASM(Bindings *bindings){
    //first evaluate the address and place on stack
    expression->printASM(bindings);
    //then get hold of the pointer object and use the dereference function
    ((Pointer*)((Variable*)expression->getType(bindings)))->defreference(bindings);  
}

void AddressOperator::printASM(Bindings *bindings){
    //first get hold of the offset of the current variable and then add it to the frame pointer
    int offset = bindings->stackPosition(((Variable*)expression)->getName());
    std::cout << "addiu   $v0,$fp," << offset <<std::endl;
    //then store the address on the stack 
    std::cout << "sw    $v0, " << bindings->currentOffset() << "($fp)" << std::endl;
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
    if(((value & 4294901760) >> 16) == 0){
        std::cout << "li    $v0," << value << std::endl;
        std::cout << "sw    $v0, " << bindings->currentOffset() << "($fp)" << std::endl;
    }
    else{
        int upper = (value & 4294901760) >> 16;
        int lower = (value & 65535);
        std::cout << "mv    $v0,$zero" << std::endl;
        std::cout << "lui    $v0," << upper << std::endl;
        std::cout << "addi    $v0," << lower << std::endl;
        std::cout << "sw    $v0, " << bindings->currentOffset() << "($fp)" << std::endl;
    }
}

Type*  NumberConstant::getType(Bindings *bindings){
    return new Int();
}

BinaryOperatorExpression::BinaryOperatorExpression(Expression *leftExpression, Expression *rightExpression){
    this->leftExpression = leftExpression;
    this->rightExpression = rightExpression;
}

Type* BinaryOperatorExpression::getType(Bindings *bindings){
    return leftExpression->getType(bindings);
}

void SizeOfOperator::printASM(Bindings *bindings){
    int size = expression->getType(bindings)->getSize();
    NumberConstant *numberConstant = new NumberConstant(size);
    numberConstant->printASM(bindings);
    delete numberConstant;
}

TernaryOperator::TernaryOperator(Expression* expressionOne, Expression* expressionTwo, Expression* expressionThree){
    this->expressionOne = expressionOne;
    this->expressionTwo = expressionTwo;
    this->expressionThree = expressionThree;
}

void TernaryOperator::printASM(Bindings *bindings){
    expressionOne->printASM(bindings);
    Type *type = expressionOne->getType(bindings);
    type->placeInRegister(bindings,RegisterType::evaluateReg);
    std::string endLabel = bindings->createLabel("end");
    std::string failLabel = bindings->createLabel("fail");
    std::cout << "beq $zero," << type->getRegister(RegisterType::evaluateReg)
        << ", " << failLabel <<std::endl;
    expressionTwo->printASM(bindings);
    std::cout << "j " << endLabel;
    std::cout << ".global " << failLabel << std::endl;
    std::cout << failLabel << ":" << std::endl;
    expressionThree->printASM(bindings);
    std::cout << ".global " << endLabel << std::endl;
    std::cout << endLabel << ":" << std::endl;
}