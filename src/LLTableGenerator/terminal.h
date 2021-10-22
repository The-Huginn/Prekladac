#pragma once

#include"symbol.h"

class Terminal : public Symbol
{
private:

	virtual void Print(unsigned int flags) const;
public:
	Terminal(string name);
	virtual ~Terminal();

	virtual inline bool IsTerminal() const { return true; }

	static inline pair<string, shared_ptr<Symbol>> Pair(string name) { return { name, make_shared<Terminal>(name) }; }
};