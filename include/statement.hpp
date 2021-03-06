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

class BreakStatement: public Statement {
    protected:
    Statement *nextStatement;
    public:
    BreakStatement(Statement *nextStatement);
    void printASM(Bindings *bindings);
};

class ContinueStatement: public Statement {
    protected:
    Statement *nextStatement;
    public:
    ContinueStatement(Statement *nextStatement);
    void printASM(Bindings *bindings);
};

class ScopedStatement: public Statement{
    protected:
    Statement *nextStatement;
    Statement *statement;
    public:
    ScopedStatement(Statement *statement, Statement *nextStatement);
    void printASM(Bindings* bindings);
};

class VariableDefinition: public Statement{
    protected:
    Decleration *decleration;
    public:
    VariableDefinition(Decleration *decleration, Statement *nextStatement);
    void printASM(Bindings* bindings) override;
};

class StructDecleration: public Statement{
    protected:
    std::string structId;
    std::string id;
    public:
    StructDecleration(std::string *structId, std::string *id, Statement *nextStatement);
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
    Decleration *definitionDec;
    Expression *definitionExp;
    Expression *condition;
    Expression *incrementer;
    Statement *statement;

    public:
    ForLoopStatement(Decleration *definition, Expression *condition, Expression *incrementer,
        Statement *statement, Statement *nextStatement);
    ForLoopStatement(Expression *definition, Expression *condition, Expression *incrementer,
        Statement *statement, Statement *nextStatement);
    ForLoopStatement(Expression *condition, Expression *incrementer,
        Statement *statement, Statement *nextStatement);
    void printASM(Bindings* bindings) override;

};

class WhileLoopStatement: public Statement{
    protected:
    Expression *condition;
    Statement *statement;
    public:
    WhileLoopStatement(Expression *condition, Statement *statement, Statement *nextStatement);
    void printASM(Bindings* bindings) override;
};
//TODO
class DoWhileLoopStatement: public Statement{
    protected:
    Expression *condition;
    Statement *statement;
    public:
    DoWhileLoopStatement(Expression *condition, Statement *statement, Statement *nextStatement);
    void printASM(Bindings* bindings) override;
};

class SwitchStatement: public Statement{
    protected:
    Expression *expression;
    Statement *caseStatement;
    public:
    SwitchStatement(Expression *expression, Statement *caseStatement, Statement *nextStatement);
    void printASM(Bindings* bindings) override;
};

class CaseStatement: public Statement{
    protected:
    Statement *statement;
    Expression *constant;
    public:
    CaseStatement(Expression *constant, Statement *statement, Statement *nextCaseStatement);
    virtual void printASM(Bindings* bindings) override;
};

class ExpressionStatement: public Statement{
    protected:
    Expression *expression;
    public:
    ExpressionStatement(Expression *expression, Statement *nextStatement);
    void printASM(Bindings* bindings) override;
};


#endif