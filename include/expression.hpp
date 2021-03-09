#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include "node.hpp"
class Bindings;
#include "type.hpp"
class Parameter;

class Expression: public Node{
    public:
    virtual void printASM(Bindings *bindings) = 0;
    //virtual void printASM(Bindings *bindings, std::string returnVariable) = 0;
    //virtual void printASM(Bindings *bindings, std::string returnRegister) = 0;
    virtual Type*  getType(Bindings *bindings) = 0;
};

class FunctionCall: public Expression{
    protected:
    std::string id;
    Parameter *parameter;
    public:
    FunctionCall(std::string id,Parameter *parameter);
    void printASM(Bindings *bindings);
    //void printASM(Bindings *bindings, std::string returnVariable);
    //void printASM(Bindings *bindings, std::string returnRegister);
};

class Variable: public Expression{
    protected:
    std::string id;
    public:
    Variable(std::string id);
    void printASM(Bindings *bindings) override;
    Type*  getType(Bindings *bindings) override;
    std::string getName();
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
    void printASM(Bindings *bindings) override;
    Type*  getType(Bindings *bindings) override;
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
    using BinaryOperatorExpression::BinaryOperatorExpression;
    void printASM(Bindings *bindings) override;
    Type*  getType(Bindings *bindings) override;
};

class AdditionOperator: public BinaryOperatorExpression{
    public:
    using BinaryOperatorExpression::BinaryOperatorExpression;
    void printASM(Bindings *bindings) override;
    //void printASM(Bindings *bindings, std::string returnRegister);
    Type* getType(Bindings *bindings) override;
};

class SubtractionOperator: public BinaryOperatorExpression{
    public:
    void printASM(Bindings *bindings) override;
    void printASM(Bindings *bindings, std::string returnRegister);
};

class MultiplicationOperator: public BinaryOperatorExpression{
    public:
    void printASM(Bindings *bindings, std::string returnRegister); 
};

class DivisionOperator: public BinaryOperatorExpression{
    public:
    void printASM(Bindings *bindings);
};

class EqualityOperator: public BinaryOperatorExpression{
    public:
    void printASM(Bindings *bindings);
};

class InequalityOperator: public BinaryOperatorExpression{
    public:
    void printASM(Bindings *bindings);
};

class UnaryOperatorExpression: public Expression{
    protected:
    Expression *expression;

    public:
    void printASM(Bindings *bindings);
    UnaryOperatorExpression(Expression *expression);
};

#endif