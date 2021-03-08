#include <iostream>

#include "node.hpp"
#include "expression.hpp"
#include "decleration.hpp"

#include "statement.hpp"


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