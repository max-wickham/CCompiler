#ifndef TYPE_HPP
#define TYPE_HPP

#include "node.hpp"
#include "bindings.hpp"
#include <string>
#include <map>

class Type : public Node
{
protected:
	bool _signed;
	bool _extern;
	bool _static;
	bool _const;
public:
	virtual Type type();
	virtual int getSize();
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
};

class Int : public Type { 
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
	void extractFromRegister(Bindings *bindings, RegisterType type) override;
	std::string getRegister(RegisterType type) override;
};

class Bool : public Type {
protected:
	const int size = 4;
public:
	Bool();
	int getSize();
	std::string getName();
	void loadParameter(ReturnRegisters &returnRegisters, Bindings *bindings);
	void evaluateReturn(Bindings *bindings);
	void processReturn(Bindings *bindings);
	void placeInRegister(Bindings *bindings, RegisterType type);
	void extractFromRegister(Bindings *bindings, RegisterType type);
	std::string getRegister(RegisterType type);
};

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

#endif