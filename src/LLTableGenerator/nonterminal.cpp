#include "nonterminal.h"

void NonTerminal::Print(unsigned int flags) const
{
	printf("%s - NonTerminal\n", name.c_str()); 
	if (flags && PRINT_FLAGS::FIRST)
		cout << "\tFirst: { " << first << "}" << endl;

	if (flags && PRINT_FLAGS::RULES)
	{
		cout << "\tRules:\n";
		for (shared_ptr<Rule> rule : rules)
		{
			printf("\t\t");
			rule->Print();
		}
	}
}

NonTerminal::NonTerminal(string name) : Symbol(name)
{
}

NonTerminal::~NonTerminal()
{
}
