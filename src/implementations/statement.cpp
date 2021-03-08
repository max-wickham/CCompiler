#include <iostream>

#include "node.hpp"
#include "expression.hpp"
#include "decleration.hpp"

#include "statement.hpp"

ReturnStatement::ReturnStatement(Expression *expression){
    this->expression = expression;
}

void ReturnStatement::printASM(Bindings* bindings){
    //the expression needs to be evaluated,
    expression->printASM(bindings);
    //the expression needs to be placed in the return register or in the return memory space dependent on the type
    expression->getType()->evaluateReturn(bindings);
    //the save registers need to be loaded back
    std::cout << "sw      $s0,-12($fp)" << std::endl;
    std::cout << "sw      $s1,-16($fp)" << std::endl;
    std::cout << "sw      $s2,-20($fp)" << std::endl;
    std::cout << "sw      $s3,-24($fp)" << std::endl;
    std::cout << "sw      $s4,-28($fp)" << std::endl;
    std::cout << "sw      $s5,-32($fp)" << std::endl;
    std::cout << "sw      $s6,-36($fp)" << std::endl;
    std::cout << "sw      $s7,-40($fp)" << std::endl;
    //the return register needs to be loaded back 
    std::cout << "sw      $ra,-8($fp)" << std::endl;
    //the frame and stack pointer need to be reset
    std::cout << "move    $sp,$fp" << std::endl;
    std::cout << "lw      $fp,-4($fp)" << std::endl;
    //the load register needs to be jumped to
    stdd::cout << "jr     $ra" << std::endl;
}

WhileLoopStatement::WhileLoopStatement(Expression *condition, Statement *statement){
    this->condition = condition;
    this->statement = statement;
}

void WhileLoopStatement::printASM(Bindings* bindings){
    std::string endLabel = bindings->createLabel("end");
    std::string beginLabel = bindings->createLabel("begin");
    bindings->setBreak(endLabel);
    bindings->setContinue(beginLabel);
    std::cout << beginLabel << ":";
    //check that the condition is true
    condition->printASM(bindings,"$t0");
    //jump to the end if failed
    std::cout << "beq $t0, $zero, " << endLabel << std::endl;
    //Create a new scope
    Bindings newBindings = *bindings;
    //run the statement
    statement->printASM(&newBindings);
    //jump back to the begining
    std:cout << "j " << beginLabel << std::endl;
    std::cout << endLabel << ":";
    if(nextStatement != nullptr){
        nextStatement->printASM(bindings);
    }
}

ForLoopStatement::ForLoopStatement(Decleration *definition, Expression *condition, Expression *incrementer,
        Statement *statement){
    this->definition = definition;
    this->condition = condition;
    this->incrementer = incrementer;
    this->statement = statement;
}

void ForLoopStatement::printASM(Bindings* bindings){
    std::string endLabel = bindings->createLabel("end");
    std::string beginLabel = bindings->createLabel("begin");
    bindings->setBreak(end);
    bindings->setContinue(begin);
    //firstly create the definded varaible
    bindings->addVariable(decleration);
    std::cout << beginLabel << ":";
    //check that the condition is true
    condition->printASM(bindings,"$t0");
    //jump to the end if failed
    std::cout << "beq $t0, $zero, " << endLabel << std::endl;
    //Create a new scope
    Bindings newBindings = *bindings;
    //run the statement
    statement->printASM(&newBindings);
    //run the incrementer
    incrementer->printASM(bindings);
    //jump back to the begining
    std:cout << "j " << beginLabel << std::endl;
    std::cout << endLabel << ":";
    nextStatement->printASM(bindings);
}

IfElseStatement::IfElseStatement(Expression *condition, Statement *ifStatement, Statement *elseStatement = nullptr){
    this->condition = condition;
    this->ifStatement = ifStatement;
    this->elseStatement = elseStatement;
}

void IfElseStatement::printASM(Bindings* bindings){
    condition->printASM(bindings,"$t0");
    std::string endElseLabel = bindings->createLabel("endelse");
    std::string elseLabel = bindings->createLabel("else");
    //std::cout << "lw $t0, " + std::to_string(bindings->currentOffset()) + "($zero)" << std::endl;
    std::cout << "beq $t0, $zero, " << elseLabel << std::endl;
    //Create a new scope
    Bindings newBindings = *bindings;
    ifStatement->printASM(&newBindings);
    std:cout << "j " << endElseLabel << std::endl;
    std::cout << elseLabel << ":";
    elseStatement->printASM(&newBindings);
    std::cout << endElseLabel << ":";
}

ExpressionStatement::ExpressionStatement(Expression *expression, Statement *nextStatement = nullptr){
    this->expression = expression;
    this->nextStatement = nextStatement;
}

void ExpressionStatement::printASM(Bindings* bindings){
    this->expression->printASM(bindings);
}