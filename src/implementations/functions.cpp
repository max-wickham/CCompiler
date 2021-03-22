#include "node.hpp"
#include "type.hpp"
#include "statement.hpp"
#include "parameter.hpp"
#include "bindings.hpp"
#include "function.hpp"
#include "enum.hpp"
#include <string>
#include <vector>
#include <iostream>

Function::Function(Decleration *decleration, Statement* statement,
        ParameterDefinition *firstParameter){
            this->type = decleration->type;
            this->id = decleration->id;
            this->statement = statement;
            this->firstParameter = firstParameter;
}


void Function::addToMap(std::map<std::string, Type*> &functionMap){
    functionMap[id] = type;
}

void Function::printASM(Bindings *bindings, std::vector<VariableDefinition*> globalVariables, std::vector<Enum*> enums){
    std::string params = "";
    //if(firstParameter != nullptr){
        //create the label string needed to find where to jump to
    //    firstParameter->createLabel(params, bindings);
    //}
    //print the current label
    std::cout << ".global " << id << std::endl;
    std::cout << id << ":" <<std::endl;
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
    bindings->setOffset(-16);
    //load the parameters into memory 
    ReturnRegisters returnRegisters;
    if(firstParameter != nullptr){
        firstParameter->createScope(bindings,returnRegisters);
    }
    //make the frame pointer equal to the stack pointer to begin a new frame
    std::cout << "move    $fp,$sp" << std::endl;
    //increase the bindings offset 
    for(VariableDefinition *definition: globalVariables){
        definition->printASM(bindings);
    }
    for(Enum* enumDef: enums){
        enumDef->addVariables(bindings);
    }
    bindings->addScope();
    statement->printASM(bindings);

    std::cout << "lw      $ra,-8($fp)" << std::endl;
    //the frame and stack pointer need to be reset
    std::cout << "move    $sp,$fp" << std::endl;
    std::cout << "lw      $fp,-4($fp)" << std::endl;
    //the load register needs to be jumped to
    std::cout << "jr     $ra" << std::endl;
    std::cout << "nop   " <<std::endl;

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