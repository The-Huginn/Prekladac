#include "symbol.h"

#include"nonterminal.h"
#include"terminal.h"

map<string, shared_ptr<Symbol>> Symbol::table = {};

ostream& operator<<(ostream& os, const set<string>& set)
{
	for (auto& data : set)
		os << data << " ";
	return os;
}

ostream& operator<<(ostream& os, const vector<string>& set)
{
	for (auto& data : set)
		os << data << " ";
	return os;
}

set<string> operator+(const set<string>& a, const set<string>& b)
{
	set<string> c(a);
	c.insert(b.begin(), b.end());
	return c;
}


Symbol::Symbol(string name) : name(name), first({}), follow({}), empty(false)
{
}

Symbol::~Symbol()
{
}

bool Symbol::AddFirst(set<string> add_first)
{
	int old_size = first.size();
	first = first + add_first;
	return old_size != first.size();
}

bool Symbol::AddFollow(set<string> add_follow)
{
	int old_size = follow.size();
	follow = follow + add_follow;
	return old_size != follow.size();
}

map<string, shared_ptr<Symbol>> Symbol::InitTable(map<string, shared_ptr<Symbol>> symbols)
{
	table = symbols;
	for (auto[id, symbol] : symbols)
		symbol->AddToTable();

	return symbols;
}

shared_ptr<Symbol> Symbol::AddSymbol(pair<string, shared_ptr<Symbol>> symbol)
{
	table.insert(symbol);
	symbol.second->AddToTable();
	return symbol.second;
}

void Symbol::PrintTable(unsigned int flags)
{
	if (flags & PRINT_FLAGS::SORTED)
	{
		for (auto[id, symbol] : Terminal::Table())
			symbol->Print(flags);

		for (auto[id, symbol] : NonTerminal::Table())
			symbol->Print(flags);

	}
	else
	{
		for (auto symbol : table)
			symbol.second->Print(flags);
	}
}

bool Symbol::Empty(vector<string> vec)
{
	for (string name : vec)
		if (!table[name]->IsEmpty())
			return false;
	return true;
}

set<string> Symbol::First(vector<string> vec)
{
	set<string> f = table[vec[0]]->GetFirst();

	for (int i = 0; i < vec.size() - 1; i++)
		if (table[vec[0]]->IsEmpty())
			f = f + table[vec[1]]->GetFirst();
		else
			break;

	return f;
}
