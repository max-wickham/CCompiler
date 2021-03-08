#include "node.hpp"
#include "type.hpp"
#include "bindings.hpp"
#include <iostream>


void Int::loadParameter(ReturnRegisters &returnRegisters, Bindings *bindings){
    if(returnRegisters.a0 == false){
        std::cout << " sw      $a0," <<  bindings->currentOffset() << "($sp)";
    }
    else if(returnRegisters.a1 == false){
        std::cout << " sw      $a0," <<  bindings->currentOffset() << "($sp)";
    }
    else if(returnRegisters.a2 == false){
        std::cout << " sw      $a0," <<  bindings->currentOffset() << "($sp)";
    }
    else if(returnRegisters.a3 == false){
        std::cout << " sw      $a0," <<  bindings->currentOffset() << "($sp)";
    }
    else{
        //load the value from memory 
        //store it in the new frame
        std::cout << " lw      $v1," <<  returnRegisters.currentMemOffset << "($sp)";
        std::cout << " sw      $v1," <<  bindings->currentOffset() << "($sp)";
    }
    returnRegisters.currentMemOffset += 4;
}