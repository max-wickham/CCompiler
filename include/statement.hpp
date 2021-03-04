#ifndef STATEMENT_HPP
#define STATEMENT_HPP

#include "node.hpp"
#include "expression.hpp"

class Statement: public Node{
    protected:
    Statement *nextStatement;
};

class FunctionCallStatement: public Statement{
    protected:
    std::string id;
    Parameter *parameter;
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



#endif