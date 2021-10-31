#include "rule.h"
#include "nonterminal.h"

#include<algorithm>

vector<shared_ptr<Rule>> Rule::rules = {};

Rule::Rule(string left, vector<string> right) : left_side(left), right_side(right), predict({})
{
	static unsigned int id = 1;
	this->id = id++;
}

Rule::~Rule()
{
}

bool Rule::AddPredict(set<string> add_predict)
{
	int old_size = predict.size();
	predict = predict + add_predict;
	return old_size != predict.size();
}

vector<shared_ptr<Rule>> Rule::InitVector(vector<shared_ptr<Rule>> rules)
{
	Rule::rules = rules;
	auto nonterminals = NonTerminal::Table();
	for (shared_ptr<Rule> rule : rules)
		nonterminals[rule->left_side]->AddRule(rule);
	return rules;
}

shared_ptr<Rule> Rule::AddRule(shared_ptr<Rule> rule)
{
	rules.push_back(rule);
	NonTerminal::Table()[rule->left_side]->AddRule(rule);
	return rule;
}

void Rule::RemoveRule(shared_ptr<Rule> rule)
{
	NonTerminal::Table()[rule->left_side]->RemoveRule(rule);
	erase(rules, rule);
}

void Rule::Print(bool flag) const
{
	printf("%d: %s -> ", id, left_side.c_str());
	for (string right : right_side)
		printf("%s ", right.c_str());
	printf("\n");
	if (flag)
		cout << "\tPredict: { " << predict << "}" << endl;
}

void Rule::PrintRules()
{
	for (auto rule : rules)
	{
		rule->Print(true);
	}
}
