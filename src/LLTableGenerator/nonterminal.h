#pragma once

#include"symbol.h"
#include"rule.h"

class NonTerminal : public Symbol
{
private:
	vector<shared_ptr<Rule>> rules = {};

	virtual void Print(unsigned int flags) const;
public:
	NonTerminal(string name);
	virtual ~NonTerminal();

	inline vector<shared_ptr<Rule>> GetRules() const { return rules; }
	inline void AddRule(shared_ptr<Rule> rule) { rules.push_back(rule); }

	virtual inline bool IsTerminal() const { return false; }

	static inline pair<string, shared_ptr<Symbol>> Pair(string name) { return { name, make_shared<NonTerminal>(name) }; }
};