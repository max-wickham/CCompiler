#include "type.hpp"

#include "node.hpp"
#include "bindings.hpp"
#include "expression.hpp"
#include <string>
#include <iostream>

bool Type::isSigned(){
    return this->_signed;
}

void Type::setUnSigned(){
    this->_signed = false;
}

//struct

Struct::Struct (int size, std::string *id){
    this->size = size;
    this->id = *id;
}

int Struct::getSize(){
    return this->size;
}

std::string Struct::getName(){
    return "struct";
}

void Struct::loadParameter(ReturnRegisters &returnRegisters, Bindings *bindings){}

void Struct::saveParameter(ReturnRegisters &returnRegisters, Bindings *bindings){}

void Struct::evaluateReturn(Bindings *bindings){}

void Struct::processReturn(Bindings *bindings){}

//places the value at the top of the stack into the register
void Struct::placeInRegister(Bindings *bindings, RegisterType type){}

//places the value in the register at the top of the stack
void Struct::extractFromRegister(Bindings *bindings, RegisterType type){}

//extract from register and place in the mem address in the address register
void Struct::extractFromregister(Bindings *bindings, RegisterType type, RegisterType address){}

//gets the register string from a register type
std::string Struct::getRegister(RegisterType type){return "";}

void Struct::saveVariable(Bindings *bindings, std::string id){}

void Struct::placeVariableOnStack(Bindings *bindings, std::string id){}
//TODO
Type* Struct::getElementType(Bindings *bindings, std::string id, std::string elementId){}
//TODO

void Struct::placeVariableOnStack(Bindings *bindings){}

void Struct::beq(Bindings *bindings, RegisterType reg1, RegisterType reg2, std::string label){}

//void
Void::Void(){}

int Void::getSize(){return 0;}

std::string Void::getName(){return "void";}

std::string Void::getRegister(RegisterType type){return "$zero";}

//integral type
std::string IntegralType::getAdditionOperator(){
    return "add ";
}

std::string IntegralType::getSubtractionOperator(){
    return "sub ";
}

std::string IntegralType::getMultiplicationOperator(){
    if(_signed){
        return "multu ";
    }
    return "mult ";
}

std::string IntegralType::getDivisionOperator(){
    if(_signed){
        return "divu ";
    }
    return "div ";
}

void IntegralType::extractFromMultRegister(Bindings *bindings){
    std::cout << "mflo " << this->getRegister(RegisterType::evaluateReg) <<std::endl;
    this->extractFromRegister(bindings,RegisterType::evaluateReg);
    //std::cout << "lw    $v0" << "," << bindings->currentOffset() << "($fp)" << std::endl;
}

void IntegralType::extractFromDivRegister(Bindings *bindings){
    std::cout << "mflo " << this->getRegister(RegisterType::evaluateReg) <<std::endl;
    this->extractFromRegister(bindings,RegisterType::evaluateReg);
    //std::cout << "lw    $v0" << "," << bindings->currentOffset() << "($fp)" << std::endl;
}

void IntegralType::extractFromModuloRegister(Bindings *bindings){
    std::cout << "mfhi " << this->getRegister(RegisterType::evaluateReg) <<std::endl;
    this->extractFromRegister(bindings,RegisterType::evaluateReg);
    //std::cout << "lw    $v0" << "," << bindings->currentOffset() << "($fp)" << std::endl;
}
//TODO
void IntegralType::beq(Bindings *bindings, RegisterType reg1, RegisterType reg2, std::string label){
    std::cout << "beq " << this->getRegister(reg1)
        << "," << this->getRegister(reg2) << "," << label << std::endl; 
}

Expression* IntegralType::getZero(){
    NumberConstant *zero = new NumberConstant(0);
    return zero;
}

//int
Int::Int(){}

std::string Int::getRegister(RegisterType type){
    if(type == RegisterType::leftReg){
        return "$v0";
    }
    if(type == RegisterType::rightReg){
        return "$v1";
    }
    if(type == RegisterType::evaluateReg){
        return "$t8";
    }
    if(type == RegisterType::returnReg){
        return "$v0";
    }
    if(type == RegisterType::zeroReg){
        return "$zero";
    }
    return "v0";
}

std::string Int::getName(){
    return "int";
}

int Int::getSize(){
    return this->size;
}

void Int::loadParameter(ReturnRegisters &returnRegisters, Bindings *bindings){
    if(returnRegisters.a0 == false && returnRegisters.f12 == false){
        std::cout << "sw      $a0," <<  bindings->currentOffset() << "($sp)" << std::endl;
        returnRegisters.a0 = true;
    }
    else if(returnRegisters.a1 == false && returnRegisters.f14 == false){
        std::cout << "sw      $a1," <<  bindings->currentOffset() << "($sp)" << std::endl;
        returnRegisters.a1 = true;
    }
    else if(returnRegisters.a2 == false){
        std::cout << "sw      $a2," <<  bindings->currentOffset() << "($sp)" << std::endl;
        returnRegisters.a2 = true;
    }
    else if(returnRegisters.a3 == false){
        std::cout << "sw      $a3," <<  bindings->currentOffset() << "($sp)" << std::endl;
        returnRegisters.a3 = true;
    }
    else{
        //load the value from memory 
        //store it in the new frame
        std::cout << "lw      $v1," <<  returnRegisters.currentMemOffset << "($sp)" << std::endl;
        std::cout << "sw      $v1," <<  bindings->currentOffset() << "($sp)" << std::endl;
    }
    returnRegisters.currentMemOffset += this->size;
}
//TODO
void Int::saveParameter(ReturnRegisters &returnRegisters, Bindings *bindings){
    if(returnRegisters.a0 == false && returnRegisters.f12 == false){
        std::cout << "lw      $a0," <<  bindings->currentOffset() << "($sp)" << std::endl;
        returnRegisters.a0 = true;
    }
    else if(returnRegisters.a1 == false && returnRegisters.f14 == false){
        std::cout << "lw      $a1," <<  bindings->currentOffset() << "($sp)" << std::endl;
        returnRegisters.a1 = true;
    }
    else if(returnRegisters.a2 == false){
        std::cout << "lw      $a2," <<  bindings->currentOffset() << "($sp)" << std::endl;
        returnRegisters.a2 = true;
    }
    else if(returnRegisters.a3 == false){
        std::cout << "lw      $a3," <<  bindings->currentOffset() << "($sp)" << std::endl;
        returnRegisters.a3 = true;
    }
}

void Int::evaluateReturn(Bindings *bindings){
    std::cout << "lw      " << this->getRegister(RegisterType::returnReg) 
        << "," <<  bindings->currentOffset() << "($fp)" << std::endl;
}

void Int::processReturn(Bindings *bindings){
    //move the value from the return register to the top 
    std::cout << "sw      " << this->getRegister(RegisterType::returnReg) 
        << "," <<  bindings->currentOffset() << "($fp)" << std::endl;
}

void Int::placeInRegister(Bindings *bindings, RegisterType type){
    std::cout << "lw    " << this->getRegister(type) 
        << "," << bindings->currentOffset() << "($fp)" << std::endl;
}

void Int::extractFromRegister(Bindings *bindings, RegisterType type){
    std::cout << "sw    " << this->getRegister(type) 
        << "," << bindings->currentOffset() << "($fp)" << std::endl;
}

void Int::extractFromregister(Bindings *bindings, RegisterType type, RegisterType address){
    std::cout << "sw    " << this->getRegister(type) 
        << "," << "0" << "(" << this->getRegister(address) << ")" << std::endl;
}

void Int::saveVariable(Bindings *bindings, std::string id){
    std::cout << "lw    " << this->getRegister(RegisterType::evaluateReg) << ","
        << bindings->currentOffset() << "($fp)" << std::endl;
    std::cout << "sw    " << this->getRegister(RegisterType::evaluateReg) << ","
        << bindings->stackPosition(id) << "($fp)" << std::endl;
}

void Int::saveVariable(Bindings *bindings, int offset){
    std::cout << "lw    " << this->getRegister(RegisterType::evaluateReg) << ","
        << bindings->currentOffset() << "($fp)" << std::endl;
    std::cout << "sw    " << this->getRegister(RegisterType::evaluateReg) << ","
        << offset << "($fp)" << std::endl;
}

void Int::placeVariableOnStack(Bindings *bindings, std::string id){
    std::cout << "lw    " << this->getRegister(RegisterType::evaluateReg)
        << "," << bindings->stackPosition(id) << "($fp)" << std::endl;
    this->extractFromRegister(bindings,RegisterType::evaluateReg);
}

void Int::placeVariableOnStack(Bindings *bindings, int offset){
    std::cout << "lw    " << this->getRegister(RegisterType::evaluateReg)
        << "," << offset << "($fp)" << std::endl;
    this->extractFromRegister(bindings,RegisterType::evaluateReg);
}

void Int::placeVariableOnStack(Bindings *bindings){
    //load the address from the stack
    std::cout << "lw    " << "$t9"
        << "," << bindings->currentOffset() << "($fp)" << std::endl;
    //put the value at the address into evaluate register
    std::cout << "lw    " << this->getRegister(RegisterType::evaluateReg)
        << "," << 0 << "("  << "$t9" << ")" << std::endl;
    //put the value in the evaluate register onto the stack
    //std::cout << "sw    " << this->getRegister(RegisterType::evaluateReg)
    //    << "," << bindings->currentOffset() << "($fp)" << std::endl;
    this->extractFromRegister(bindings,RegisterType::evaluateReg);
}

//pointer
Pointer::Pointer(Type *type){
    this->type = type;
}

void Pointer::defreference(Bindings *bindings){
    this->type->placeVariableOnStack(bindings);
}

Type* Pointer::getType(){
    return type;
}

void Pointer::storeDereference(Bindings *bindings){
    //get the variable on the stack and put it in the evaluate register
    std::cout << "lw    " << type->getRegister(RegisterType::leftReg) << ","
        << bindings->currentOffset() << "($fp)" << std::endl;
    //store the pointed address in the evaluate register
    std::cout << "lw    " << type->getRegister(RegisterType::leftReg) << ","
        << bindings->currentOffset() << "($fp)" << std::endl;
}

void Pointer::initialise(Bindings *bindings){
    //save the bindings offset at the bindings offset
    //store the current memory location at the previouse mempry location
    int size = type->getSize();
    std::cout << "addiu $v0,$fp," << bindings->currentOffset() - (size-4) <<std::endl;
    std::cout << "sw    $v0," << bindings->currentOffset()+4 << "($fp)" << std::endl;
    //bindings->setOffset(bindings->currentOffset() - 4);
    //NumberConstant *constant = new NumberConstant(bindings->currentOffset()+4);
    //constant->printASM(bindings);
    //bindings->setOffset(bindings->currentOffset() + 4);
    //delete constant
    bindings->setOffset(bindings->currentOffset() - size);
}

Array::Array(Type *type, int arraySize){
    this->type = type;
    this->arraySize = arraySize;
}

void Array::initialise(Bindings *bindings){
    //save the bindings offset at the bindings offset
    //bindings->setOffset(bindings->currentOffset() - 4);
    //NumberConstant *constant = new NumberConstant(bindings->currentOffset()+4);
    //constant->printASM(bindings);
    //bindings->setOffset(bindings->currentOffset() + 4);
    //delete constant;
    int total = type->getSize()*arraySize;
    std::cout << "addiu $v0,$fp," << bindings->currentOffset() - (total-4) <<std::endl;
    std::cout << "sw    $v0," << bindings->currentOffset()+4 << "($fp)" << std::endl;
    bindings->setOffset(bindings->currentOffset() - total);
}

//char
Char::Char(){}
//-> check this is correct
int Char::getSize() {
    return this->size;
}

std::string Char::getName() {
    return "char";
}
//-> check this is correct
void Char::loadParameter(ReturnRegisters &returnRegisters, Bindings *bindings) {
    if(returnRegisters.a0 == false && returnRegisters.f12 == false){
        std::cout << "sb      $a0," <<  bindings->currentOffset() << "($sp)" << std::endl;
        returnRegisters.a0 = true;
    }
    else if(returnRegisters.a1 == false && returnRegisters.f14 == false){
        std::cout << "sb      $a1," <<  bindings->currentOffset() << "($sp)" << std::endl;
        returnRegisters.a1 = true;
    }
    else if(returnRegisters.a2 == false){
        std::cout << "sb      $a2," <<  bindings->currentOffset() << "($sp)" << std::endl;
        returnRegisters.a2 = true;
    }
    else if(returnRegisters.a3 == false){
        std::cout << "sb      $a3," <<  bindings->currentOffset() << "($sp)" << std::endl;
        returnRegisters.a3 = true;
    }
    else{
        //load the value from memory 
        //store it in the new frame
        std::cout << "lb      $v1," <<  returnRegisters.currentMemOffset << "($sp)" << std::endl;
        std::cout << "sb      $v1," <<  bindings->currentOffset() << "($sp)" << std::endl;
    }
    returnRegisters.currentMemOffset += this->size;
}
//TODO
void Char::saveParameter(ReturnRegisters &returnRegisters, Bindings *bindings){
    if(returnRegisters.a0 == false && returnRegisters.f12 == false){
        std::cout << "lb      $a0," <<  bindings->currentOffset() << "($sp)" << std::endl;
        returnRegisters.a0 = true;
    }
    else if(returnRegisters.a1 == false && returnRegisters.f14 == false){
        std::cout << "lb      $a1," <<  bindings->currentOffset() << "($sp)" << std::endl;
        returnRegisters.a1 = true;
    }
    else if(returnRegisters.a2 == false){
        std::cout << "lb      $a2," <<  bindings->currentOffset() << "($sp)" << std::endl;
        returnRegisters.a2 = true;
    }
    else if(returnRegisters.a3 == false){
        std::cout << "lb      $a3," <<  bindings->currentOffset() << "($sp)" << std::endl;
        returnRegisters.a3 = true;
    }
}
//-> check this is correct
void Char::evaluateReturn(Bindings *bindings) {
    std::cout << "lb      " << this->getRegister(RegisterType::returnReg) 
        << "," <<  bindings->currentOffset() << "($fp)" << std::endl;
}
//-> check this is correct
void Char::processReturn(Bindings *bindings) {
    std::cout << "sb      " << this->getRegister(RegisterType::returnReg) 
        << "," <<  bindings->currentOffset() << "($fp)" << std::endl;
}
//-> check this is correct
void Char::placeInRegister(Bindings *bindings, RegisterType type) {
    std::cout << "lb    " << this->getRegister(type) 
        << "," << bindings->currentOffset() << "($fp)" << std::endl;
}
//-> check this is correct --> different from int
void Char::extractFromRegister(Bindings *bindings, RegisterType type) {
    //std::cout << "andi " << this->getRegister(type)
    //    << "," << this->getRegister(type) << ",255" << std::endl;
    std::cout << "sb    " << this->getRegister(type) 
        << "," << bindings->currentOffset() << "($fp)" << std::endl;
}
//-> check this is correct --> different from int
void Char::extractFromregister(Bindings *bindings, RegisterType type, RegisterType address){
    //std::cout << "andi " << this->getRegister(type)
    //    << "," << this->getRegister(type) << ",255" << std::endl;
    std::cout << "sb    " << this->getRegister(type) 
        << "," << "0" << "(" << this->getRegister(address) << ")" << std::endl;
}
//-> check this is correct
void Char::saveVariable(Bindings *bindings, std::string id) {
    std::cout << "lb    " << this->getRegister(RegisterType::evaluateReg) << ","
        << bindings->currentOffset() << "($fp)" << std::endl;
    std::cout << "sb    " << this->getRegister(RegisterType::evaluateReg) << ","
        << bindings->stackPosition(id) << "($fp)" << std::endl;
}
//-> check this is 
void Char::saveVariable(Bindings *bindings, int offset){
    std::cout << "lb    " << this->getRegister(RegisterType::evaluateReg) << ","
        << bindings->currentOffset() << "($fp)" << std::endl;
    std::cout << "sb    " << this->getRegister(RegisterType::evaluateReg) << ","
        << offset << "($fp)" << std::endl;
}


void Char::placeVariableOnStack(Bindings *bindings, std::string id) {
    std::cout << "lb    " << this->getRegister(RegisterType::evaluateReg)
        << "," << bindings->stackPosition(id) << "($fp)" << std::endl;
    this->extractFromRegister(bindings,RegisterType::evaluateReg);
}

void Char::placeVariableOnStack(Bindings *bindings, int offset){
    std::cout << "lb    " << this->getRegister(RegisterType::evaluateReg)
        << "," << offset << "($fp)" << std::endl;
    this->extractFromRegister(bindings,RegisterType::evaluateReg);
}
//-> check this is correct
void Char::placeVariableOnStack(Bindings *bindings) {
    // //load the address from the stack
    // std::cout << "sb    " << this->getRegister(RegisterType::evaluateReg)
    //     << "," << bindings->currentOffset() << "($fp)" << std::endl;
    // //put the value at the address into evaluate register
    // std::cout << "lb    " << this->getRegister(RegisterType::evaluateReg)
    //     << "," << 0 << "("  << this->getRegister(RegisterType::evaluateReg) << ")" << std::endl;
    // //put the value in the evaluate register onto the stack
    // std::cout << "sb    " << this->getRegister(RegisterType::evaluateReg)
    //     << "," << bindings->currentOffset() << "($fp)" << std::endl;
    // this->extractFromRegister(bindings,RegisterType::evaluateReg);

    //load the address from the stack
    std::cout << "lw    " << "$t9"
        << "," << bindings->currentOffset() << "($fp)" << std::endl;
    //put the value at the address into evaluate register
    std::cout << "lb    " << this->getRegister(RegisterType::evaluateReg)
        << "," << 0 << "("  << "$t9" << ")" << std::endl;
    this->extractFromRegister(bindings,RegisterType::evaluateReg);
}
//-> check this is correct
std::string Char::getRegister(RegisterType type) {
    if(type == RegisterType::leftReg){
        return "$v0";
    }
    if(type == RegisterType::rightReg){
        return "$v1";
    }
    if(type == RegisterType::evaluateReg){
        return "$t8";
    }
    if(type == RegisterType::returnReg){
        return "$v0";
    }
    return "v0";
}

//float
Float::Float(){}

int Float::getSize(){
    return this->size;
}

std::string Float::getName(){
    return "float";
}
//-> check this is correct
void Float::loadParameter(ReturnRegisters &returnRegisters, Bindings *bindings){
    bool integer = (returnRegisters.a0 == false) && (returnRegisters.a1 == false) && (returnRegisters.a2 == false) && (returnRegisters.a3 == false);
    if(returnRegisters.f12 == false && integer){
        std::cout << "s.s      $f12," <<  bindings->currentOffset() << "($sp)" << std::endl;
        returnRegisters.f12 = true;
    }
    else if(returnRegisters.f14 == false && integer){
        std::cout << "s.s      $f14," <<  bindings->currentOffset() << "($sp)" << std::endl;
        returnRegisters.f14 = true;
    }
    else if(returnRegisters.a1 == false && returnRegisters.f14 == false){
        std::cout << "sw      $a1," <<  bindings->currentOffset() << "($sp)" << std::endl;
        returnRegisters.a1 = true;
    }
    else if(returnRegisters.a2 == false){
        std::cout << "sw      $a2," <<  bindings->currentOffset() << "($sp)" << std::endl;
        returnRegisters.a2 = true;
    }
    else if(returnRegisters.a3 == false){
        std::cout << "sw      $a3," <<  bindings->currentOffset() << "($sp)" << std::endl;
        returnRegisters.a3 = true;
    }
    else{
        std::cout << "l.s      " << this->getRegister(RegisterType::evaluateReg) 
            << "," <<  returnRegisters.currentMemOffset << "($sp)" << std::endl;
        std::cout << "s.s      " << this->getRegister(RegisterType::evaluateReg) 
            << "," <<  bindings->currentOffset() << "($sp)" << std::endl;
    }
    returnRegisters.currentMemOffset += this->size;
}
//TODO
void Float::saveParameter(ReturnRegisters &returnRegisters, Bindings *bindings){
    bool integer = (returnRegisters.a0 == false) && (returnRegisters.a1 == false) && (returnRegisters.a2 == false) && (returnRegisters.a3 == false);
    if(returnRegisters.f12 == false && integer){
        std::cout << "l.s      $f12," <<  bindings->currentOffset() << "($sp)" << std::endl;
        returnRegisters.f12 = true;
    }
    else if(returnRegisters.f14 == false && integer){
        std::cout << "l.s      $f14," <<  bindings->currentOffset() << "($sp)" << std::endl;
        returnRegisters.f14 = true;
    }
    else if(returnRegisters.a1 == false && returnRegisters.f14 == false){
        std::cout << "lw      $a1," <<  bindings->currentOffset() << "($sp)" << std::endl;
        returnRegisters.a1 = true;
    }
    else if(returnRegisters.a2 == false){
        std::cout << "lw      $a2," <<  bindings->currentOffset() << "($sp)" << std::endl;
        returnRegisters.a2 = true;
    }
    else if(returnRegisters.a3 == false){
        std::cout << "lw      $a3," <<  bindings->currentOffset() << "($sp)" << std::endl;
        returnRegisters.a3 = true;
    }
}
//-> check this is correct
void Float::evaluateReturn(Bindings *bindings){
    std::cout << "l.s      " << this->getRegister(RegisterType::returnReg) 
        << "," <<  bindings->currentOffset() << "($fp)" << std::endl;
}
//-> check this is correct
void Float::processReturn(Bindings *bindings){
    std::cout << "s.s      " << this->getRegister(RegisterType::returnReg) 
        << "," <<  bindings->currentOffset() << "($fp)" << std::endl;
}
//-> check this is correct
void Float::placeInRegister(Bindings *bindings, RegisterType type){
    std::cout << "l.s    " << this->getRegister(type) 
        << "," << bindings->currentOffset() << "($fp)" << std::endl;
}
//-> check this is correct
void Float::extractFromRegister(Bindings *bindings, RegisterType type){
    std::cout << "s.s    " << this->getRegister(type) 
        << "," << bindings->currentOffset() << "($fp)" << std::endl;
}

void Float::extractFromregister(Bindings *bindings, RegisterType type, RegisterType address){
    std::cout << "s.s    " << this->getRegister(type) 
        << "," << "0" << "(" << this->getRegister(address) << ")" << std::endl;
}
//-> check this is correct
std::string Float::getRegister(RegisterType type){
    if(type == RegisterType::leftReg){
        return "$f4";
    }
    if(type == RegisterType::rightReg){
        return "$f6";
    }
    if(type == RegisterType::evaluateReg){
        return "$f10";
    }
    if(type == RegisterType::returnReg){
        return "$f0";
    }
    return "$f0";
}
//-> check this is correct
void Float::saveVariable(Bindings *bindings, std::string id){
    std::cout << "l.s    " << this->getRegister(RegisterType::evaluateReg) << ","
        << bindings->currentOffset() << "($fp)" << std::endl;
    std::cout << "s.s    " << this->getRegister(RegisterType::evaluateReg) << ","
        << bindings->stackPosition(id) << "($fp)" << std::endl;
}

void Float::saveVariable(Bindings *bindings, int offset){
    std::cout << "l.s    " << this->getRegister(RegisterType::evaluateReg) << ","
        << bindings->currentOffset() << "($fp)" << std::endl;
    std::cout << "s.s    " << this->getRegister(RegisterType::evaluateReg) << ","
        << offset << "($fp)" << std::endl;
}
//-> check this is correct
void Float::placeVariableOnStack(Bindings *bindings, std::string id){
    std::cout << "l.s    " << this->getRegister(RegisterType::evaluateReg)
        << "," << bindings->stackPosition(id) << "($fp)" << std::endl;
    this->extractFromRegister(bindings,RegisterType::evaluateReg);
}
//-> check this is correct
void Float::placeVariableOnStack(Bindings *bindings){
    //load the address from the stack
    std::cout << "s.s    " << "$t9"
        << "," << bindings->currentOffset() << "($fp)" << std::endl;
    //put the value at the address into evaluate register
    std::cout << "l.s    " << this->getRegister(RegisterType::evaluateReg)
        << "," << 0 << "("  << "$t9" << ")" << std::endl;
    //put the value in the evaluate register onto the stack
    std::cout << "s.s    " << this->getRegister(RegisterType::evaluateReg)
        << "," << bindings->currentOffset() << "($fp)" << std::endl;
    this->extractFromRegister(bindings,RegisterType::evaluateReg);
}

void Float::placeVariableOnStack(Bindings *bindings, int offset){
    std::cout << "l.s    " << this->getRegister(RegisterType::evaluateReg)
        << "," << offset << "($fp)" << std::endl;
    this->extractFromRegister(bindings,RegisterType::evaluateReg);
}
//-> check this is correct
std::string Float::getAdditionOperator(){
    return "add.s";
}
//-> check this is correct
std::string Float::getSubtractionOperator(){
    return "sub.s";
}
//-> check this is correct
std::string Float::getMultiplicationOperator(){
    return "mul.s " + this->getRegister(evaluateReg) + ",";
}
//-> check this is correct
std::string Float::getDivisionOperator(){
    return "div.s" + this->getRegister(evaluateReg) + ",";
}

void Float::extractFromMultRegister(Bindings *bindings){
//doesn't need to do anything as is already in evaluate register
    this->extractFromRegister(bindings,RegisterType::evaluateReg);
}

void Float::extractFromDivRegister(Bindings *bindings){
//doesnt need to do anything as is already in  
    this->extractFromRegister(bindings,RegisterType::evaluateReg);
    return;
}

void Float::beq(Bindings *bindings, RegisterType reg1, RegisterType reg2, std::string label){
    std::string reg1String;
    std::string reg2String;
    if(reg1 == RegisterType::zeroReg){
        std::cout << "li.d $f0, 0" << std::endl;
        reg1String = "$f0";
    }else{
        reg1String = this->getRegister(reg1);
    }
    if(reg2 == RegisterType::zeroReg){
        std::cout << "li.d $f0, 0" << std::endl;
        reg2String = "$f0";
    }else{
        reg2String = this->getRegister(reg1);
    }
    std::cout << "c.e.d " << reg1String << "," << reg2String << std::endl;
    std::cout << "bc1t " << label << std::endl;
}

Expression* Float::getZero(){
    FloatConstant *zero = new FloatConstant(0);
    return zero;
}

//double
Double::Double(){}

int Double::getSize(){
    return this->size;
}

std::string Double::getName(){
    return "double";
}
//todo
void Double::loadParameter(ReturnRegisters &returnRegisters, Bindings *bindings){
    bool integer = (returnRegisters.a0 == false) && (returnRegisters.a1 == false) && (returnRegisters.a2 == false) && (returnRegisters.a3 == false);
    if(returnRegisters.f12 == false && integer == false){
        std::cout << "s.d      $f12," <<  bindings->currentOffset() << "($sp)" << std::endl;
        returnRegisters.f12 = true;
        returnRegisters.f13 = true;
    }
    else if(returnRegisters.f14 == false && integer == false){
        std::cout << "s.d      $f14," <<  bindings->currentOffset() << "($sp)" << std::endl;
        returnRegisters.f14 = true;
        returnRegisters.f15 = true;
    }
    else if(returnRegisters.a1 == false && returnRegisters.f14 == false){
        std::cout << "sw      $a1," <<  bindings->currentOffset() << "($sp)" << std::endl;
        std::cout << "sw      $a2," <<  bindings->currentOffset() + 4 << "($sp)" << std::endl;
        returnRegisters.a1 = true;
        returnRegisters.a2 = true;
    }
    else if(returnRegisters.a2 == false){
        std::cout << "sw      $a2," <<  bindings->currentOffset() << "($sp)" << std::endl;
        std::cout << "sw      $a3," <<  bindings->currentOffset() + 4 << "($sp)" << std::endl;
        returnRegisters.a2 = true;
        returnRegisters.a3 = true;
    }
    else{
        std::cout << "l.d      " << this->getRegister(RegisterType::evaluateReg) 
            << "," <<  returnRegisters.currentMemOffset << "($sp)" << std::endl;
        std::cout <<"nop" <<std::endl;
        std::cout << "s.d      " << this->getRegister(RegisterType::evaluateReg) 
            << "," <<  bindings->currentOffset() << "($sp)" << std::endl;
    }
    returnRegisters.currentMemOffset += this->size;
}
//TODO
void Double::saveParameter(ReturnRegisters &returnRegisters, Bindings *bindings){
    bool integer = (returnRegisters.a0 == false) && (returnRegisters.a1 == false) && (returnRegisters.a2 == false) && (returnRegisters.a3 == false);
    if(returnRegisters.f12 == false && integer == false){
        std::cout << "l.d      $f12," <<  bindings->currentOffset() << "($sp)" << std::endl;
        returnRegisters.f12 = true;
        returnRegisters.f13 = true;
    }
    else if(returnRegisters.f14 == false && integer == false){
        std::cout << "l.d      $f14," <<  bindings->currentOffset() << "($sp)" << std::endl;
        returnRegisters.f14 = true;
        returnRegisters.f15 = true;
    }
    else if(returnRegisters.a1 == false && returnRegisters.f14 == false){
        std::cout << "lw      $a1," <<  bindings->currentOffset() << "($sp)" << std::endl;
        std::cout << "lw      $a2," <<  bindings->currentOffset() + 4 << "($sp)" << std::endl;
        returnRegisters.a1 = true;
        returnRegisters.a2 = true;
    }
    else if(returnRegisters.a2 == false){
        std::cout << "lw      $a2," <<  bindings->currentOffset() << "($sp)" << std::endl;
        std::cout << "lw      $a3," <<  bindings->currentOffset() + 4 << "($sp)" << std::endl;
        returnRegisters.a2 = true;
        returnRegisters.a3 = true;
    }
}
//todo
void Double::evaluateReturn(Bindings *bindings){
    std::cout << "l.s      " << this->getRegister(RegisterType::returnReg) 
        << "," <<  bindings->currentOffset() << "($fp)" << std::endl;
    std::cout <<"nop" <<std::endl;
}
//todo
void Double::processReturn(Bindings *bindings){
    std::cout << "s.s      " << this->getRegister(RegisterType::returnReg) 
        << "," <<  bindings->currentOffset() << "($fp)" << std::endl;
}
//----check
void Double::placeInRegister(Bindings *bindings, RegisterType type){
    std::cout << "l.d    " << this->getRegister(type) 
        << "," << bindings->currentOffset() << "($fp)" << std::endl;
    std::cout <<"nop" <<std::endl;
}
//----check
void Double::extractFromRegister(Bindings *bindings, RegisterType type){
    std::cout << "s.d    " << this->getRegister(type) 
        << "," << bindings->currentOffset() << "($fp)" << std::endl;
}
//----check
void Double::extractFromregister(Bindings *bindings, RegisterType type, RegisterType address){
    std::cout << "s.d    " << this->getRegister(type) 
        << "," << "0" << "(" << this->getRegister(address) << ")" << std::endl;
}
//----check
std::string Double::getRegister(RegisterType type){
    if(type == RegisterType::leftReg){
        return "$f4";
    }
    if(type == RegisterType::rightReg){
        return "$f6";
    }
    if(type == RegisterType::evaluateReg){
        return "$f8";
    }
    if(type == RegisterType::returnReg){
        return "$f0";
    }
    return "$f0";
}
//----check
void Double::saveVariable(Bindings *bindings, std::string id){
    std::cout << "l.d    " << this->getRegister(RegisterType::evaluateReg) << ","
        << bindings->currentOffset() << "($fp)" << std::endl;
    std::cout <<"nop" <<std::endl;
    std::cout << "s.d    " << this->getRegister(RegisterType::evaluateReg) << ","
        << bindings->stackPosition(id) << "($fp)" << std::endl;
}
//----check
void Double::saveVariable(Bindings *bindings, int offset){
    std::cout << "l.d    " << this->getRegister(RegisterType::evaluateReg) << ","
        << bindings->currentOffset() << "($fp)" << std::endl;
    std::cout <<"nop" <<std::endl;
    std::cout << "s.d    " << this->getRegister(RegisterType::evaluateReg) << ","
        << offset << "($fp)" << std::endl;
}

void Double::placeVariableOnStack(Bindings *bindings, std::string id){
    std::cout << "l.d    " << this->getRegister(RegisterType::evaluateReg)
        << "," << bindings->stackPosition(id) << "($fp)" << std::endl;
    std::cout <<"nop" <<std::endl;
    this->extractFromRegister(bindings,RegisterType::evaluateReg);
}

void Double::placeVariableOnStack(Bindings *bindings, int offset){
    std::cout << "l.d    " << this->getRegister(RegisterType::evaluateReg)
        << "," << offset << "($fp)" << std::endl;
    std::cout <<"nop" <<std::endl;
    this->extractFromRegister(bindings,RegisterType::evaluateReg);
}
//----check
void Double::placeVariableOnStack(Bindings *bindings){
    //load the address from the stack
    std::cout << "s.d    " << "$t9"
        << "," << bindings->currentOffset() << "($fp)" << std::endl;
    //put the value at the address into evaluate register
    std::cout << "l.d    " << this->getRegister(RegisterType::evaluateReg)
        << "," << 0 << "("  << "$t9" << ")" << std::endl;
    std::cout <<"nop" <<std::endl;
    //put the value in the evaluate register onto the stack
    std::cout << "s.d    " << this->getRegister(RegisterType::evaluateReg)
        << "," << bindings->currentOffset() << "($fp)" << std::endl;
    this->extractFromRegister(bindings,RegisterType::evaluateReg);
}
//----check
std::string Double::getAdditionOperator(){
    return "add.d";
}
//----check
std::string Double::getSubtractionOperator(){
    return "sub.d";
}
//----check
std::string Double::getMultiplicationOperator(){
    return "mul.d " + this->getRegister(evaluateReg) + ",";
}
//----check
std::string Double::getDivisionOperator(){

    return "div.d " + this->getRegister(evaluateReg) + ",";
}
//----check
void Double::extractFromMultRegister(Bindings *bindings){
    this->extractFromRegister(bindings,RegisterType::evaluateReg);
}
//----check
void Double::extractFromDivRegister(Bindings *bindings){
    this->extractFromRegister(bindings,RegisterType::evaluateReg);
}
//----check
void Double::beq(Bindings *bindings, RegisterType reg1, RegisterType reg2, std::string label){
    std::string reg1String;
    std::string reg2String;
    if(reg1 == RegisterType::zeroReg){
        std::cout << "li.d $f0, 0" << std::endl;
        reg1String = "$f0";
    }else{
        reg1String = this->getRegister(reg1);
    }
    if(reg2 == RegisterType::zeroReg){
        std::cout << "li.d $f0, 0" << std::endl;
        reg2String = "$f0";
    }else{
        reg2String = this->getRegister(reg1);
    }
    std::cout << "c.e.d " << reg1String << "," << reg2String << std::endl;
    std::cout << "bc1t " << label << std::endl;
}
//----check
Expression* Double::getZero(){
    DoubleConstant *zero = new DoubleConstant(0);
    return zero;
}