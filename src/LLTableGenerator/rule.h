#pragma once

#include<string>
#include<vector>
#include<memory>

#include"symbol.h"

using namespace std;

class Rule
{
private:
	string left_side;
	vector<string> right_side;

	unsigned int id;
public:
	Rule(string left, vector<string> right);
	~Rule();

	inline string GetLeft() const { return left_side; }
	inline vector<string> GetRight() const { return right_side; }

	inline unsigned int GetId() const { return id; }

	static vector<shared_ptr<Rule>> InitVector(vector<shared_ptr<Rule>> rules, map<string, shared_ptr<Symbol>> table);

	static inline shared_ptr<Rule> SharedPtr(string left, vector<string> right) { return make_shared<Rule>(left, right); }

	void Print() const;
};