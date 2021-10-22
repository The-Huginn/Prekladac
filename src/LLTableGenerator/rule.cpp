#include "rule.h"
#include "nonterminal.h"

Rule::Rule(string left, vector<string> right) : left_side(left), right_side(right)
{
	static unsigned int id = 1;
	this->id = id++;
}

Rule::~Rule()
{
}

vector<shared_ptr<Rule>> Rule::InitVector(vector<shared_ptr<Rule>> rules, map<string, shared_ptr<Symbol>> table)
{
	for (shared_ptr<Rule> rule : rules)
		dynamic_pointer_cast<NonTerminal>(table[rule->left_side])->AddRule(rule);
	return rules;
}

void Rule::Print() const
{
	printf("%d: %s -> ", id, left_side.c_str());
	for (string right : right_side)
		printf("%s ", right.c_str());
	printf("\n");
}
