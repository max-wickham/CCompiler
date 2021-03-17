#ifndef TYPE_HPP
#define TYPE_HPP

#include "node.hpp"
#include "bindings.hpp"
#include <string>
// #include <map>

struct ReturnRegisters{
	bool a0 = false;
	bool a1 = false;
	bool a2 = false;
	bool a3 = false;
	bool f12 = false;
	bool f13 = false;
	bool f14 = false;
	bool f15 = false;
	int currentMemOffset = 0;
};

enum RegisterType{
	leftReg,
	rightReg,
	evaluateReg,
	returnReg,
	zeroReg
};

class Type : public Node
{
protected:
	bool _signed = true;
public:
	bool isSigned();

	void setUnSigned();

	virtual int getSize() = 0;

	virtual std::string getName() = 0;

	virtual void loadParameter(ReturnRegisters &returnRegisters, Bindings *bindings) = 0;

	virtual void evaluateReturn(Bindings *bindings) = 0;

	virtual void processReturn(Bindings *bindings) = 0;

	//places the value at the top of the stack into the register
	virtual void placeInRegister(Bindings *bindings, RegisterType type) = 0;

	//places the value in the register at the top of the stack
	virtual void extractFromRegister(Bindings *bindings, RegisterType type) = 0;

	//gets the register string from a register type
	virtual std::string getRegister(RegisterType type) = 0;

	virtual void saveVariable(Bindings *bindings, std::string id) = 0;

	virtual void placeVariableOnStack(Bindings *bindings, std::string id) = 0;

	virtual void placeVariableOnStack(Bindings *bindings) = 0;

	virtual void beq(Bindings *bindings, RegisterType reg1, RegisterType reg2, std::string label) = 0;
};

class Void : public Type {
	public:
	Void();

	int getSize() override;

	std::string getName() override;

	void loadParameter(ReturnRegisters &returnRegisters, Bindings *bindings) override{}

	void evaluateReturn(Bindings *bindings) override{}

	void processReturn(Bindings *bindings) override{}

	//places the value at the top of the stack into the register
	void placeInRegister(Bindings *bindings, RegisterType type) override{}

	//places the value in the register at the top of the stack
	void extractFromRegister(Bindings *bindings, RegisterType type) override{}

	//gets the register string from a register type
	std::string getRegister(RegisterType type) override;

	void saveVariable(Bindings *bindings, std::string id) override{}

	void placeVariableOnStack(Bindings *bindings, std::string id) override{}

	void placeVariableOnStack(Bindings *bindings) override{}

	void beq(Bindings *bindings, RegisterType reg1, RegisterType reg2, std::string label) override{}
};

class OperandType : public Type {
public:
	//TODO
	virtual std::string getAdditionOperator() = 0;
	//TODO
	virtual std::string getSubtractionOperator() = 0;
	//TODO
	virtual std::string getMultiplicationOperator() = 0;
	//TODO
	virtual std::string getDivisionOperator() = 0;
	//TODO
	virtual void extractFromMultRegister(Bindings *bindings) = 0;
	//TODO
	virtual void extractFromDivRegister(Bindings *bindings) = 0;
};

class IntegralType : public OperandType {
public:
	virtual void extractFromRegister(Bindings *bindings, RegisterType type) = 0;

	virtual void saveVariable(Bindings *bindings, std::string id) = 0;

	virtual void placeVariableOnStack(Bindings *bindings, std::string id) = 0;

	std::string getAdditionOperator();

	std::string getSubtractionOperator();

	std::string getMultiplicationOperator();

	std::string getDivisionOperator();

	void extractFromMultRegister(Bindings *bindings);

	void extractFromDivRegister(Bindings *bindings);

	void extractFromModuloRegister(Bindings *bindings);
	//TODO
	void beq(Bindings *bindings, RegisterType reg1, RegisterType reg2, std::string label) override;
};

class Char : public IntegralType {
protected:
	const int size = 4;
public:

	Char();

	int getSize() override;

	std::string getName() override;

	void loadParameter(ReturnRegisters &returnRegisters, Bindings *bindings) override;

	void evaluateReturn(Bindings *bindings) override;

	void processReturn(Bindings *bindings) override;

	void placeInRegister(Bindings *bindings, RegisterType type) override;

	void extractFromRegister(Bindings *bindings, RegisterType type) override;

	void saveVariable(Bindings *bindings, std::string id) override;

	void placeVariableOnStack(Bindings *bindings, std::string id) override;

	void placeVariableOnStack(Bindings *bindings) override;

	std::string getRegister(RegisterType type) override;
};

class Int : public IntegralType { 
protected:
	const int size = 4;
public:
	Int();

	int getSize() override;

	std::string getName() override;

	void loadParameter(ReturnRegisters &returnRegisters, Bindings *bindings) override;

	void evaluateReturn(Bindings *bindings) override;

	void processReturn(Bindings *bindings) override;

	void placeInRegister(Bindings *bindings, RegisterType type) override;

	void extractFromRegister(Bindings *bindings, RegisterType type);

	std::string getRegister(RegisterType type) override;
	
	void saveVariable(Bindings *bindings, std::string id);

	void placeVariableOnStack(Bindings *bindings, std::string id);

	void placeVariableOnStack(Bindings *bindings);
};

class Pointer : public Int {
protected:
	Type *type;
	std::string id;
public:
	Pointer(Type *type);

	void defreference(Bindings *bindings);

	Type* getType();
};

class Float : public OperandType {
protected:
	const int size = 4;
public:
	Float();

	int getSize() override;

	std::string getName() override;

	void loadParameter(ReturnRegisters &returnRegisters, Bindings *bindings) override;

	void evaluateReturn(Bindings *bindings) override;

	void processReturn(Bindings *bindings) override;

	void placeInRegister(Bindings *bindings, RegisterType type) override;

	void extractFromRegister(Bindings *bindings, RegisterType type) override;

	std::string getRegister(RegisterType type) override;

	void saveVariable(Bindings *bindings, std::string id) override;

	void placeVariableOnStack(Bindings *bindings, std::string id) override;

	void placeVariableOnStack(Bindings *bindings) override;

	std::string getAdditionOperator() override;

	std::string getSubtractionOperator() override;

	std::string getMultiplicationOperator() override;

	std::string getDivisionOperator() override;

	void extractFromMultRegister(Bindings *bindings) override;

	void extractFromDivRegister(Bindings *bindings) override;
	//TODO
	void beq(Bindings *bindings, RegisterType reg1, RegisterType reg2, std::string label);
};

 #endif