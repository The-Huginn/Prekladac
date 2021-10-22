#include<iostream>
#include<memory>
#include<vector>
#include<map>

#include"nonterminal.h"
#include"terminal.h"
#include"rule.h"

using namespace std;

void CalculateFirst(vector<shared_ptr<Rule>> rules, map<string, shared_ptr<Symbol>> symbols)
{
	bool change = true;
	while (change)
	{
		change = false;

		for (shared_ptr<Rule> rule : rules)
		{
			if (symbols[rule->GetLeft()]->AddFirst(symbols[rule->GetRight()[0]]->GetFirst()))
				change = true;
		}
	}
}

void PrintLLTable(vector<shared_ptr<Rule>> rules, map<string, shared_ptr<Symbol>> symbols)
{
	printf("%*s|", 10, "");
	for (auto symbol : symbols)
		if (symbol.second->IsTerminal())
			printf("%*s%*s|", int(5 + symbol.second->GetName().size() / 2), symbol.second->GetName().c_str(), int(5 - symbol.second->GetName().size() / 2), "");
	printf("\n");

	for (auto symbol : symbols)
		if (!symbol.second->IsTerminal())
		{
			printf("%*s%*s|", int(5 + symbol.second->GetName().size() / 2), symbol.second->GetName().c_str(), int(5 - symbol.second->GetName().size() / 2), "");
			for (auto terminal : symbols)
			{
				if (terminal.second->IsTerminal())
				{
					string rule_numbers = "";
					for (auto rule : dynamic_pointer_cast<NonTerminal>(symbol.second)->GetRules())
					{
						auto first = symbols[rule->GetRight()[0]]->GetFirst();
						if (first.find(terminal.second->GetName()) != first.end())
							rule_numbers += to_string(rule->GetId()) + " ";
					}
					if (!rule_numbers.empty())
						rule_numbers.pop_back();
					printf("%*s%*s|", int(5 + rule_numbers.size() / 2), rule_numbers.c_str(), int(5 - rule_numbers.size() / 2), "");
				}
			}
			printf("\n");
		}
}


int main()
{
	
	map<string, shared_ptr<Symbol>> symbols = Symbol::InitTable({
		NonTerminal::Pair("<prog>"),
		Terminal::Pair("begin"),
		NonTerminal::Pair("<st-list>"),
		NonTerminal::Pair("<stat>"),
		Terminal::Pair(";"),
		Terminal::Pair("end"),
		Terminal::Pair("read"),
		Terminal::Pair("id"),
		Terminal::Pair("write"),
		NonTerminal::Pair("<item>"),
		Terminal::Pair(":="),
		Terminal::Pair("add"),
		Terminal::Pair("("),
		NonTerminal::Pair("<it-list>"),
		Terminal::Pair(","),
		Terminal::Pair(")"),
		Terminal::Pair("int")
	});

	vector<shared_ptr<Rule>> rules = Rule::InitVector({ 
		Rule::SharedPtr("<prog>", {"begin", "<st-list>"}), 
		Rule::SharedPtr("<st-list>", {"<stat>", ";", "<st-list>"}), 
		Rule::SharedPtr("<st-list>", {"end"}), 
		Rule::SharedPtr("<stat>", {"read", "id"}), 
		Rule::SharedPtr("<stat>", {"write", "<item>"}), 
		Rule::SharedPtr("<stat>", {"id", ":=", "add", "(", "<item>", "<it-list>"}), 
		Rule::SharedPtr("<it-list>", {",", "<item>", "<it-list>"}), 
		Rule::SharedPtr("<it-list>", {")"}), 
		Rule::SharedPtr("<item>", {"int"}), 
		Rule::SharedPtr("<item>", {"id"}) 
	}, symbols);

	CalculateFirst(rules, symbols);

	Symbol::PrintTable(symbols, Symbol::PRINT_FLAGS::SORTED || Symbol::PRINT_FLAGS::FIRST || Symbol::PRINT_FLAGS::RULES);

	PrintLLTable(rules, symbols);

	return 0;
}