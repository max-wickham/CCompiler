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
	int currentMemOffset = 0;
};

enum RegisterType{
	leftReg,
	rightReg,
	evaluateReg,
	returnReg
};

class Type : public Node
{
protected:
	bool _signed;
	bool _extern;
	bool _static;
	bool _const;
public:
	virtual int getSize() = 0;
	virtual std::string getName() = 0;
	virtual void loadParameter(ReturnRegisters &returnRegisters, Bindings *bindings) = 0;
	virtual void evaluateReturn(Bindings *bindings) = 0;
	virtual void processReturn(Bindings *bindings) = 0;
	// //places the value at the top of the stack into the register
	virtual void placeInRegister(Bindings *bindings, RegisterType type) = 0;
	// //places the value in the register at the top of the stack
	virtual void extractFromRegister(Bindings *bindings, RegisterType type) = 0;
	// //gets the register string from a register type
	virtual std::string getRegister(RegisterType type) = 0;
	virtual void saveVariable(Bindings *bindings, std::string id) = 0;
	virtual void placeVariableOnStack(Bindings *bindings, std::string id) = 0;
	virtual void placeVariableOnStack(Bindings *bindings) = 0;
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
	//TODO
	void extractFromModuloRegister(Bindings *bindings);
	//TODO
	std::string getAdditionOperator();
	//TODO
	std::string getSubtractionOperator();
	//TODO
	std::string getMultiplicationOperator();
	//TODO
	std::string getDivisionOperator();
	//TODO
	void extractFromMultRegister(Bindings *bindings);
	//TODO
	void extractFromDivRegister(Bindings *bindings);
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
};

 #endif