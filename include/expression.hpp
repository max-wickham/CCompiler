#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include "node.hpp"
class Bindings;
#include "type.hpp"
class Parameter;


class Expression: public Node{
    public:
    virtual ~Expression(){};
    virtual void printASM(Bindings *bindings) = 0;
    virtual Type*  getType(Bindings *bindings) = 0;
};

class DotOperator: public Expression{
    protected:
    std::string structId;
    std::string elementId;
    public:
    DotOperator(std::string *structId, std::string *elementId);
    void printASM(Bindings *bindings);
    void printASMAssign(Bindings *bindings);
    Type* getType(Bindings* bindings) override;
};

class ArrowOperator: public Expression{
    protected:
    std::string pointerId;
    std::string elementId;
    public:
    ArrowOperator(std::string *pointerId, std::string *elementId);
    void printASM(Bindings *bindings);
    void printASMAssign(Bindings *bindings);
    Type* getType(Bindings* bindings);
};


class FunctionCall: public Expression{
    protected:
    std::string id;
    Parameter *parameter;
    public:
    FunctionCall(std::string *id,Parameter *parameter);
    void printASM(Bindings *bindings);
    Type*  getType(Bindings *bindings) override;
};

class Variable: public Expression{
    protected:
    std::string id;
    public:
    Variable(std::string *id);
    void printASM(Bindings *bindings) override;
    Type*  getType(Bindings *bindings) override;
    std::string getName();
};
//TODO
class StringConstant: public Expression{
    protected:
    std::string value;
    public:
    StringConstant(std::string *value);
    void printASM(Bindings *bindings);
    Type*  getType(Bindings *bindings);
};

class NumberConstant: public Expression{
    protected:
    int value;
    public:
    NumberConstant(int value);
    void printASM(Bindings *bindings) override;
    Type*  getType(Bindings *bindings) override;
};

class FloatConstant: public Expression{
    protected:
    float value;
    public:
    FloatConstant(float value);
    void printASM(Bindings *bindings) override;
    Type*  getType(Bindings *bindings) override;
};

class DoubleConstant: public Expression{
    protected:
    double value;
    public:
    DoubleConstant(double value);
    void printASM(Bindings *bindings) override;
    Type*  getType(Bindings *bindings) override;
};

class UnsignedConstant: public Expression{
    protected:
    unsigned int value;
    public:
    UnsignedConstant(unsigned int value);
    void printASM(Bindings *bindings) override;
    Type*  getType(Bindings *bindings) override;
};

class UnaryOperatorExpression: public Expression{
    protected:
    Expression *expression;

    public:
    UnaryOperatorExpression(Expression *expression);
};

class BinaryOperatorExpression: public Expression{
    protected:
    Expression *leftExpression;
    Expression *rightExpression;

    public:
    BinaryOperatorExpression(Expression *leftExpression, Expression *rightExpression);
    virtual Type* getType(Bindings *bindings);
};

class AssignmentOperator: public BinaryOperatorExpression{
    public:
    using BinaryOperatorExpression::BinaryOperatorExpression;
    void printASM(Bindings *bindings) override;
};

class AdditionOperator: public BinaryOperatorExpression{
    public:
    using BinaryOperatorExpression::BinaryOperatorExpression;
    void printASM(Bindings *bindings) override;
    //void printASM(Bindings *bindings, std::string returnRegister);
};

class SubtractionOperator: public BinaryOperatorExpression{
    public:
    using BinaryOperatorExpression::BinaryOperatorExpression;
    void printASM(Bindings *bindings) override;
};

class MultiplicationOperator: public BinaryOperatorExpression{
    public:
    using BinaryOperatorExpression::BinaryOperatorExpression;
    void printASM(Bindings *bindings); 
};

class DivisionOperator: public BinaryOperatorExpression{
    public:
    using BinaryOperatorExpression::BinaryOperatorExpression;
    void printASM(Bindings *bindings);
};

class EqualityOperator: public BinaryOperatorExpression{
    public:
    using BinaryOperatorExpression::BinaryOperatorExpression;
    void printASM(Bindings *bindings);
    Type* getType(Bindings *bindings) override;
};

class LessThanOperator: public BinaryOperatorExpression{
    public:
    using BinaryOperatorExpression::BinaryOperatorExpression;
    void printASM(Bindings *bindings);
    Type* getType(Bindings *bindings) override;
};

class LessThanEqualOperator: public BinaryOperatorExpression{
    public:
    using BinaryOperatorExpression::BinaryOperatorExpression;
    void printASM(Bindings *bindings);
    Type* getType(Bindings *bindings) override;
};

class ModuloOperator: public BinaryOperatorExpression{
    public:
    using BinaryOperatorExpression::BinaryOperatorExpression;
    void printASM(Bindings *bindings);
};

class LogicalAndOperator: public BinaryOperatorExpression{
    public:
    using BinaryOperatorExpression::BinaryOperatorExpression;
    void printASM(Bindings *bindings);
};

class BitwiseAndOperator: public BinaryOperatorExpression{
    public:
    using BinaryOperatorExpression::BinaryOperatorExpression;
    void printASM(Bindings *bindings);
};

class LogicalOrOperator: public BinaryOperatorExpression{
    public:
    using BinaryOperatorExpression::BinaryOperatorExpression;
    void printASM(Bindings *bindings);
};

class BitwiseOrOperator: public BinaryOperatorExpression{
    public:
    using BinaryOperatorExpression::BinaryOperatorExpression;
    void printASM(Bindings *bindings);
};

class BitwiseXorOperator: public BinaryOperatorExpression{
    public:
    using BinaryOperatorExpression::BinaryOperatorExpression;
    void printASM(Bindings *bindings);
};

class ShiftLeftOperator: public BinaryOperatorExpression{
    public:
    using BinaryOperatorExpression::BinaryOperatorExpression;
    void printASM(Bindings *bindings);
};

class ShiftRightOperator: public BinaryOperatorExpression{
    public:
    using BinaryOperatorExpression::BinaryOperatorExpression;
    void printASM(Bindings *bindings);
};

class IndexOperator: public BinaryOperatorExpression{
    public:
    using BinaryOperatorExpression::BinaryOperatorExpression;
    void printASMAssign(Bindings *bindings);
    void printASM(Bindings *bindings);
    Type* getType(Bindings *bindings) override;
};

class DefreferenceOperator: public UnaryOperatorExpression{
    public:
    using UnaryOperatorExpression::UnaryOperatorExpression;
    void printASM(Bindings *bindings);
    void printASMAssign(Bindings *bindings);
    Type*  getType(Bindings *bindings);
};

class AddressOperator: public UnaryOperatorExpression{
    public:
    using UnaryOperatorExpression::UnaryOperatorExpression;
    void printASM(Bindings *bindings);
    Type*  getType(Bindings *bindings);
};

class NotOperator: public UnaryOperatorExpression{
    public:
    using UnaryOperatorExpression::UnaryOperatorExpression;
    void printASM(Bindings *bindings);
    Type*  getType(Bindings *bindings);
};

class BitwiseNotOperator: public UnaryOperatorExpression{
    public:
    using UnaryOperatorExpression::UnaryOperatorExpression;
    void printASM(Bindings *bindings);
    Type*  getType(Bindings *bindings);
};

class SizeOfOperator: public Expression{
    Type *type;
    public:
    SizeOfOperator(Type *type);
    void printASM(Bindings *bindings);
    Type*  getType(Bindings *bindings);
};

class SizeOfExpressionOperator: public UnaryOperatorExpression{
    public:
    using UnaryOperatorExpression::UnaryOperatorExpression;
    void printASM(Bindings *bindings);
    Type*  getType(Bindings *bindings);
};

class TernaryOperator: public Expression{
    protected:
    Expression* expressionOne;
    Expression* expressionTwo;
    Expression* expressionThree;
    public:
    TernaryOperator(Expression* expressionOne, Expression* expressionTwo, Expression* expressionThree);
    void printASM(Bindings *bindings);
    Type*  getType(Bindings *bindings);
};

#endif