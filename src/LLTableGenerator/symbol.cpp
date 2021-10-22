#include "symbol.h"

ostream& operator<<(ostream& os, const set<string>& set)
{
	for (auto& data : set)
		os << data << " ";
	return os;
}


Symbol::Symbol(string name) : name(name), first({})
{
}

Symbol::~Symbol()
{
}

bool Symbol::AddFirst(set<string> add_first)
{
	int old_size = first.size();
	first.insert(add_first.begin(), add_first.end());
	return old_size != first.size();
}

void Symbol::PrintTable(map<string, shared_ptr<Symbol>> table, unsigned int flags)
{
	if (flags && PRINT_FLAGS::SORTED)
	{
		for (auto symbol : table)
			if (symbol.second->IsTerminal())
				symbol.second->Print(flags);

		for (auto symbol : table)
			if (!symbol.second->IsTerminal())
				symbol.second->Print(flags);

	}
	else
	{
		for (auto symbol : table)
		{
			symbol.second->Print(flags);
		}
	}
}