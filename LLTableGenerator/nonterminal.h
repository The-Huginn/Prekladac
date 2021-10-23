#pragma once

#include<algorithm>

#include"symbol.h"
#include"rule.h"

/**
 * @brief Class for NonTerminal symbols
*/
class NonTerminal : public Symbol
{
private:
	/**
	 * @brief rules that contain this NonTerminal on the left side
	*/
	vector<shared_ptr<Rule>> rules = {};

	/**
	 * @brief Prints nonterminal
	 * @param flags flags for printing different components
	*/
	virtual void Print(unsigned int flags) const;

	/**
	 * @brief table containing only nonterminal symbols
	*/
	static map<string, shared_ptr<NonTerminal>> table_nonterminal;

	/**
	 * @brief Adds new nonterminal to table_nonterminal
	*/
	virtual inline void AddToTable() { table_nonterminal.insert({ name, shared_from_base<NonTerminal>() }); }
public:
	/**
	 * @brief base constructor
	 * @param name 
	 * @param start should be true only for starting nonterminal
	*/
	NonTerminal(string name, bool start = false);
	/**
	 * @brief base destructor
	*/
	virtual ~NonTerminal();

	/**
	 * @brief rules access function
	 * @return rules
	*/
	inline vector<shared_ptr<Rule>> GetRules() const { return rules; }
	/**
	 * @brief adds new rule
	 * @param rule
	*/
	inline void AddRule(shared_ptr<Rule> rule) { rules.push_back(rule); }
	/**
	 * @brief removes rule
	 * @param rule 
	*/
	inline void RemoveRule(shared_ptr<Rule> rule) { erase(rules, rule); }

	/**
	 * @brief Overload of pure virtual function
	 * @return false
	*/
	virtual inline bool IsTerminal() const { return false; }

	/**
	 * @brief table_nonterminal access function
	 * @return Table of nonterminals
	*/
	static inline map<string, shared_ptr<NonTerminal>> Table() { return table_nonterminal; }

	/**
	 * @brief Constructs pair element
	 * @param name
	 * @param empty should be true only starting nonterminal
	 * @return created pair
	*/
	static inline pair<string, shared_ptr<Symbol>> Pair(string name, bool start = false) { return { name, make_shared<NonTerminal>(name, start) }; }

	/**
	 * @brief friend function to allow access
	 * @param flags
	*/
	friend static void Symbol::PrintTable(unsigned int flags);
};