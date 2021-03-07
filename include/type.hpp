#ifndef TYPE_HPP
#define TYPE_HPP

#include "node.hpp"

class Type : public Node
{
protected:
	bool _signed;
	bool _extern;
	bool _static;
	bool _const;
public:
	virtual void print() const = 0;
    
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
};

#endif