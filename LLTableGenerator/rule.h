#pragma once

#include<string>
#include<vector>
#include<memory>


#include"symbol.h"

using namespace std;

/**
 * @brief Contains rule data
*/
class Rule
{
private:
	/**
	 * @brief left side of the rule
	*/
	string left_side;
	/**
	 * @brief right side of the rule
	*/
	vector<string> right_side;

	/**
	 * @brief Predict set of the rule (LL table)
	*/
	set<string> predict;

	/**
	 * @brief id number of the rule
	*/
	unsigned int id;

	/**
	 * @brief vector of all rules
	*/
	static vector<shared_ptr<Rule>> rules;
public:
	/**
	 * @brief Base constructo
	 * @param left 
	 * @param right 
	*/
	Rule(string left, vector<string> right);
	/**
	 * @brief Base destructor 
	*/
	~Rule();

	/**
	 * @brief Left side access function
	 * @return left side
	*/
	inline string GetLeft() const { return left_side; }
	/**
	 * @brief Right side access function
	 * @param b starting index of symbol
	 * @param s number of requested symbols, -1 if all to the end 
	 * @return substring of symbols
	*/
	inline vector<string> GetRight(int b = 0, int s = -1) const { return { right_side.begin() + b, (s == -1) ? right_side.end() : right_side.begin() + b + s }; }

	/**
	 * @brief Id access function
	 * @return id of the rule
	*/
	inline unsigned int GetId() const { return id; }

	/**
	 * @brief Predict access function
	 * @return predict set
	*/
	inline set<string> GetPredict() const { return predict; }
	/**
	 * @brief Adds new elements to predict set
	 * @param add_predict elements to be added
	 * @return true if new element was added
	*/
	bool AddPredict(set<string> add_predict);

	/**
	 * @brief Inits vector of rules
	 * @param rules Initial rules
	 * @return Created vector
	*/
	static vector<shared_ptr<Rule>> InitVector(vector<shared_ptr<Rule>> rules);
	/**
	 * @brief Adds new rule to the vector
	 * @param rule Rule to be added
	 * @return Pointer to added item
	*/
	static shared_ptr<Rule> AddRule(shared_ptr<Rule> rule);
	/**
	 * @brief Removes rule from the vector
	 * @param rule rule to be removed
	*/
	static void RemoveRule(shared_ptr<Rule> rule);

	/**
	 * @brief Vector access function
	 * @return vector
	*/
	static inline vector<shared_ptr<Rule>> Vector() { return rules; }

	/**
	 * @brief Creates shared ptr
	 * @param left Left side
	 * @param right Right side
	 * @return Created ptr
	*/
	static inline shared_ptr<Rule> SharedPtr(string left, vector<string> right) { return make_shared<Rule>(left, right); }

	/**
	 * @brief prints rule 
	 * @param flag if true additionaly prints predict sets
	*/
	void Print(bool flag = false) const;

	/**
	 * @brief Prints all rules
	*/
	static void PrintRules();
};