#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include "node.hpp"
#include "bindings.hpp"

class Expression: public Node{
    public:
    virtual void printASM(Bindings *bindings, std::string returnRegister) const = 0;
};

class FunctionCall: public Expression{
    protected:
    std::string id;
    Parameter *parameter;
    public:
    void printASM(Bindings *bindings, std::string returnRegister);
};

class Variable: public Expression{
    protected:
    std::string id;
    public:
    Variable(std::string id);
    void printASM(Bindings *bindings, std::string returnRegister);
};

class StringConstant: public Expression{
    protected:
    std::string value;
    public:
    StringConstant(std::string value);
    void printASM(Bindings *bindings, std::string returnRegister);
};

class NumberConstant: public Expression{
    protected:
    int value;
    public:
    NumberConstant(int value);
    void printASM(Bindings *bindings, std::string returnRegister);
};

class BinaryOperatorExpression: public Expression{
    protected:
    Expression *leftExpression;
    Expression *rightExpression;

    public:
    BinaryOperatorExpression(Expression *leftExpression, Expression *rightExpression);
    void printASM(Bindings *bindings, std::string returnRegister);
    void print() const;
};

class AssignmentOperator: public BinaryOperatorExpression{
    public:
    void printASM(Bindings *bindings, std::string returnRegister);
};

class AdditionOperator: public BinaryOperatorExpression{
    public:
    void printASM(Bindings *bindings, std::string returnRegister);
};

class SubtractionOperator: public BinaryOperatorExpression{
    public:
    void printASM(Bindings *bindings, std::string returnRegister);
};

class MultiplicationOperator: public BinaryOperatorExpression{
    public:
    void printASM(Bindings *bindings, std::string returnRegister); 
};

class DivisionOperator: public BinaryOperatorExpression{
    public:
    void printASM(Bindings *bindings, std::string returnRegister);
};

class EqualityOperator: public BinaryOperatorExpression{
    public:
    void printASM(Bindings *bindings, std::string returnRegister);
};

class InequalityOperator: public BinaryOperatorExpression{
    public:
    void printASM(Bindings *bindings, std::string returnRegister);
};

class UnaryOperatorExpression: public Expression{
    protected:
    Expression *expression;

    public:
    void printASM(Bindings *bindings, std::string returnRegister);
    UnaryOperatorExpression(Expression *expression);
};

#endif