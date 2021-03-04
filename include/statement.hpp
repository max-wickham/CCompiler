#ifndef STATEMENT_HPP
#define STATEMENT_HPP

#include "node.hpp"
#include "expression.hpp"

class Statement: public Node{

};

class IfElseStatement: public Statement{
    protected:
    Expression *condition;
    Statement *ifStatement;
    Statement *elseStatement;

    public:
    IfElseStatement(Expression *condition, Statement *ifStatement, Statement *elseStatement = nullptr);

}



#endif