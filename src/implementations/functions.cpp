#include "node.hpp"
#include "type.hpp"
#include "statement.hpp"
#include "parameter.hpp"
#include "bindings.hpp"
#include "function.hpp"
#include <string>
#include <iostream>

Function::Function(Type *type, std::string& id, Statement* statement,
        ParameterDefinition *firstParameter){
            this->type = type;
            this->id = id;
            this->statement = statement;
            this->firstParameter = firstParameter;
}

void Function::printASM(Bindings *bindings){
    bindings->addFunction(id, type);
    //print the current label
    std::cout << id << "(" << type->getName() << "):" <<std::endl;
    //store the current frame pointer in +4
    std::cout << "sw      $fp,-4($sp)" << std::endl;
    //store the current return address in +8
    std::cout << "sw      $ra,-8($sp)" << std::endl;
    //save the registers
    // std::cout << "sw      $s0,-12($sp)" << std::endl;
    // std::cout << "sw      $s1,-16($sp)" << std::endl;
    // std::cout << "sw      $s2,-20($sp)" << std::endl;
    // std::cout << "sw      $s3,-24($sp)" << std::endl;
    // std::cout << "sw      $s4,-28($sp)" << std::endl;
    // std::cout << "sw      $s5,-32($sp)" << std::endl;
    // std::cout << "sw      $s6,-36($sp)" << std::endl;
    // std::cout << "sw      $s7,-40($sp)" << std::endl;
    //set the current offset to + 44
    bindings->setOffset(-8);
    //load the parameters into memory 
    ReturnRegisters returnRegisters;
    if(firstParameter != nullptr){
        firstParameter->createScope(bindings,returnRegisters);
    }
    //make the frame pointer equal to the stack pointer to begin a new frame
    std::cout << "move    $fp,$sp" << std::endl;
    //increase the bindings offset 
    statement->printASM(bindings);

    //functions that return integral types should place that value in $2
    //functions that return floating point values should place that value in $f0
    //functions that return structs or unions should place the mem address in $4 and a pointer to the data in $2
    //the space for this data should be created by the function call
    //the return address should be in the return address register
    //---->copy saved registers into the stack
    //---->copy the return address
    //add the function to the current scope
    //load the variables into the bindings, 
    //each parameter should set its current stack position in the bindings
    //then print the asm of the statement
    
}