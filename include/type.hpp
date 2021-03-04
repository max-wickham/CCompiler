#ifndef TYPE_HPP
#define TYPE_HPP

#include "node.hpp"

class Type : public Node
{
public:
	virtual void print() const = 0;
    
	virtual Type type() = 0;
	virtual void setSigned(bool _signed);
	virtual void setExtern(bool _extern);
	virtual void setStatic(bool _static);
	virtual void setConst(bool _const);
};

#endif