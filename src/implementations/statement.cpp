#include <iostream>
#include "statement.hpp"
#include "node.hpp"
#include "expression.hpp"
class Decleration;
#include "statement.hpp"

ScopedStatement::ScopedStatement(Statement *statement, Statement *nextStatement){
    this->statement = statement;
    this->nextStatement = nextStatement;
}   

BreakStatement::BreakStatement(Statement *nextStatement){
    this->nextStatement = nextStatement;
}

void BreakStatement::printASM(Bindings *bindings){
    std::cout << "j " << bindings->getBreak() << std::endl;
    std::cout <<"nop" <<std::endl;
    if(nextStatement != nullptr){
        nextStatement->printASM(bindings);
    }
}

ContinueStatement::ContinueStatement(Statement *nextStatement){
    this->nextStatement = nextStatement;
}

void ContinueStatement::printASM(Bindings *bindings){
    std::cout << "j " << bindings->getContinue() << std::endl;
    std::cout <<"nop" <<std::endl;
    if(nextStatement != nullptr){
        nextStatement->printASM(bindings);
    }
}

void ScopedStatement::printASM(Bindings* bindings){
    bindings->addScope();
    this->statement->printASM(bindings);
    bindings->deleteScope();
    if(this->nextStatement != nullptr){
        this->nextStatement->printASM(bindings);
    }
}

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
    std::cout << "nop   " <<std::endl;
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
    std::cout << ".global " << beginLabel << std::endl;
    std::cout << beginLabel << ":" << std::endl;
    //check that the condition is true
    condition->printASM(bindings);
    condition->getType(bindings)->placeInRegister(bindings,RegisterType::evaluateReg);
    //jump to the end if failed
    std::cout << "beq " << condition->getType(bindings)->getRegister(RegisterType::evaluateReg)
        << ", $zero, " << endLabel << std::endl;
    //run the statement
    bindings->addScope();
    if(statement != nullptr){
        statement->printASM(bindings);
    }
    bindings->deleteScope();
    //jump back to the begining
    std::cout << "j " << beginLabel << std::endl;
    std::cout << "nop " <<std::endl;
    std::cout << ".global " << endLabel << std::endl;
    std::cout << endLabel << ":" << std::endl;
    bindings->removeContinue();
    bindings->removeBreak();
    if(nextStatement != nullptr){
        nextStatement->printASM(bindings);
    }
}

ForLoopStatement::ForLoopStatement(Decleration *definition, Expression *condition, Expression *incrementer,
        Statement *statement, Statement *nextStatement){
    this->definitionDec = definition;
    this->condition = condition;
    this->incrementer = incrementer;
    this->statement = statement;
    this->nextStatement = nextStatement;
}

ForLoopStatement::ForLoopStatement(Expression *definition, Expression *condition, Expression *incrementer,
        Statement *statement, Statement *nextStatement){
    this->definitionExp = definition;
    this->condition = condition;
    this->incrementer = incrementer;
    this->statement = statement;
    this->nextStatement = nextStatement;
}

ForLoopStatement::ForLoopStatement(Expression *condition, Expression *incrementer,
        Statement *statement, Statement *nextStatement){
    this->condition = condition;
    this->incrementer = incrementer;
    this->statement = statement;
    this->nextStatement = nextStatement;
}

void ForLoopStatement::printASM(Bindings* bindings){
    std::string endLabel = bindings->createLabel("end");
    std::string beginLabel = bindings->createLabel("begin");
    std::string continueLabel = bindings->createLabel("continue");
    bindings->setBreak(endLabel);
    bindings->setContinue(continueLabel);
    bindings->addScope();
    //firstly create the definded varaible
    if(definitionDec != nullptr){
        VariableDefinition *variable = new VariableDefinition(definitionDec,nullptr);
        variable->printASM(bindings);
        delete variable;
    }
    if(definitionExp != nullptr){
        definitionExp->printASM(bindings);
    }
    //definition->printASM(bindings);
    std::cout << ".global " << beginLabel << std::endl;
    std::cout << beginLabel << ":" << std::endl;
    //check that the condition is true
    condition->printASM(bindings);
    condition->getType(bindings)->placeInRegister(bindings,RegisterType::evaluateReg);
    //jump to the end if failed
    std::cout << "beq " << condition->getType(bindings)->getRegister(RegisterType::evaluateReg)
        << ", $zero, " << endLabel << std::endl;
    //run the statement
    bindings->addScope();
    if(statement != nullptr){
        statement->printASM(bindings);
    }
    bindings->deleteScope();
    //run the incrementer
    std::cout << ".global " << continueLabel << std::endl;
    std::cout << continueLabel << ":" << std::endl;
    incrementer->printASM(bindings);
    //jump back to the begining
    std::cout << "j " << beginLabel << std::endl;
    std::cout << "nop " <<std::endl;
    std::cout << endLabel << ":";
    bindings->deleteScope();
    bindings->removeContinue();
    bindings->removeBreak();
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
    std::cout << "lw    $v0, " << bindings->currentOffset() << "($fp)" <<std::endl;
    //condition->getType(bindings)->placeInRegister(bindings,RegisterType::evaluateReg);
    std::string endElseLabel = bindings->createLabel("endelse");
    std::string elseLabel = bindings->createLabel("else");
    //std::cout << "lw $t0, " + std::to_string(bindings->currentOffset()) + "($zero)" << std::endl;
    //std::cout << "beq " << condition->getType(bindings)->getRegister(RegisterType::evaluateReg)
    std::cout << "beq " << "$v0"
        << ", $zero, " << elseLabel << std::endl;
    bindings->addScope();
    ifStatement->printASM(bindings);
    bindings->deleteScope();
    std::cout << "j " << endElseLabel << std::endl;
    std::cout << "nop " <<std::endl;
    std::cout << ".global " << elseLabel  << std::endl;
    std::cout << elseLabel << ":" << std::endl;
    bindings->addScope();
    if(elseStatement != nullptr){
        elseStatement->printASM(bindings);
    }
    bindings->deleteScope();
    std::cout << ".global " << endElseLabel << std::endl;
    std::cout << endElseLabel << ":" << std::endl;
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
    if(decleration->expression != nullptr){
        Variable *variable = new Variable(&decleration->id);
        AssignmentOperator *assignment = new AssignmentOperator(variable,decleration->expression);
        assignment->printASM(bindings);
        delete variable;
        delete assignment;
    }
    else if(dynamic_cast<Pointer*>(decleration->type) != nullptr){
        ((Pointer*)decleration->type)->initialise(bindings);
    }
    //std::cout << "end decleration " << std::endl;
    if(nextStatement != nullptr){
        this->nextStatement->printASM(bindings);
    }
}

StructDecleration::StructDecleration(std::string *structId, std::string *id, Statement *nextStatement){
    this->nextStatement = nextStatement;
    this->structId = *structId;
    this->id = *id;
}

void StructDecleration::printASM(Bindings* bindings){
    Struct *structT = bindings->getStruct(structId);
    Decleration *decleration = new Decleration(structT,&id);
    bindings->addVariable(decleration);
    if(nextStatement != nullptr){
        this->nextStatement->printASM(bindings);
    }
}

DoWhileLoopStatement::DoWhileLoopStatement(Expression *condition, Statement *statement, Statement *nextStatement){
    this->condition = condition;
    this->statement = statement;
    this->nextStatement = nextStatement;
}

void DoWhileLoopStatement::printASM(Bindings* bindings){
    statement->printASM(bindings);
    WhileLoopStatement *loop = new WhileLoopStatement(condition, statement, nextStatement);
    loop->printASM(bindings);
    delete loop;
}

SwitchStatement::SwitchStatement(Expression *expression, Statement *caseStatement, Statement *nextStatement){
    this->expression = expression;
    this->caseStatement = caseStatement;
    this->nextStatement = nextStatement;
}

void SwitchStatement::printASM(Bindings* bindings){
    std::string caseLabel = bindings->createLabel("case");
    bindings->setBreak(caseLabel);
    expression->printASM(bindings);
    caseStatement->printASM(bindings);
    std::cout << ".global " << caseLabel <<std::endl;
    std::cout << caseLabel << ":" << std::endl;
    if(nextStatement != nullptr){
        nextStatement->printASM(bindings);
    }
}

CaseStatement::CaseStatement(Expression *constant, Statement *statement, Statement *nextCaseStatement){
    this->constant = constant;
    this->statement = statement;
    this->nextStatement = nextCaseStatement;
}

void CaseStatement::printASM(Bindings* bindings){
    if(constant != nullptr){
        std::string endLabel = bindings->createLabel("endcase");
        std::string passLabel = bindings->createLabel("passcase");
        //place expression that is in stack in register
        int offset = bindings->currentOffset();
        bindings->setOffset(bindings->currentOffset() - constant->getType(bindings)->getSize());
        constant->printASM(bindings);
        constant->getType(bindings)->placeInRegister(bindings,RegisterType::rightReg);
        bindings->setOffset(offset);
        constant->getType(bindings)->placeInRegister(bindings,RegisterType::leftReg);
        bindings->setOffset(bindings->currentOffset() - constant->getType(bindings)->getSize());
        //constant->getType(bindings)->extractFromRegister(bindings,RegisterType::leftReg);
        constant->getType(bindings)->beq(bindings,RegisterType::leftReg,RegisterType::rightReg,passLabel);
        std::cout << "nop" << std::endl;
        std::cout << "j " << endLabel <<std::endl;
        std::cout << "nop" << std::endl;
        std::cout << ".global " << passLabel <<std::endl;
        std::cout << passLabel << ":" <<std::endl;
        statement->printASM(bindings);
       // std::cout << "j " << bindings->getBreak() <<std::endl;
       // std::cout << "nop" << std::endl;
        std::cout << ".global " << endLabel <<std::endl;
        std::cout << endLabel << ":" <<std::endl;
        //return bindings to expressoin
        bindings->setOffset(offset);
    }else{
       statement->printASM(bindings); 
    }
    if(nextStatement != nullptr){
        if(dynamic_cast<CaseStatement*>(nextStatement) == nullptr){
            bindings->removeBreak();
        }
        nextStatement->printASM(bindings);
    }
}