#include "nonterminal.h"

map<string, shared_ptr<NonTerminal>> NonTerminal::table_nonterminal = {};

void NonTerminal::Print(unsigned int flags) const
{
	printf("%s - NonTerminal\n", name.c_str()); 
	if (flags & PRINT_FLAGS::EMPTY)
		cout << "\tEmpty: " << empty << endl;
	if (flags & PRINT_FLAGS::FIRST)
		cout << "\tFirst: { " << first << "}" << endl;
	if (flags & PRINT_FLAGS::FOLLOW)
		cout << "\tFollow: { " << follow << "}" << endl;

	if (flags & PRINT_FLAGS::RULES)
	{
		cout << "\tRules:\n";
		for (shared_ptr<Rule> rule : rules)
		{
			printf("\t\t");
			rule->Print();
		}
	}
}

NonTerminal::NonTerminal(string name, bool start) : Symbol(name)
{
	if (start)
		follow.insert("$");
}

NonTerminal::~NonTerminal()
{
}
