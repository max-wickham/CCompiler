#ifndef STATEMENT_HPP
#define STATEMENT_HPP

#include "node.hpp"
#include "expression.hpp"
#include "decleration.hpp"

class Statement: public Node{
    protected:
    Statement *nextStatement;
    public:
    virtual void printASM(Bindings* bindings) const = 0;
};

class VariableDefinition: public Statement{
    protected:
    Type *type;
    std::string id;
    Expression *expression;
    public:
    VariableDefinition(Type *type, std::string id, Expression *expression = nullptr);
    void printASM(Bindings* bindings);
};

class ReturnStatement: public Statement{
    protected:
    Expression *expression;
    public:
    void printASM(Bindings* bindings);
};

class IfElseStatement: public Statement{
    protected:
    Expression *condition;
    Statement *ifStatement;
    Statement *elseStatement;

    public:
    IfElseStatement(Expression *condition, Statement *ifStatement, Statement *elseStatement = nullptr);
    void printASM(Bindings* bindings);
};

class ForLoopStatement: public Statement{
    protected:
    Decleration *definition;
    Expression *condition;
    Expression *incrementer;
    Statement *statement;

    public:
    ForLoopStatement(Decleration *definition, Expression *condition, Expression *incrementer,
        Statement *statement);
    void printASM(Bindings* bindings);

};

class WhileLoopStatement: public Statement{
    protected:
    Expression *condition;
    Statement *statement;
    WhileLoopStatement(Expression *condition, Statement *statement);
    void printASM(Bindings* bindings);
};

class ExpressionStatement: public Statement{
    protected:
    Expression *expression;
    ExpressionStatement(Expression *expression, Statement *nextStatement = nullptr);
    void printASM(Bindings* bindings);
};


#endif