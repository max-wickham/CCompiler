#ifndef STATEMENT_HPP
#define STATEMENT_HPP

#include "node.hpp"
#include "expression.hpp"

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
};

class ReturnStatement: public Statement{
    protected:
    Expression *expression;
};

class IfElseStatement: public Statement{
    protected:
    Expression *condition;
    Statement *ifStatement;
    Statement *elseStatement;

    public:
    IfElseStatement(Expression *condition, Statement *ifStatement, Statement *elseStatement = nullptr);

};

class ForLoopStatement: public Statement{
    protected:
    Expression *definition;
    Expression *condition;
    Expression *incrementer;
    Statement *ifStatement;
    Statement *elseStatement;

    public:
    ForLoopStatement(Expression *definition, Expression *condition, Expression *incrementer,
        Statement *ifStatement, Statement *elseStatement = nullptr);

};

class ExpressionStatement: public Statement{
    protected:
    Expression *expression;
};


#endif