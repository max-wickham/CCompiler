#include <iostream>
#include "statement.hpp"
#include "node.hpp"
#include "expression.hpp"
class Decleration;

ReturnStatement::ReturnStatement(Expression *expression, Statement *nextStatement){
    this->expression = expression;
    this->nextStatement = nextStatement;
}

void ReturnStatement::printASM(Bindings* bindings){
    //the expression needs to be evaluated,
    expression->printASM(bindings);
    //the expression needs to be placed in the return register or in the return memory space dependent on the type
    expression->getType(bindings)->evaluateReturn(bindings);
    //the save registers need to be loaded back
    // std::cout << "sw      $s0,-12($fp)" << std::endl;
    // std::cout << "sw      $s1,-16($fp)" << std::endl;
    // std::cout << "sw      $s2,-20($fp)" << std::endl;
    // std::cout << "sw      $s3,-24($fp)" << std::endl;
    // std::cout << "sw      $s4,-28($fp)" << std::endl;
    // std::cout << "sw      $s5,-32($fp)" << std::endl;
    // std::cout << "sw      $s6,-36($fp)" << std::endl;
    // std::cout << "sw      $s7,-40($fp)" << std::endl;
    //the return register needs to be loaded back 
    std::cout << "lw      $ra,-8($fp)" << std::endl;
    //the frame and stack pointer need to be reset
    std::cout << "move    $sp,$fp" << std::endl;
    std::cout << "lw      $fp,-4($fp)" << std::endl;
    //the load register needs to be jumped to
    std::cout << "jr     $ra" << std::endl;
}

WhileLoopStatement::WhileLoopStatement(Expression *condition, Statement *statement, Statement *nextStatement){
    this->condition = condition;
    this->statement = statement;
    this->nextStatement = nextStatement;
}

void WhileLoopStatement::printASM(Bindings* bindings){
    std::string endLabel = bindings->createLabel("end");
    std::string beginLabel = bindings->createLabel("begin");
    bindings->setBreak(endLabel);
    bindings->setContinue(beginLabel);
    std::cout << beginLabel << ":";
    //check that the condition is true
    condition->printASM(bindings);
    condition->getType(bindings)->placeInRegister(bindings,RegisterType::evaluateReg);
    //jump to the end if failed
    std::cout << "beq " << condition->getType(bindings)->getRegister(RegisterType::evaluateReg)
        << ", $zero, " << endLabel << std::endl;
    //run the statement
    bindings->addScope();
    statement->printASM(bindings);
    bindings->deleteScope();
    //jump back to the begining
    std::cout << "j " << beginLabel << std::endl;
    std::cout << endLabel << ":";
    if(nextStatement != nullptr){
        nextStatement->printASM(bindings);
    }
}

ForLoopStatement::ForLoopStatement(Decleration *definition, Expression *initialiser, Expression *condition, Expression *incrementer,
        Statement *statement, Statement *nextStatement){
    this->definition = definition;
    this->initialiser = initialiser;
    this->condition = condition;
    this->incrementer = incrementer;
    this->statement = statement;
    this->nextStatement = nextStatement;
}

void ForLoopStatement::printASM(Bindings* bindings){
    std::string endLabel = bindings->createLabel("end");
    std::string beginLabel = bindings->createLabel("begin");
    bindings->setBreak(endLabel);
    bindings->setContinue(beginLabel);
    //firstly create the definded varaible
    if(definition != nullptr){
        bindings->addVariable(definition);
    }
    if(initialiser != nullptr){
        initialiser->printASM(bindings);
    }
    std::cout << beginLabel << ":";
    //check that the condition is true
    condition->printASM(bindings);
    condition->getType(bindings)->placeInRegister(bindings,RegisterType::evaluateReg);
    //jump to the end if failed
    std::cout << "beq " << condition->getType(bindings)->getRegister(RegisterType::evaluateReg)
        << ", $zero, " << endLabel << std::endl;
    //run the statement
    bindings->addScope();
    statement->printASM(bindings);
    bindings->deleteScope();
    //run the incrementer
    incrementer->printASM(bindings);
    //jump back to the begining
    std::cout << "j " << beginLabel << std::endl;
    std::cout << endLabel << ":";
    if(nextStatement != nullptr){
        this->nextStatement->printASM(bindings);
    }
}

IfElseStatement::IfElseStatement(Expression *condition, Statement *ifStatement, Statement *elseStatement, Statement *nextStatement){
    this->condition = condition;
    this->ifStatement = ifStatement;
    this->elseStatement = elseStatement;
    this->nextStatement = nextStatement;
}

void IfElseStatement::printASM(Bindings* bindings){
    condition->printASM(bindings);
    condition->getType(bindings)->placeInRegister(bindings,RegisterType::evaluateReg);
    std::string endElseLabel = bindings->createLabel("endelse");
    std::string elseLabel = bindings->createLabel("else");
    //std::cout << "lw $t0, " + std::to_string(bindings->currentOffset()) + "($zero)" << std::endl;
    std::cout << "beq " << condition->getType(bindings)->getRegister(RegisterType::evaluateReg)
        << ", $zero, " << elseLabel << std::endl;
    bindings->addScope();
    ifStatement->printASM(bindings);
    bindings->deleteScope();
    std::cout << "j " << endElseLabel << std::endl;
    std::cout << elseLabel << ":";
    bindings->addScope();
    elseStatement->printASM(bindings);
    bindings->deleteScope();
    std::cout << endElseLabel << ":";
    if(nextStatement != nullptr){
        this->nextStatement->printASM(bindings);
    }
}

ExpressionStatement::ExpressionStatement(Expression *expression, Statement *nextStatement){
    this->expression = expression;
    this->nextStatement = nextStatement;
}

void ExpressionStatement::printASM(Bindings* bindings){
    this->expression->printASM(bindings);
    if(nextStatement != nullptr){
        this->nextStatement->printASM(bindings);
    }
}

VariableDefinition::VariableDefinition(Decleration *decleration, Statement *nextStatement){
    this->decleration = decleration;
    this->nextStatement = nextStatement;   
}

void VariableDefinition::printASM(Bindings *bindings){
    bindings->addVariable(decleration);
    if(nextStatement != nullptr){
        this->nextStatement->printASM(bindings);
    }
}