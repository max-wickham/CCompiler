#ifndef STATEMENT_HPP
#define STATEMENT_HPP

#include "node.hpp"
#include "expression.hpp"
class Decleration;

class Statement: public Node{
    protected:
    Statement *nextStatement;
    public:
    virtual void printASM(Bindings* bindings) = 0;
};

class VariableDefinition: public Statement{
    protected:
    Decleration *decleration;
    public:
    VariableDefinition(Decleration *decleration, Statement *nextStatement);
    void printASM(Bindings* bindings) override;
};

class ReturnStatement: public Statement{
    protected:
    Expression *expression;
    public:
    ReturnStatement(Expression *expression, Statement *nextStatement);
    void printASM(Bindings* bindings) override;
};

class IfElseStatement: public Statement{
    protected:
    Expression *condition;
    Statement *ifStatement;
    Statement *elseStatement;

    public:
    IfElseStatement(Expression *condition, Statement *ifStatement, Statement *elseStatement,
    Statement *nextStatement = nullptr);
    void printASM(Bindings* bindings);
};

class ForLoopStatement: public Statement{
    protected:
    Decleration *definition;
    Expression *initialiser;
    Expression *condition;
    Expression *incrementer;
    Statement *statement;

    public:
    ForLoopStatement(Decleration *definition, Expression *initialiser, Expression *condition, Expression *incrementer,
        Statement *statement, Statement *nextStatement);
    void printASM(Bindings* bindings) override;

};

class WhileLoopStatement: public Statement{
    protected:
    Expression *condition;
    Statement *statement;
    WhileLoopStatement(Expression *condition, Statement *statement, Statement *nextStatement);
    void printASM(Bindings* bindings) override;
};
//TODO
class DoWhileLoopStatement: public Statement{
    protected:
    Expression *condition;
    Statement *statement;
    DoWhileLoopStatement(Expression *condition, Statement *statement, Statement *nextStatement);
    void printASM(Bindings* bindings) override;
};

class ExpressionStatement: public Statement{
    protected:
    Expression *expression;
    public:
    ExpressionStatement(Expression *expression, Statement *nextStatement);
    void printASM(Bindings* bindings) override;
};


#endif