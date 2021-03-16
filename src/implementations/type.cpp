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

//integral type
std::string IntegralType::getAdditionOperator(){
    return "addu";
}

std::string IntegralType::getSubtractionOperator(){
    return "subbu";
}

std::string IntegralType::getMultiplicationOperator(){
    if(_signed){
        return "multu";
    }
    return "mult";
}

std::string IntegralType::getDivisionOperator(){
    if(_signed){
        return "div";
    }
    return "divu";
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
    if(returnRegisters.a0 == false){
        std::cout << " sw      $a0," <<  bindings->currentOffset() << "($sp)";
        returnRegisters.a0 = true;
    }
    else if(returnRegisters.a1 == false){
        std::cout << " sw      $a1," <<  bindings->currentOffset() << "($sp)";
        returnRegisters.a1 = true;
    }
    else if(returnRegisters.a2 == false){
        std::cout << " sw      $a2," <<  bindings->currentOffset() << "($sp)";
        returnRegisters.a2 = true;
    }
    else if(returnRegisters.a3 == false){
        std::cout << " sw      $a3," <<  bindings->currentOffset() << "($sp)";
        returnRegisters.a3 = true;
    }
    else{
        //load the value from memory 
        //store it in the new frame
        std::cout << " lw      $v1," <<  returnRegisters.currentMemOffset << "($sp)";
        std::cout << " sw      $v1," <<  bindings->currentOffset() << "($sp)";
    }
    returnRegisters.currentMemOffset += this->size;
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

void Int::saveVariable(Bindings *bindings, std::string id){
    std::cout << "lw    " << this->getRegister(RegisterType::evaluateReg) << ","
        << bindings->currentOffset() << "($fp)" << std::endl;
    std::cout << "sw    " << this->getRegister(RegisterType::evaluateReg) << ","
        << bindings->stackPosition(id) << "($fp)" << std::endl;
}

void Int::placeVariableOnStack(Bindings *bindings, std::string id){
    std::cout << "lw    " << this->getRegister(RegisterType::evaluateReg)
        << "," << bindings->stackPosition(id) << "($fp)" << std::endl;
    this->extractFromRegister(bindings,RegisterType::evaluateReg);
}

void Int::placeVariableOnStack(Bindings *bindings){
    //load the address from the stack
    std::cout << "sw    " << this->getRegister(RegisterType::evaluateReg)
        << "," << bindings->currentOffset() << "($fp)" << std::endl;
    //put the value at the address into evaluate register
    std::cout << "lw    " << this->getRegister(RegisterType::evaluateReg)
        << "," << 0 << "("  << this->getRegister(RegisterType::evaluateReg) << ")" << std::endl;
    //put the value in the evaluate register onto the stack
    std::cout << "sw    " << this->getRegister(RegisterType::evaluateReg)
        << "," << bindings->currentOffset() << "($fp)" << std::endl;
    this->extractFromRegister(bindings,RegisterType::evaluateReg);
}

//pointer
Pointer::Pointer(Type *type){
    this->type = type;
}

void Pointer::defreference(Bindings *bindings){
    this->type->placeVariableOnStack(bindings);
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
    if(returnRegisters.a0 == false){
        std::cout << " sw      $a0," <<  bindings->currentOffset() << "($sp)";
        returnRegisters.a0 = true;
    }
    else if(returnRegisters.a1 == false){
        std::cout << " sw      $a1," <<  bindings->currentOffset() << "($sp)";
        returnRegisters.a1 = true;
    }
    else if(returnRegisters.a2 == false){
        std::cout << " sw      $a2," <<  bindings->currentOffset() << "($sp)";
        returnRegisters.a2 = true;
    }
    else if(returnRegisters.a3 == false){
        std::cout << " sw      $a3," <<  bindings->currentOffset() << "($sp)";
        returnRegisters.a3 = true;
    }
    else{
        //load the value from memory 
        //store it in the new frame
        std::cout << " lw      $v1," <<  returnRegisters.currentMemOffset << "($sp)";
        std::cout << " sw      $v1," <<  bindings->currentOffset() << "($sp)";
    }
    returnRegisters.currentMemOffset += this->size;
}
//-> check this is correct
void Char::evaluateReturn(Bindings *bindings) {
    std::cout << "lw      " << this->getRegister(RegisterType::returnReg) 
        << "," <<  bindings->currentOffset() << "($fp)" << std::endl;
}
//-> check this is correct
void Char::processReturn(Bindings *bindings) {
    std::cout << "sw      " << this->getRegister(RegisterType::returnReg) 
        << "," <<  bindings->currentOffset() << "($fp)" << std::endl;
}
//-> check this is correct
void Char::placeInRegister(Bindings *bindings, RegisterType type) {
    std::cout << "lw    " << this->getRegister(type) 
        << "," << bindings->currentOffset() << "($fp)" << std::endl;
}
//-> check this is correct --> different from int
void Char::extractFromRegister(Bindings *bindings, RegisterType type) {
    std::cout << "andi " << this->getRegister(type)
        << "," << this->getRegister(type) << ",255" << std::endl;
    std::cout << "sw    " << this->getRegister(type) 
        << "," << bindings->currentOffset() << "($fp)" << std::endl;
}
//-> check this is correct
void Char::saveVariable(Bindings *bindings, std::string id) {
    std::cout << "lw    " << this->getRegister(RegisterType::evaluateReg) << ","
        << bindings->currentOffset() << "($fp)" << std::endl;
    std::cout << "sw    " << this->getRegister(RegisterType::evaluateReg) << ","
        << bindings->stackPosition(id) << "($fp)" << std::endl;
}
//-> check this is correct
void Char::placeVariableOnStack(Bindings *bindings, std::string id) {
    std::cout << "lw    " << this->getRegister(RegisterType::evaluateReg)
        << "," << bindings->stackPosition(id) << "($fp)" << std::endl;
    this->extractFromRegister(bindings,RegisterType::evaluateReg);
}
//-> check this is correct
void Char::placeVariableOnStack(Bindings *bindings) {
    //load the address from the stack
    std::cout << "sw    " << this->getRegister(RegisterType::evaluateReg)
        << "," << bindings->currentOffset() << "($fp)" << std::endl;
    //put the value at the address into evaluate register
    std::cout << "lw    " << this->getRegister(RegisterType::evaluateReg)
        << "," << 0 << "("  << this->getRegister(RegisterType::evaluateReg) << ")" << std::endl;
    //put the value in the evaluate register onto the stack
    std::cout << "sw    " << this->getRegister(RegisterType::evaluateReg)
        << "," << bindings->currentOffset() << "($fp)" << std::endl;
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
    if(returnRegisters.f12 == false){
        std::cout << " s.s      $f12," <<  bindings->currentOffset() << "($sp)";
        returnRegisters.f12 = true;
    }
    else if(returnRegisters.f14 == false){
        std::cout << " s.s      $f14," <<  bindings->currentOffset() << "($sp)";
        returnRegisters.f14 = true;
    }
    else{
        std::cout << " l.s      " << this->getRegister(RegisterType::evaluateReg) 
            << "," <<  returnRegisters.currentMemOffset << "($sp)";
        std::cout << " s.s      " << this->getRegister(RegisterType::evaluateReg) 
            << "," <<  bindings->currentOffset() << "($sp)";
    }
    returnRegisters.currentMemOffset += this->size;
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
//-> check this is correct
void Float::placeVariableOnStack(Bindings *bindings, std::string id){
    std::cout << "l.s    " << this->getRegister(RegisterType::evaluateReg)
        << "," << bindings->stackPosition(id) << "($fp)" << std::endl;
    this->extractFromRegister(bindings,RegisterType::evaluateReg);
}
//-> check this is correct
void Float::placeVariableOnStack(Bindings *bindings){
    //load the address from the stack
    std::cout << "s.s    " << this->getRegister(RegisterType::evaluateReg)
        << "," << bindings->currentOffset() << "($fp)" << std::endl;
    //put the value at the address into evaluate register
    std::cout << "l.s    " << this->getRegister(RegisterType::evaluateReg)
        << "," << 0 << "("  << this->getRegister(RegisterType::evaluateReg) << ")" << std::endl;
    //put the value in the evaluate register onto the stack
    std::cout << "s.s    " << this->getRegister(RegisterType::evaluateReg)
        << "," << bindings->currentOffset() << "($fp)" << std::endl;
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
    return;
}

void Float::extractFromDivRegister(Bindings *bindings){
//doesnt need to do anything as is already in  
    return;
}

void Float::beq(Bindings *bindings, RegisterType reg1, RegisterType reg2, std::string label){
    std::string reg1String;
    std::string reg2String;
    if(reg1 == RegisterType::zero){
        std::cout << "li.d $f0, 0" << std::endl;
        reg1String = "$f0";
    }else{
        reg1String = this->getRegister(reg1);
    }
    if(reg2 == RegisterType::zero){
        std::cout << "li.d $f0, 0" << std::endl;
        reg2String = "$f0";
    }else{
        reg2String = this->getRegister(reg1);
    }
    std::cout << "c.e.d " << reg1String << "," << reg2String << std::endl;
    std::cout << "bc1t " << label << std::endl;
}