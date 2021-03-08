#include "node.hpp"
#include "type.hpp"
#include "statement.hpp"
#include "parameter.hpp"
#include "bindings.hpp"
#include "function.hpp"
#include <string>
#include <iostream>

Function::Function(Type *type, std::string& id, Statement* statement,
        ParameterDefinition *firstParameter = nullptr){
            this->type = type;
            this->id = id;
            this->statement = statement;
            this->firstParameter = firstParameter;
}

void Function::printASM(Bindings *bindings){
    //print the current label
    std::cout << id << "(" << type->getId() << "):";
    //store the current frame pointer in +4
    std::cout << "sw      $fp,4($sp)" << std::endl;
    //store the current return address in +8
    std::cout << "sw      $ra,8($sp)" << std::endl;
    //load the parameters into memory 
    ReturnRegisters returnRegisters;
    firstParameter->createScope(bindings,returnRegisters);
    //make the frame pointer equal to the stack pointer to begin a new frame
    std::cout << "move    $fp,$sp" << std::endl;
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
    firstParameter->createScope(bindings);
    //then print the asm of the statement
    
}