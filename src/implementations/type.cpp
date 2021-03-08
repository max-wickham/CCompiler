#include "node.hpp"
#include "type.hpp"
#include "bindings.hpp"
#include <iostream>

std::string Int::getRegister(RegisterType type){
    if(type = RegisterType::leftReg){
        return "v0";
    }
    if(type = RegisterType::rightReg){
        return "v1";
    }
    if(type = RegisterType::evaluateReg){
        return "t8";
    }
    if(type = RegisterType::returnReg){
        return "v0";
    }
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
    std::cout << "lw    " << this->getRegister(type) << bindings->currentOffset() << "($fp)" << std::endl;
}

void Int::extractFromRegister(Bindings *bindings, RegisterType type){
    std::cout << "sw    " << this->getRegister(type) << bindings->currentOffset() << "($fp)" << std::endl;
}

std::string Bool::getRegister(RegisterType type){
    if(type = RegisterType::leftReg){
        return "v0";
    }
    if(type = RegisterType::rightReg){
        return "v1";
    }
    if(type = RegisterType::evaluateReg){
        return "t8";
    }
    if(type = RegisterType::returnReg){
        return "v0";
    }
}

std::string Bool::getName(){
    return "bool";
}

int Bool::getSize(){
    return this->size;
}

void Bool::loadParameter(ReturnRegisters &returnRegisters, Bindings *bindings){
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

void Bool::evaluateReturn(Bindings *bindings){
    std::cout << "lw      " << this->getRegister(RegisterType::returnReg) 
        << "," <<  bindings->currentOffset() << "($fp)" << std::endl;
}

void Bool::processReturn(Bindings *bindings){
    //move the value from the return register to the top 
    std::cout << "sw      " << this->getRegister(RegisterType::returnReg) 
        << "," <<  bindings->currentOffset() << "($fp)" << std::endl;
}

void Bool::placeInRegister(Bindings *bindings, RegisterType type){
    std::cout << "lw    " << this->getRegister(type) << bindings->currentOffset() << "($fp)" << std::endl;
}

void Bool::extractFromRegister(Bindings *bindings, RegisterType type){