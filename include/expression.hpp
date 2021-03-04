#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include "node.hpp"

class Expression: public Node{

};

class Variable: public Expression{
    protected:
    std::string id;
    public:
    Variable(std::string id);
};

class StringConstant: public Expression{
    protected:
    std::string value;
    public:
    StringConstant(std::string value);
};

class NumberConstant: public Expression{
    protected:
    int value;
    public:
    NumberConstant(int value);
};

class BinaryOperatorExpression: public Expression{
    protected:
    Expression *leftExpression;
    Expression *rightExpression;

    public:
    BinaryOperatorExpression(Expression *leftExpression, Expression *rightExpression);
    void print() const;
    virtual std::string getOperator() const = 0;
};

class AssignmentOperator: public BinaryOperatorExpression{
    public:
    std::string getOperator() const;
};

class AdditionOperator: public BinaryOperatorExpression{
    public:
    std::string getOperator() const;
};

class SubtractionOperator: public BinaryOperatorExpression{
    
};

class MultiplicationOperator: public BinaryOperatorExpression{
    
};

class DivisionOperator: public BinaryOperatorExpression{
    
};

class EqualityOperator: public BinaryOperatorExpression{

};

class InequalityOperator: public BinaryOperatorExpression{

};


class UnaryOperatorExpression: public Expression{
    protected:
    Expression *expression;

    public:
    UnaryOperatorExpression(Expression *expression);
};

#endif