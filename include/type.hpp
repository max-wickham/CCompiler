#ifndef TYPE_HPP
#define TYPE_HPP

#include "node.hpp"
#include "bindings.hpp"

class Type : public Node
{
protected:
	bool _signed;
	bool _extern;
	bool _static;
	bool _const;
public:
	virtual std::string getId() const = 0;
	virtual void loadParameter(ReturnRegisters &returnRegisters, Bindings *bindings) const = 0;
	virtual void evaluateReturn(Bindings *bindings) const = 0;
    
	virtual Type type() = 0;
	virtual int getSize(int arraySize);
	virtual void setSigned(bool _signed);
	virtual void setExtern(bool _extern);
	virtual void setStatic(bool _static);
	virtual void setConst(bool _const);
};

class Int : public Type {
public:
	Int();
	int getSize(int arraySize);
	void loadParameter(ReturnRegisters &returnRegisters, Bindings *bindings);
	void evaluateReturn(Bindings *bindings);
};

struct ReturnRegisters{
	bool a0 = false;
	bool a1 = false;
	bool a2 = false;
	bool a3 = false;
	int currentMemOffset = 0;
};

#endif