#include "expression.hpp"
#include "node.hpp"
#include "type.hpp"
#include "parameter.hpp"
#include <iostream>
class Bindings;
#include <math.h> 
#include <typeinfo>



DotOperator::DotOperator(std::string *structId, std::string *elementId){
    this->structId = *structId;
    this->elementId = *elementId;
}

void DotOperator::printASM(Bindings *bindings){
     //((Struct*)bindings->getVariable(structId))->placeElementOnStack(bindings,structId,elementId);
    int offset = bindings->elementPosition(structId,elementId);
    Type *type = bindings->getElement(structId,elementId);
    type->placeVariableOnStack(bindings,offset);
}

void DotOperator::printASMAssign(Bindings *bindings){
    //take the value on the stack and place it in the 
    //memory location given by the struct offset and the element offset
    int offset = bindings->elementPosition(structId,elementId);
    Type *type = bindings->getElement(structId,elementId);
    type->saveVariable(bindings,offset);
}

Type* DotOperator::getType(Bindings* bindings){
    return ((Struct*)bindings->getVariable(structId))->getElementType(bindings,structId,elementId);
    //return new Int();
}
// ArrowOperator::ArrowOperator(std::string *pointerId, std::string *elementId){
//     this->pointerId = *pointerId;
//     this->elementId = *elementId;
// }

// void ArrowOperator::printASM(Bindings *bindings){
//     //do something different dependent on whether the left hand operator is a struct or a arrow operator or a dot operator
//     //if a struct
//     //first place the memlocation on the stack
//     //then find the offset and add that to the stack address
//     //then dereference the value using the correct type and place that on the stack
//  //   ((Struct*)bindings->getVariable(pointerId))->placeElementOnStack(bindings,pointerId,elementId);
// }

// Type* ArrowOperator::getType(Bindings* bindings){
//     ((Struct*)bindings->getVariable(pointerId))->getElementType(bindings,pointerId,elementId);
// }

UnaryOperatorExpression::UnaryOperatorExpression(Expression *expression){
    this->expression = expression;
}

FunctionCall::FunctionCall(std::string *id, Parameter *parameter){
    this->id = *id;
    this->parameter = parameter;
}

void FunctionCall::printASM(Bindings *bindings){
    //std::cout << "function call" << std::endl;
    //first put the parameters into memory in order
    std::string params = "";
    ReturnRegisters returnRegister;
    int total = 0;
    if(parameter != nullptr){
        //std::cout << "loading param" << std::endl;
        //create the label string needed to find where to jump to
        //parameter->createLabel(params, bindings);
        //calculate the total needed memory
        int mem = 0;
        parameter->calculateTotalMem(mem, bindings);
        total = mem;
        //place the parameters on the stack
        parameter->placeOnStack(bindings, mem, returnRegister);
        //set the new bindings offset
        bindings->setOffset(bindings->currentOffset() + mem);
        //std::cout << "loaded param" << std::endl;
    }
    //set the sp to the current bindings offset
    //std::cout << bindings->currentOffset() << std::endl;
    //int upper = (bindings->currentOffset() && 4294901760) >> 16;
    //int lower = (bindings->currentOffset() && 65535);
    //std::cout << lower << std::endl;
    //std::cout << "lui    $sp," << upper << std::endl;
    //std::cout << "addi    $sp," << lower << std::endl;
    std::cout << "li    $sp," << (bindings->currentOffset() - total) << std::endl;
    //add stack pointer to the frame pointer
    std::cout << "addu $sp,$sp,$fp" << std::endl;
    //then jump to the function code
    std::cout << "jal     " << id << std::endl;
    std::cout << "nop " << std::endl;
    //put the return values into the stack
    //bindings->getFunction(id)->processReturn(bindings);
    std::cout << "sw    $v0," << bindings->currentOffset() << "($fp)" <<std::endl;
}

Type*  FunctionCall::getType(Bindings *bindings){
    return bindings->getFunction(id);
}

Variable::Variable(std::string *id){
    this->id = *id;
}

Type* Variable::getType(Bindings *bindings){
    return bindings->getVariable(id);
}

StringConstant::StringConstant(std::string *value){
    this->value = *value;
}
//---check
void StringConstant::printASM(Bindings *bindings){
    std::string label = bindings->createLabel("string");
    std::cout << ".data" <<std::endl;
    std::cout << label << ": .asciiz " << "\"" << value << "\"" << std::endl;
    std::cout << ".text" <<std::endl;
    bindings->setOffset(bindings->currentOffset() - 1*value.size() - 4);
    while(bindings->currentOffset()%4!=0){
        bindings->setOffset(bindings->currentOffset() - 1);
    }
    std::cout << "la $v0, " << label << std::endl;
    std::cout << "sw $v0, " << bindings->currentOffset() << "($fp)" << std::endl;
    // //first reverse the order of the string
    // std::string input = "";
    // for(char &c: value){
    //     input = c + input;
    // }
    // //load the values into memory
    // //store the current mem address in a register
    // bindings->setOffset(bindings->currentOffset()-(4-input.size()%4));
    // std::cout << "li $v0," << (int)'\0' << std::endl;
    // std::cout << "sb $v0," << bindings->currentOffset() << "($fp)" <<std::endl;
    // bindings->setOffset(bindings->currentOffset()-1);
    // for(char &c: input){
    //     //NumberConstant *constant = new NumberConstant(c);
    //     //constant->printASM(bindings);
    //     std::cout << "li $v0," << (int)c << std::endl;
    //     std::cout << "sb $v0," << bindings->currentOffset() << "($fp)" <<std::endl;
    //     bindings->setOffset(bindings->currentOffset()-1);
    //     //delete constant;
    // }
    // std::cout << "li    $t2," << (bindings->currentOffset() + 1) << std::endl;
    // std::cout << "addu    $t2,$t2,$fp" << std::endl;
    // //load the saved mem address into memory
    // std::cout << "sw    $t2," << bindings->currentOffset() << "($fp)" << std::endl;
}
//---check
Type*  StringConstant::getType(Bindings *bindings){return new Array(new Char(),value.size());}

FloatConstant::FloatConstant(float value){
    this->value = value;
}

void FloatConstant::printASM(Bindings *bindings) {
    std::cout << "li.s $f5," << value << std::endl;
    std::cout << "s.s    $f5, " << bindings->currentOffset() << "($fp)" << std::endl;
}

Type*  FloatConstant::getType(Bindings *bindings){
    return new Float();
}

DoubleConstant::DoubleConstant(double value){
    this->value = value;
}

void DoubleConstant::printASM(Bindings *bindings) {
    std::cout << "li.d $f4," << value << std::endl;
    std::cout << "s.d    $f4, " << bindings->currentOffset() << "($fp)" << std::endl;
}

Type*  DoubleConstant::getType(Bindings *bindings){
    return new Double();
}

UnsignedConstant::UnsignedConstant(unsigned int value){
    this->value = value;
}

void UnsignedConstant::printASM(Bindings *bindings) {
    std::cout << "li.s $f5," << value << std::endl;
    std::cout << "s.s    $f5, " << bindings->currentOffset() << "($fp)" << std::endl;
}

Type*  UnsignedConstant::getType(Bindings *bindings){
    Int *integer = new Int();
    integer->setUnSigned();
    return integer;
}

NumberConstant::NumberConstant(int value) {
    this->value = value;
}

void NumberConstant::printASM(Bindings *bindings){
    // if(((value & 4294901760) >> 16) == 0){
        std::cout << "li    $v0," << value << std::endl;
        std::cout << "sw    $v0, " << bindings->currentOffset() << "($fp)" << std::endl;
    // }
    // else{
    //     int upper = (value & 4294901760) >> 16;
    //     int lower = (value & 65535);
    //     std::cout << "move    $v0,$zero" << std::endl;
    //     std::cout << "lui    $v0," << upper << std::endl;
    //     std::cout << "addi    $v0," << lower << std::endl;
    //     std::cout << "sw    $v0, " << bindings->currentOffset() << "($fp)" << std::endl;
    // }
}

Type*  NumberConstant::getType(Bindings *bindings){
    return new Int();
}

void EqualityOperator::printASM(Bindings *bindings){
    //evalute the left expression and place on stack
    std::string labelEnd = bindings->createLabel("emd");
    std::string labelPass = bindings->createLabel("pass"); 
    Type* leftType = leftExpression->getType(bindings);
    Type* rightType = rightExpression->getType(bindings);

    leftExpression->printASM(bindings);
    bindings->setOffset(bindings->currentOffset() - 4);
    rightExpression->printASM(bindings);
    rightType->placeInRegister(bindings, RegisterType::rightReg);
    bindings->setOffset(bindings->currentOffset() + 4);
    leftType->placeInRegister(bindings, RegisterType::leftReg);
    leftType->beq(bindings,RegisterType::leftReg, RegisterType::rightReg, labelPass);
    std::cout << "nop" << std::endl;
    //std::cout << "beq   " << leftType->getRegister(RegisterType::leftReg) <<
    //    ", " << rightType->getRegister(RegisterType::rightReg) << labelPass <<std::endl;
    std::cout << "li    " << "$v0, 0" <<std::endl;
    std::cout << "j     " << labelEnd <<std::endl;
    std::cout << "nop " <<std::endl;
    std::cout << ".global " << labelPass <<std::endl;
    std::cout << labelPass << ":" <<std::endl;
    std::cout << "li    " << "$v0, 1" <<std::endl;
    std::cout << ".global " << labelEnd <<std::endl;
    std::cout << labelEnd << ":" <<std::endl;
    std::cout << "sw    $v0, " << bindings->currentOffset() << "($fp)" << std::endl;
}

void LessThanOperator::printASM(Bindings *bindings){
    Type* leftType = leftExpression->getType(bindings);
    Type* rightType = rightExpression->getType(bindings);
    leftExpression->printASM(bindings);
    bindings->setOffset(bindings->currentOffset() - leftExpression->getType(bindings)->getSize());
    rightExpression->printASM(bindings);
    rightExpression->getType(bindings)->placeInRegister(bindings, RegisterType::rightReg);
    bindings->setOffset(bindings->currentOffset() + leftExpression->getType(bindings)->getSize());
    leftExpression->getType(bindings)->placeInRegister(bindings, RegisterType::leftReg);
    std::cout << "slt    $v0," << leftType->getRegister(RegisterType::leftReg) << "," <<
        rightType->getRegister(RegisterType::rightReg) <<std::endl;
    std::cout << "sw    $v0, " << bindings->currentOffset() << "($fp)" << std::endl;
}

void LessThanEqualOperator::printASM(Bindings *bindings){
    std::string labelEnd = bindings->createLabel("end");
    std::string labelNext = bindings->createLabel("next");
    std::string labelPass = bindings->createLabel("pass"); 
    Type* leftType = leftExpression->getType(bindings);
    Type* rightType = rightExpression->getType(bindings);
    leftExpression->printASM(bindings);
    bindings->setOffset(bindings->currentOffset() - leftExpression->getType(bindings)->getSize());
    rightExpression->printASM(bindings);
    rightExpression->getType(bindings)->placeInRegister(bindings, RegisterType::rightReg);
    bindings->setOffset(bindings->currentOffset() + leftExpression->getType(bindings)->getSize());
    leftExpression->getType(bindings)->placeInRegister(bindings, RegisterType::leftReg);
    std::cout << "slt    " << leftType->getRegister(RegisterType::evaluateReg)
        <<"," << leftType->getRegister(RegisterType::leftReg) << "," <<
        rightType->getRegister(RegisterType::rightReg) <<std::endl;
    leftExpression->getType(bindings)->beq(bindings,RegisterType::evaluateReg,RegisterType::zeroReg,labelNext);
    std::cout << "li   $v0,1"<<std::endl;
    std::cout << "j     " << labelEnd <<std::endl;
    std::cout << "nop " <<std::endl;
    std::cout << ".global " << labelNext << std::endl;
    std::cout << labelNext << ":" <<std::endl;    
    leftExpression->getType(bindings)->beq(bindings,RegisterType::leftReg,RegisterType::rightReg,labelPass);
    std::cout << "li   $v0,0"<<std::endl;
    std::cout << "j     " << labelEnd <<std::endl;
    std::cout << "nop " <<std::endl;
    std::cout << ".global " << labelPass <<std::endl;
    std::cout << labelPass << ":" <<std::endl;
    std::cout << "li   $v0,1 " <<std::endl;
    std::cout << ".global " << labelEnd << std::endl;
    std::cout << labelEnd << ":" <<std::endl;    
    std::cout << "sw    $v0, " << bindings->currentOffset() << "($fp)" << std::endl;
}

void AssignmentOperator::printASM(Bindings *bindings){
    //evaluate the expression
    //this will be different for pointer than it is for none pointer types
    rightExpression->printASM(bindings);
    //std::cout << "added 0 " << typeid(*leftExpression).name() <<std::endl;
    if (dynamic_cast<DefreferenceOperator*>(leftExpression) != nullptr){
        //std::cout << "added 1 " <<std::endl;
        //std::cout << "dereference assign --------" << std::endl;
        ((DefreferenceOperator*)leftExpression)->printASMAssign(bindings);
        //std::cout << "end dereference assign --------" << std::endl;
    }
    else if (dynamic_cast<IndexOperator*>(leftExpression) != nullptr)
    {
        //std::cout << "added 2 " <<std::endl;
        ((IndexOperator*)leftExpression)->printASMAssign(bindings);
    }
    else if (dynamic_cast<DotOperator*>(leftExpression) != nullptr)
    {
        //std::cout << "added 3 " <<std::endl;
        ((DotOperator*)leftExpression)->printASMAssign(bindings);
    }
    else{
        //std::cout << "adding 4 " <<std::endl;
        //std::cout << "assign --------" << std::endl;
        leftExpression->getType(bindings)->saveVariable(bindings,((Variable*)leftExpression)->getName());
        //std::cout << "end assign --------" << std::endl;
    }
    //std::cout << "added 5 " <<std::endl;
}

void AdditionOperator::printASM(Bindings *bindings){
    //std::cout << "add ---->" << std::endl;
    //first evaluate left hand expression and place in register, the print asm places the value at the top of the stack
    
    leftExpression->printASM(bindings);
    bindings->setOffset(bindings->currentOffset() - leftExpression->getType(bindings)->getSize());
    rightExpression->printASM(bindings);
    rightExpression->getType(bindings)->placeInRegister(bindings, RegisterType::rightReg);
    bindings->setOffset(bindings->currentOffset() + leftExpression->getType(bindings)->getSize());
    leftExpression->getType(bindings)->placeInRegister(bindings, RegisterType::leftReg);
    std::string reg = rightExpression->getType(bindings)->getRegister(RegisterType::rightReg);
    if (dynamic_cast<Pointer*>(leftExpression->getType(bindings)) != nullptr){
       if (dynamic_cast<Pointer*>(rightExpression->getType(bindings)) == nullptr){
            if (dynamic_cast<Char*>(((Pointer*)leftExpression->getType(bindings))->getType()) == nullptr){
                int shift = log2(((Pointer*)leftExpression->getType(bindings))->getType()->getSize());
                std::cout << "sll    " << reg << "," << reg << "," << shift << std::endl;
            }
        } 
    }
    //next add the two registers and place in evaluation register
    std::string evalReg = leftExpression->getType(bindings)->getRegister(RegisterType::evaluateReg);
    std::string leftReg = leftExpression->getType(bindings)->getRegister(RegisterType::leftReg);
    std::string rightReg = rightExpression->getType(bindings)->getRegister(RegisterType::rightReg);
    std::string addu = ((OperandType*)(leftExpression->getType(bindings)))->getAdditionOperator();
    std::cout << addu << "   " << evalReg << "," << leftReg << "," << rightReg <<std::endl;
    //move the value from the evaluation register onto the stack
    leftExpression->getType(bindings)->extractFromRegister(bindings,RegisterType::evaluateReg);
    //std::cout << "endadd ---->" << std::endl;
}

void SubtractionOperator::printASM(Bindings *bindings){
    //first evaluate left hand expression and place in register, the print asm places the value at the top of the stack
    leftExpression->printASM(bindings);
    bindings->setOffset(bindings->currentOffset() - leftExpression->getType(bindings)->getSize());
    rightExpression->printASM(bindings);
    rightExpression->getType(bindings)->placeInRegister(bindings, RegisterType::rightReg);
    bindings->setOffset(bindings->currentOffset() + leftExpression->getType(bindings)->getSize());
    leftExpression->getType(bindings)->placeInRegister(bindings, RegisterType::leftReg);
    //next add the two registers and place in evaluation register
    std::string reg = rightExpression->getType(bindings)->getRegister(RegisterType::rightReg);
    if (dynamic_cast<Pointer*>(leftExpression->getType(bindings)) != nullptr){
       if (dynamic_cast<Pointer*>(rightExpression->getType(bindings)) == nullptr){
            if (dynamic_cast<Char*>(((Pointer*)leftExpression->getType(bindings))->getType()) == nullptr){
                int shift = log2(((Pointer*)leftExpression->getType(bindings))->getType()->getSize());
                std::cout << "sll    " << reg << "," << reg << "," << shift << std::endl;
            }
        } 
    }
    std::string evalReg = leftExpression->getType(bindings)->getRegister(RegisterType::evaluateReg);
    std::string leftReg = leftExpression->getType(bindings)->getRegister(RegisterType::leftReg);
    std::string rightReg = leftExpression->getType(bindings)->getRegister(RegisterType::rightReg);
    std::string subbu = ((OperandType*)(leftExpression->getType(bindings)))->getSubtractionOperator();
    std::cout << subbu << "   " << evalReg << "," << leftReg << "," << rightReg << std::endl;
    //move the value from the evaluation register onto the stack
    leftExpression->getType(bindings)->extractFromRegister(bindings,RegisterType::evaluateReg);
}

void MultiplicationOperator::printASM(Bindings *bindings){
    //std::cout << "mul ---->" << std::endl;
    //first evaluate left hand expression and place in register, the print asm places the value at the top of the stack
    leftExpression->printASM(bindings);
    bindings->setOffset(bindings->currentOffset() - leftExpression->getType(bindings)->getSize());
    rightExpression->printASM(bindings);
    rightExpression->getType(bindings)->placeInRegister(bindings, RegisterType::rightReg);
    bindings->setOffset(bindings->currentOffset() + leftExpression->getType(bindings)->getSize());
    leftExpression->getType(bindings)->placeInRegister(bindings, RegisterType::leftReg);
    //next add the two registers and place in evaluation register
    //std::string evalReg = leftExpression->getType(bindings)->getRegister(RegisterType::evaluateReg);
    std::string leftReg = leftExpression->getType(bindings)->getRegister(RegisterType::leftReg);
    std::string rightReg = rightExpression->getType(bindings)->getRegister(RegisterType::rightReg);
    std::string multu = ((OperandType*)(leftExpression->getType(bindings)))->getMultiplicationOperator();
    std::cout << multu << leftReg << "," << rightReg <<std::endl;
    //move the value from the evaluation register onto the stack
    ((OperandType*)(leftExpression->getType(bindings)))->extractFromMultRegister(bindings);
    //std::cout << "end mul ---->" << std::endl;
}

void DivisionOperator::printASM(Bindings *bindings){
    //first evaluate left hand expression and place in register, the print asm places the value at the top of the stack
    leftExpression->printASM(bindings);
    bindings->setOffset(bindings->currentOffset() - leftExpression->getType(bindings)->getSize());
    rightExpression->printASM(bindings);
    rightExpression->getType(bindings)->placeInRegister(bindings, RegisterType::rightReg);
    bindings->setOffset(bindings->currentOffset() + leftExpression->getType(bindings)->getSize());
    leftExpression->getType(bindings)->placeInRegister(bindings, RegisterType::leftReg);
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
    bindings->setOffset(bindings->currentOffset() - leftExpression->getType(bindings)->getSize());
    rightExpression->printASM(bindings);
    rightExpression->getType(bindings)->placeInRegister(bindings, RegisterType::rightReg);
    bindings->setOffset(bindings->currentOffset() + leftExpression->getType(bindings)->getSize());
    leftExpression->getType(bindings)->placeInRegister(bindings, RegisterType::leftReg);
    //next add the two registers and place in evaluation register
    std::string evalReg = leftExpression->getType(bindings)->getRegister(RegisterType::evaluateReg);
    std::string leftReg = leftExpression->getType(bindings)->getRegister(RegisterType::leftReg);
    std::string rightReg = rightExpression->getType(bindings)->getRegister(RegisterType::rightReg);
    std::cout << "divu   " << evalReg << "," << leftReg << "," << rightReg <<std::endl;
    //move the value from the evaluation register onto the stack
    ((IntegralType*)(leftExpression->getType(bindings)))->extractFromModuloRegister(bindings);
}

void LogicalAndOperator::printASM(Bindings *bindings){
    //first evaluate left hand expression and place in register, the print asm places the value at the top of the stack
    //std::cout << leftExpression->getType(bindings)->getName() <<std::endl;
    std::string endLabel = bindings->createLabel("end");
    std::string failLabel = bindings->createLabel("fail");

    Expression *zeroL = ((OperandType*)leftExpression->getType(bindings))->getZero();
    EqualityOperator* equalityOperator = new EqualityOperator(leftExpression,zeroL);
    equalityOperator->printASM(bindings);
    Int *intT = new Int();
    std::cout << "lw    " << intT->getRegister(RegisterType::evaluateReg)
        << ", " << bindings->currentOffset() << "($fp)" << std::endl;
    std::cout << "li " << intT->getRegister(RegisterType::leftReg) << ",1"<<std::endl;
    intT->beq(bindings,RegisterType::evaluateReg,RegisterType::leftReg,failLabel);
    std::cout << "nop" << std::endl;
    delete equalityOperator;
    delete zeroL;

    Expression *zeroR = ((OperandType*)rightExpression->getType(bindings))->getZero();
    equalityOperator = new EqualityOperator(rightExpression,zeroR);
    equalityOperator->printASM(bindings);
    delete equalityOperator;
    delete zeroR;
    std::cout << "lw    " << intT->getRegister(RegisterType::evaluateReg)
        << ", " << bindings->currentOffset() << "($fp)" << std::endl;
    std::cout << "li " << intT->getRegister(RegisterType::leftReg) << ",1"<<std::endl;
    intT->beq(bindings,RegisterType::evaluateReg,RegisterType::leftReg,failLabel);
    std::cout << "nop" << std::endl;
    std::cout << "li    $v0,1" << std::endl;
    std::cout << "sw    $v0, " << bindings->currentOffset() << "($fp)" << std::endl;
    std::cout << "j   " << endLabel << std::endl;
    std::cout << "nop " <<std::endl;
    std::cout << ".global " << failLabel << std::endl;
    std::cout << failLabel << ":" << std::endl;
    std::cout << "li    $v0,0" << std::endl;
    std::cout << "sw    $v0, " << bindings->currentOffset() << "($fp)" << std::endl;
    std::cout << ".global " << endLabel << std::endl;
    std::cout << endLabel << ":" << std::endl;
    delete intT;
}

void LogicalOrOperator::printASM(Bindings *bindings){
    //first evaluate left hand expression and place in register, the print asm places the value at the top of the stack
    std::string endLabel = bindings->createLabel("end");
    std::string nextLabel = bindings->createLabel("next");
    std::string failLabel = bindings->createLabel("fail");
    std::string passLabel = bindings->createLabel("pass");
    Expression *zeroL = ((OperandType*)leftExpression->getType(bindings))->getZero();
    EqualityOperator* equalityOperator = new EqualityOperator(leftExpression,zeroL);
    equalityOperator->printASM(bindings);
    Int *intT = new Int();
    std::cout << "lw    " << intT->getRegister(RegisterType::evaluateReg)
        << ", " << bindings->currentOffset() << "($fp)" << std::endl;
    std::cout << "li " << intT->getRegister(RegisterType::leftReg) << ",1"<<std::endl;
    intT->beq(bindings,RegisterType::evaluateReg,RegisterType::leftReg,nextLabel);
    //std::cout << "beq   $v0,$zero, " << failLabel << std::endl;
    std::cout << "nop" << std::endl;
    std::cout << "j   " << passLabel << std::endl;
    std::cout << "nop" << std::endl;
    std::cout << ".global " << nextLabel << std::endl;
    std::cout << nextLabel << ":" << std::endl;
    delete equalityOperator;
    Expression *zeroR = ((OperandType*)rightExpression->getType(bindings))->getZero();
    equalityOperator = new EqualityOperator(rightExpression,zeroR);
    equalityOperator->printASM(bindings);
    delete equalityOperator;
    std::cout << "lw    " << intT->getRegister(RegisterType::evaluateReg)
        << ", " << bindings->currentOffset() << "($fp)" << std::endl;
    std::cout << "li " << intT->getRegister(RegisterType::leftReg) << ",1"<<std::endl;
    intT->beq(bindings,RegisterType::evaluateReg,RegisterType::leftReg,failLabel);
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
    bindings->setOffset(bindings->currentOffset() - leftExpression->getType(bindings)->getSize());
    rightExpression->printASM(bindings);
    rightExpression->getType(bindings)->placeInRegister(bindings, RegisterType::rightReg);
    bindings->setOffset(bindings->currentOffset() + leftExpression->getType(bindings)->getSize());
    leftExpression->getType(bindings)->placeInRegister(bindings, RegisterType::leftReg);
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
    bindings->setOffset(bindings->currentOffset() - leftExpression->getType(bindings)->getSize());
    rightExpression->printASM(bindings);
    rightExpression->getType(bindings)->placeInRegister(bindings, RegisterType::rightReg);
    bindings->setOffset(bindings->currentOffset() + leftExpression->getType(bindings)->getSize());
    leftExpression->getType(bindings)->placeInRegister(bindings, RegisterType::leftReg);
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
    bindings->setOffset(bindings->currentOffset() - leftExpression->getType(bindings)->getSize());
    rightExpression->printASM(bindings);
    rightExpression->getType(bindings)->placeInRegister(bindings, RegisterType::rightReg);
    bindings->setOffset(bindings->currentOffset() + leftExpression->getType(bindings)->getSize());
    leftExpression->getType(bindings)->placeInRegister(bindings, RegisterType::leftReg);
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
    bindings->setOffset(bindings->currentOffset() - leftExpression->getType(bindings)->getSize());
    rightExpression->printASM(bindings);
    rightExpression->getType(bindings)->placeInRegister(bindings, RegisterType::rightReg);
    bindings->setOffset(bindings->currentOffset() + leftExpression->getType(bindings)->getSize());
    leftExpression->getType(bindings)->placeInRegister(bindings, RegisterType::leftReg);
    std::cout << "sllv  " << type->getRegister(RegisterType::evaluateReg) <<
    ", " << type->getRegister(RegisterType::leftReg) << ", " <<  type->getRegister(RegisterType::rightReg)
    << std::endl;
}

void ShiftRightOperator::printASM(Bindings *bindings){
    Type *type = leftExpression->getType(bindings);
    leftExpression->printASM(bindings);
    bindings->setOffset(bindings->currentOffset() - leftExpression->getType(bindings)->getSize());
    rightExpression->printASM(bindings);
    rightExpression->getType(bindings)->placeInRegister(bindings, RegisterType::rightReg);
    bindings->setOffset(bindings->currentOffset() + leftExpression->getType(bindings)->getSize());
    leftExpression->getType(bindings)->placeInRegister(bindings, RegisterType::leftReg);
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

Type*  BitwiseNotOperator::getType(Bindings *bindings){
    return expression->getType(bindings);
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

Type*  NotOperator::getType(Bindings *bindings){
    return expression->getType(bindings);
}

void DefreferenceOperator::printASM(Bindings *bindings){
    //first evaluate the address and place on stack
    expression->printASM(bindings);
    //then get hold of the pointer object and use the dereference function
    ((Pointer*)((Variable*)expression->getType(bindings)))->defreference(bindings);  
}

void DefreferenceOperator::printASMAssign(Bindings *bindings){
    //place the variable to store on the right hand register
    //std::cout << "dereference --------" << std::endl;
    ((Pointer*)((Variable*)expression)->getType(bindings))->getType()->placeInRegister(bindings, RegisterType::rightReg);
    //std::cout << "one --------" << std::endl;
    expression->printASM(bindings);
    //std::cout << "two --------" << std::endl;
    expression->getType(bindings)->placeInRegister(bindings,RegisterType::evaluateReg);
    //std::cout << "three --------" << std::endl;
    ((Pointer*)((Variable*)expression)->getType(bindings))->getType()->extractFromregister(bindings,RegisterType::rightReg,RegisterType::evaluateReg);
    //std::cout << "end dereference --------" << std::endl;
}

Type*  DefreferenceOperator::getType(Bindings *bindings){
    return ((Pointer*)expression->getType(bindings))->getType();
}

void AddressOperator::printASM(Bindings *bindings){
    //first get hold of the offset of the current variable and then add it to the frame pointer
    //std::cout << "address --------" << std::endl;
    int offset = bindings->stackPosition(((Variable*)expression)->getName());
    std::cout << "addiu   $v0,$fp," << offset <<std::endl;
    //then store the address on the stack 
    std::cout << "sw    $v0, " << bindings->currentOffset() << "($fp)" << std::endl;
    //std::cout << "end address --------" << std::endl;
}

Type*  AddressOperator::getType(Bindings *bindings){
    return new Pointer(expression->getType(bindings));
}

std::string Variable::getName(){
    return id;
}

void Variable::printASM(Bindings *bindings){
    bindings->getVariable(id)->placeVariableOnStack(bindings,id);
}

BinaryOperatorExpression::BinaryOperatorExpression(Expression *leftExpression, Expression *rightExpression){
    this->leftExpression = leftExpression;
    this->rightExpression = rightExpression;
}

Type* BinaryOperatorExpression::getType(Bindings *bindings){
    return leftExpression->getType(bindings);
}

void IndexOperator::printASM(Bindings *bindings){
    //add one to the address
    AdditionOperator *sub = new AdditionOperator(leftExpression,rightExpression);
    AssignmentOperator *assign1 = new AssignmentOperator(leftExpression,sub);
    assign1->printASM(bindings);
    //load the address onto memory
    leftExpression->printASM(bindings);
    //dereference the address
    ((Pointer*)((Variable*)leftExpression->getType(bindings)))->defreference(bindings);
    int size = ((Pointer*)((Variable*)leftExpression->getType(bindings)))->getType()->getSize();
    bindings->setOffset(bindings->currentOffset() - size);
    //reset the address
    SubtractionOperator *add = new SubtractionOperator(leftExpression,rightExpression);
    AssignmentOperator *assign2 = new AssignmentOperator(leftExpression,add);
    assign2->printASM(bindings);
    bindings->setOffset(bindings->currentOffset() + size);
    delete assign1;
    delete assign2;
    delete add;
    delete sub;
}

void IndexOperator::printASMAssign(Bindings *bindings){
    //offset bindings to save the value
    bindings->setOffset(bindings->currentOffset() - 4);
    AdditionOperator *add = new AdditionOperator(leftExpression,rightExpression);
    AssignmentOperator *assign1 = new AssignmentOperator(leftExpression,add);
    assign1->printASM(bindings);
    leftExpression->getType(bindings)->placeInRegister(bindings,RegisterType::evaluateReg);
    bindings->setOffset(bindings->currentOffset() + 4);
    ((Pointer*)((Variable*)leftExpression)->getType(bindings))
        ->getType()->placeInRegister(bindings, RegisterType::rightReg);
    ((Pointer*)((Variable*)leftExpression)->getType(bindings))
        ->getType()->extractFromregister(bindings,RegisterType::rightReg,RegisterType::evaluateReg);
    SubtractionOperator *sub = new SubtractionOperator(leftExpression,rightExpression);
    AssignmentOperator *assign2 = new AssignmentOperator(leftExpression,sub);
    assign2->printASM(bindings);
    delete assign1;
    delete assign2;
    delete add;
    delete sub;
}

Type* IndexOperator::getType(Bindings *bindings){
    return ((Array*)((Variable*)leftExpression->getType(bindings)))->getType();
}

SizeOfOperator::SizeOfOperator(Type *type){
    this->type = type;
}

void SizeOfOperator::printASM(Bindings *bindings){
    int size =type->getSize();
    if(dynamic_cast<Char*>(type) != nullptr){
        size = 1;
    }
    NumberConstant *numberConstant = new NumberConstant(size);
    numberConstant->printASM(bindings);
    delete numberConstant;
}

void SizeOfExpressionOperator::printASM(Bindings *bindings){
    Type *type = expression->getType(bindings);
    int size = type->getSize();
    if(dynamic_cast<Char*>(type) != nullptr){
        size = 1;
    }
    NumberConstant *numberConstant = new NumberConstant(size);
    numberConstant->printASM(bindings);
    delete numberConstant;
}

Type*  SizeOfExpressionOperator::getType(Bindings *bindings){
    return new Int();
}

Type*  SizeOfOperator::getType(Bindings *bindings){
    return new Int();
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
    std::cout << "nop " <<std::endl;
    std::cout << ".global " << failLabel << std::endl;
    std::cout << failLabel << ":" << std::endl;
    expressionThree->printASM(bindings);
    std::cout << ".global " << endLabel << std::endl;
    std::cout << endLabel << ":" << std::endl;
}

Type*  TernaryOperator::getType(Bindings *bindings){
    return expressionTwo->getType(bindings);
}