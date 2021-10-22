#pragma once

#include<string>
#include<map>
#include<memory>
#include<vector>
#include<set>
#include<iostream>

using namespace std;

ostream& operator<<(ostream& os, const set<string>& set);


class Symbol
{
protected:
	string name;
	set<string> first;

	virtual void Print(unsigned int flags) const = 0;
public:
	Symbol(string name);
	virtual ~Symbol();

	inline string GetName() const { return name; }

	inline set<string> GetFirst() const { return first; }
	bool AddFirst(set<string> add_first);

	virtual inline bool IsTerminal() const = 0;

	static inline map<string, shared_ptr<Symbol>> InitTable(map<string, shared_ptr<Symbol>> symbols) { return symbols; }


	static void PrintTable(map<string, shared_ptr<Symbol>> table, unsigned int flags);


	enum PRINT_FLAGS
	{
		SORTED = 1 << 0, RULES = 1 << 1, FIRST = 1 << 2
	};
};

