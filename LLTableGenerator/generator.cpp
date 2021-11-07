#include<iostream>
#include<memory>
#include<vector>
#include<map>
#include<algorithm>
#include<ranges>

#include"nonterminal.h"
#include"terminal.h"
#include"rule.h"
#include"fileoutput.h"

using namespace std;


vector<string> operator+(const vector<string>& a, const vector<string>& b)
{
	vector<string> c = a;
	c.insert(c.end(), b.begin(), b.end());
	return c;
}

/**
 * @brief Compares rules based on the first element on the right
*/
struct RuleComp
{
	bool operator()(const shared_ptr<Rule>& a, const shared_ptr<Rule>& b)
	{
		return a->GetRight()[0] < b->GetRight()[0];
	}
};

/**
 * @brief Splits vector of rules into categories with same first symbols on the right
 * @param rules rules to be splitted
 * @return categoriezed rules
*/
vector<vector<shared_ptr<Rule>>> SplitRulesByFirst(vector<shared_ptr<Rule>>& rules)
{
	sort(rules.begin(), rules.end(), RuleComp());
	vector<vector<shared_ptr<Rule>>> categeries = { {rules[0]} };
	for (int i = 1; i < rules.size(); i++)
	{
		if (categeries.back()[0]->GetRight()[0] == rules[i]->GetRight()[0])
			categeries.back().push_back(rules[i]);
		else
			categeries.push_back({ rules[i] });
	}
	return categeries;
}

/**
 * @brief Removes nondeterminism by creating new nonterminal covering different sufixes
*/
void Factorization()
{
	auto nonterminals = NonTerminal::Table();
	vector<shared_ptr<NonTerminal>> to_check;
	for (auto it = nonterminals.begin(); it != nonterminals.end(); ++it) 
	{
		to_check.push_back(it->second);
	}

	while (!to_check.empty())
	{
		auto rules = to_check[0]->GetRules();
		if (rules.size() == 0)
			cout << to_check[0]->GetName() << endl;
		auto categories = SplitRulesByFirst(rules);

		for (auto category : categories)
		{
			if (category.size() > 1)
			{
				string nonterminal_name = category[0]->GetLeft() + "_";
				shared_ptr<NonTerminal> new_nonterminal = dynamic_pointer_cast<NonTerminal>(Symbol::AddSymbol(NonTerminal::Pair(nonterminal_name)));
				Rule::AddRule(Rule::SharedPtr(category[0]->GetLeft(), { category[0]->GetRight(0,1)[0], nonterminal_name }));
				for (auto rule : category)
				{
					auto test = rule->GetRight(1, -1);
					Rule::AddRule(Rule::SharedPtr(nonterminal_name, (rule->GetRight().size() > 1) ? rule->GetRight(1, -1) : vector<string>{ "epsilon" })); //  k_not sure
					Rule::RemoveRule(rule);
				}
				to_check.push_back(new_nonterminal);
			}
		}

		to_check.erase(to_check.begin());
	}
}

/**
 * @brief Removes left recursion from rules by creating right recursion with new nonterminal k_and epsilon transition
*/
void RemoveLeftRecursion()
{
	auto nonterminals = NonTerminal::Table();

	for (auto [id, nonterminal] : nonterminals)
	{
		auto rules = nonterminal->GetRules();

		for (auto rule : rules)
		{
			if (nonterminal->GetName() == rule->GetRight(0, 1)[0])
			{
				string nonterminal_name = rule->GetLeft() + "_";
				Symbol::AddSymbol(NonTerminal::Pair(nonterminal_name));

				for (auto non_rec_rule : rules)
				{
					if (nonterminal->GetName() != non_rec_rule->GetRight(0, 1)[0] /*&& nonterminal->GetName() != non_rec_rule->GetRight().back()*/)
					{
						Rule::AddRule(Rule::SharedPtr(nonterminal->GetName(), non_rec_rule->GetRight() + vector<string>{ nonterminal_name }));
						Rule::RemoveRule(non_rec_rule);
					}
					
				}

				Rule::AddRule(Rule::SharedPtr(nonterminal_name, rule->GetRight(1, -1) + vector<string>{ nonterminal_name }));
				Rule::AddRule(Rule::SharedPtr(nonterminal_name, { "epsilon" }));
				Rule::RemoveRule(rule);
			}
		}

	}
}

/**
 * @brief Calculates empty values for all symbols
*/
void CalculateEmpty()
{
	bool change = true;
	auto rules = Rule::Vector();
	auto symbols = Symbol::Table();
	while (change)
	{
		change = false;

		for (shared_ptr<Rule> rule : rules)
		{
			if (symbols[rule->GetLeft()] == NULL)
				cout << "Missing nonterminal: " << rule->GetLeft() << endl;
			bool empty = Symbol::Empty(rule->GetRight());

			if (empty && !symbols[rule->GetLeft()]->IsEmpty())
			{
				symbols[rule->GetLeft()]->SetEmpty(true);
				change = true;
			}
		}
	}
}

/**
 * @brief Calculates first sets for all symbols
*/
void CalculateFirst()
{
	bool change = true;
	auto rules = Rule::Vector();
	auto symbols = Symbol::Table();
	while (change)
	{
		change = false;

		for (shared_ptr<Rule> rule : rules)
		{
			set<string> first = Symbol::First(rule->GetRight());
			if (symbols[rule->GetLeft()]->AddFirst(first))
				change = true;
		}
	}
}

/**
 * @brief Calculates follow sets for all symbols
*/
void CalculateFollow()
{
	bool change = true;
	auto rules = Rule::Vector();
	auto symbols = Symbol::Table();
	while (change)
	{
		change = false;

		for (shared_ptr<Rule> rule : rules)
		{
			auto right = rule->GetRight();
			for (int i = 0; i < right.size() - 1; i++)
			{
				set<string> follow = Symbol::First({ right.begin() + i + 1, right.end() });
				if (symbols[right[i]]->AddFollow(follow))
					change = true;

				if (Symbol::Empty({ right.begin() + i + 1, right.end() }))
					if (symbols[right[i]]->AddFollow(symbols[rule->GetLeft()]->GetFollow()))
						change = true;
			}

			if (symbols[right.back()]->AddFollow(symbols[rule->GetLeft()]->GetFollow()))
				change = true;
		}
	}
}

/**
 * @brief Calculates predict sets for all rules
*/
void CalculatePredict()
{
	auto rules = Rule::Vector();
	auto symbols = Symbol::Table();
	
	for (shared_ptr<Rule> rule : rules)
	{
		auto right = rule->GetRight();

		rule->AddPredict(Symbol::First(right));

		if (Symbol::Empty(right))
			rule->AddPredict(symbols[rule->GetLeft()]->GetFollow());
	}

}

/**
 * @brief Prints created LL table
*/
void PrintLLTable()
{
	auto rules = Rule::Vector();
	auto symbols = Symbol::Table();
	auto terminals = Terminal::Table();
	auto nonterminals = NonTerminal::Table();

	unsigned int cell_size = 26;
	unsigned int h_cell_size = cell_size / 2;

	printf("%s\n", string((cell_size + 1) * (terminals.size() + 1) + 1, '-').c_str());

	printf("|%*s|", cell_size, "");
	for (auto[id, terminal] : terminals)
		printf("%*s%*s|", int(h_cell_size + terminal->GetName().size() / 2), terminal->GetName().c_str(), int(h_cell_size - terminal->GetName().size() / 2), "");
	printf("\n%s\n", string((cell_size + 1) * (terminals.size() + 1) + 1, '-').c_str());

	for (auto[non_id, nonterminal] : nonterminals)
	{
		printf("|%*s%*s|", int(h_cell_size + nonterminal->GetName().size() / 2), nonterminal->GetName().c_str(), int(h_cell_size - nonterminal->GetName().size() / 2), "");

		for (auto[term_id, terminal] : terminals)
		{
			string rule_numbers = "";
			for (auto rule : nonterminal->GetRules())
			{
				auto predict = rule->GetPredict();
			//	 auto first = symbols[rule->GetRight()[0]]->GetFirst();
				if (predict.find(terminal->GetName()) != predict.end())
					rule_numbers += to_string(rule->GetId()) + " ";
			}
			if (!rule_numbers.empty())
				rule_numbers.pop_back();
			printf("%*s%*s|", int(h_cell_size + rule_numbers.size() / 2), rule_numbers.c_str(), int(h_cell_size - rule_numbers.size() / 2), "");
		}
		printf("\n%s\n", string((cell_size + 1) * (terminals.size() + 1) + 1, '-').c_str());
	}
}


int main()
{
	/*
	Symbol::InitTable({
		NonTerminal::Pair("prog", true),
		Terminal::Pair("begin"),
		NonTerminal::Pair("st-list"),
		NonTerminal::Pair("stat"),
		Terminal::Pair(";"),
		Terminal::Pair("k_end"),
		Terminal::Pair("read"),
		Terminal::Pair("id"),
		Terminal::Pair("write"),
		NonTerminal::Pair("item"),
		Terminal::Pair(":="),
		Terminal::Pair("add"),
		Terminal::Pair("("),
		NonTerminal::Pair("it-list"),
		Terminal::Pair(","),
		Terminal::Pair(")"),
		Terminal::Pair("int"),
		Terminal::Pair("epsilon", true),
		Terminal::Pair("$")
	});
	/
	Rule::InitVector({
		Rule::SharedPtr("prog", {"begin", "st-list"}),
		Rule::SharedPtr("st-list", {"stat", ";", "st-list"}),
		Rule::SharedPtr("st-list", {"k_end"}),
		Rule::SharedPtr("stat", {"read", "id"}),
		Rule::SharedPtr("stat", {"write", "item"}),
		Rule::SharedPtr("stat", {"id", ":=", "add", "(", "item", "it-list"}),
		Rule::SharedPtr("it-list", {",", "item", "it-list"}),
		Rule::SharedPtr("it-list", {")"}),
		Rule::SharedPtr("item", {"int"}),
		Rule::SharedPtr("item", {"id"}),
		Rule::SharedPtr("item", { "item", ")" }),   random
		Rule::SharedPtr("it-list", {",", "add"}),   random
		Rule::SharedPtr("it-list", {",", "item", "id"}),   random

	});*/
	
	/*
	Symbol::InitTable({
		NonTerminal::Pair("E", true),
		NonTerminal::Pair("T"),
		NonTerminal::Pair("F"),
		Terminal::Pair("+"),
		Terminal::Pair("*"),
		Terminal::Pair("("),
		Terminal::Pair(")"),
		Terminal::Pair("i"),
		Terminal::Pair("epsilon", true),
		Terminal::Pair("$")
	});

	Rule::InitVector({
		Rule::SharedPtr("E", {"E", "+", "T"}),
		Rule::SharedPtr("E", {"T"}),
		Rule::SharedPtr("T", {"T", "*", "F"}),
		Rule::SharedPtr("T", {"F"}),
		Rule::SharedPtr("F", {"(", "E", ")"}),
		Rule::SharedPtr("F", {"i"})
		});*/
	
	Symbol::InitTable({
		NonTerminal::Pair("nt_program", true),
		NonTerminal::Pair("nt_global_scope"),
		NonTerminal::Pair("nt_function_declare"),
		NonTerminal::Pair("nt_function_define"),
		NonTerminal::Pair("nt_function_call"),
		NonTerminal::Pair("nt_called_parameters"),

		NonTerminal::Pair("nt_parameters"),
		NonTerminal::Pair("nt_parameter"),
		NonTerminal::Pair("nt_parameter_name"),
		NonTerminal::Pair("nt_parameters_defined"),
		NonTerminal::Pair("nt_parameter_defined"),

		NonTerminal::Pair("nt_returning"),

		NonTerminal::Pair("nt_scope_statements"),
		NonTerminal::Pair("nt_statements"),
		NonTerminal::Pair("nt_statement"),
		NonTerminal::Pair("nt_scope"),
		NonTerminal::Pair("nt_declare"),
		NonTerminal::Pair("nt_id"),
		NonTerminal::Pair("nt_if"),
		NonTerminal::Pair("nt_while"),
		NonTerminal::Pair("nt_scope_return"),
		NonTerminal::Pair("nt_return"),
		NonTerminal::Pair("nt_declare_assign"),
		NonTerminal::Pair("nt_assign"),

		NonTerminal::Pair("nt_condition"),
		NonTerminal::Pair("nt_condition_branch"),
		
		NonTerminal::Pair("nt_lvalues"),
		NonTerminal::Pair("nt_lvalue"),
		NonTerminal::Pair("nt_rvalues"),
		NonTerminal::Pair("nt_rvalue"),

		NonTerminal::Pair("nt_expression"),
		NonTerminal::Pair("nt_expression_2"),
		NonTerminal::Pair("nt_expression_3"),

		NonTerminal::Pair("nt_datatypes"),
		NonTerminal::Pair("nt_datatype"),

		NonTerminal::Pair("nt_unary_operator"),
		NonTerminal::Pair("nt_binary_operator"),


		Terminal::Pair("k_global"),
		Terminal::Pair("k_function"),
		Terminal::Pair("t_id"),
		Terminal::Pair("t_integer"),
		Terminal::Pair("t_number"),
		Terminal::Pair("t_string"),
		Terminal::Pair("t_minus"),
		Terminal::Pair("t_plus"),
		Terminal::Pair("t_mul"),
		Terminal::Pair("t_div"),
		Terminal::Pair("t_int_div"),
		Terminal::Pair("t_def"),
		Terminal::Pair("t_comma"),
		Terminal::Pair("t_len"),
		Terminal::Pair("t_left"),
		Terminal::Pair("t_right"),
		Terminal::Pair("t_less"),
		Terminal::Pair("t_leq"),
		Terminal::Pair("t_grt"),
		Terminal::Pair("t_geq"),
		Terminal::Pair("t_concat"),
		Terminal::Pair("t_ass"),
		Terminal::Pair("t_eq"),
		Terminal::Pair("t_neq"),
		Terminal::Pair("k_integer"),
		Terminal::Pair("k_number"),
		Terminal::Pair("k_string"),
		Terminal::Pair("k_nil"),
		Terminal::Pair("k_local"),
		Terminal::Pair("k_boolean"),
		Terminal::Pair("k_require"),
		Terminal::Pair("k_true"),
		Terminal::Pair("k_false"),
		Terminal::Pair("k_and"),
		Terminal::Pair("k_or"),
		Terminal::Pair("k_not"),

		Terminal::Pair("k_if"),
		Terminal::Pair("k_then"),
		Terminal::Pair("k_else"),
		Terminal::Pair("k_elseif"),
		Terminal::Pair("k_end"),

		Terminal::Pair("k_return"),
		Terminal::Pair("k_while"),
		Terminal::Pair("k_do"),

		Terminal::Pair("epsilon", true),
		Terminal::Pair("$")
	});


	Rule::InitVector({
		Rule::SharedPtr("nt_declare", {"k_local", "nt_lvalues", "t_def", "nt_datatypes", "nt_declare_assign"}),
		Rule::SharedPtr("nt_declare_assign", {"nt_assign"}),
		Rule::SharedPtr("nt_declare_assign", {"epsilon"}),
		Rule::SharedPtr("nt_lvalues", {"nt_lvalues", "t_comma", "nt_lvalue"}),
		Rule::SharedPtr("nt_lvalues", {"nt_lvalue"}),
		Rule::SharedPtr("nt_lvalue", {"t_id"}),
		Rule::SharedPtr("nt_rvalues", {"nt_rvalues", "t_comma", "nt_rvalue"}),
		Rule::SharedPtr("nt_rvalues", {"nt_rvalue"}),
		Rule::SharedPtr("nt_rvalue", {"nt_expression"}),

		Rule::SharedPtr("nt_id", {"t_id", "t_left", "nt_called_parameters", "t_right"}),
		Rule::SharedPtr("nt_id", {"t_id", "nt_assign" }),
		Rule::SharedPtr("nt_id", {"t_id", "t_comma", "nt_lvalues", "nt_assign"}),
		Rule::SharedPtr("nt_assign", {"t_ass", "nt_rvalues"}),

		Rule::SharedPtr("nt_expression", {"nt_expression", "nt_binary_operator", "nt_expression_2"}),
		Rule::SharedPtr("nt_expression", {"nt_expression_2"}),
		Rule::SharedPtr("nt_expression_2", {"nt_unary_operator", "nt_expression_3"}),
		Rule::SharedPtr("nt_expression_2", {"nt_expression_3"}),
		Rule::SharedPtr("nt_expression_3", {"t_left", "nt_expression", "t_right" }),
		Rule::SharedPtr("nt_expression_3", {"t_id", "t_left", "nt_called_parameters", "t_right"}),
		Rule::SharedPtr("nt_expression_3", {"t_id"}),
		Rule::SharedPtr("nt_expression_3", {"t_integer"}),
		Rule::SharedPtr("nt_expression_3", {"t_number"}),
		Rule::SharedPtr("nt_expression_3", {"t_string"}),
		Rule::SharedPtr("nt_expression_3", {"k_nil"}),
		Rule::SharedPtr("nt_expression_3", {"k_true"}),
		Rule::SharedPtr("nt_expression_3", {"k_false"}),

		Rule::SharedPtr("nt_unary_operator", {"t_len"}),
		Rule::SharedPtr("nt_unary_operator", {"k_not"}),
		Rule::SharedPtr("nt_binary_operator", {"t_plus"}),
		Rule::SharedPtr("nt_binary_operator", {"t_minus"}),
		Rule::SharedPtr("nt_binary_operator", {"t_mul"}),
		Rule::SharedPtr("nt_binary_operator", {"t_div"}),
		Rule::SharedPtr("nt_binary_operator", {"t_int_div"}),
		Rule::SharedPtr("nt_binary_operator", {"t_concat"}),

		Rule::SharedPtr("nt_datatype", {"k_integer"}),
		Rule::SharedPtr("nt_datatype", {"k_number"}),
		Rule::SharedPtr("nt_datatype", {"k_string"}),
		Rule::SharedPtr("nt_datatype", {"k_boolean"}),

		Rule::SharedPtr("nt_binary_operator", {"t_less"}),
		Rule::SharedPtr("nt_binary_operator", {"t_leq"}),
		Rule::SharedPtr("nt_binary_operator", {"t_grt"}),
		Rule::SharedPtr("nt_binary_operator", {"t_geq"}),
		Rule::SharedPtr("nt_binary_operator", {"t_eq"}),
		Rule::SharedPtr("nt_binary_operator", {"t_neq"}),
		Rule::SharedPtr("nt_binary_operator", {"k_and"}),
		Rule::SharedPtr("nt_binary_operator", {"k_or"}),

		Rule::SharedPtr("nt_if", {"k_if", "nt_condition", "k_end"}),
		Rule::SharedPtr("nt_condition", {"nt_expression", "k_then", "nt_scope", "nt_condition_branch"}),
		Rule::SharedPtr("nt_condition_branch", {"k_else", "nt_scope"}),
		Rule::SharedPtr("nt_condition_branch", {"k_elseif", "nt_condition"}),
		Rule::SharedPtr("nt_condition_branch", {"epsilon"}),
		Rule::SharedPtr("nt_scope", {"nt_scope_statements", "nt_scope_return"}),
		Rule::SharedPtr("nt_scope_statements", {"nt_statements"}),
		Rule::SharedPtr("nt_scope_statements", {"epsilon"}),
		Rule::SharedPtr("nt_statements", {"nt_statements", "nt_statement"}),
		Rule::SharedPtr("nt_statements", {"nt_statement"}),
		Rule::SharedPtr("nt_statement", {"nt_if"}),  
		Rule::SharedPtr("nt_statement", {"nt_declare"}),  
		Rule::SharedPtr("nt_statement", {"nt_id"}),  
		Rule::SharedPtr("nt_statement", {"nt_while"}),  
		//Rule::SharedPtr("nt_statement", {"epsilon"}),
		Rule::SharedPtr("nt_global_scope", {"nt_function_declare"}),  
		Rule::SharedPtr("nt_global_scope", {"nt_function_define"}),  
		Rule::SharedPtr("nt_global_scope", {"nt_function_call"}),  
		Rule::SharedPtr("nt_program", {"k_require", "t_string", "nt_global_scope"}),  

		Rule::SharedPtr("nt_called_parameters", {"nt_rvalues"}),
		Rule::SharedPtr("nt_called_parameters", {"epsilon"}),
		Rule::SharedPtr("nt_function_call", {"t_id", "t_left", "nt_called_parameters", "t_right"}),  
		Rule::SharedPtr("nt_function_declare", {"k_global", "t_id", "t_def", "k_function", "t_left", "nt_parameters", "t_right", "nt_returning"}),  
		Rule::SharedPtr("nt_function_define", {"k_function", "t_id", "t_left", "nt_parameters_defined", "t_right", "nt_returning", "nt_scope", "k_end"}),  
		Rule::SharedPtr("nt_parameters", {"nt_parameters", "t_comma", "nt_parameter"}),  
		Rule::SharedPtr("nt_parameters", {"nt_parameter"}),  
		Rule::SharedPtr("nt_parameter", {"nt_parameter_name", "nt_datatype"}),
		Rule::SharedPtr("nt_parameters", {"epsilon"}),
		Rule::SharedPtr("nt_parameters_defined", {"nt_parameters_defined", "t_comma", "nt_parameter_defined"}),
		Rule::SharedPtr("nt_parameters_defined", {"nt_parameter_defined"}),
		Rule::SharedPtr("nt_parameters_defined", {"epsilon"}),
		Rule::SharedPtr("nt_parameter_defined", {"t_id", "t_def", "nt_datatype"}),
		Rule::SharedPtr("nt_scope_return", {"nt_return"}),
		Rule::SharedPtr("nt_scope_return", {"epsilon"}),
		Rule::SharedPtr("nt_return", {"k_return", "nt_rvalues"}),
		Rule::SharedPtr("nt_return", {"k_return"}),
		Rule::SharedPtr("nt_parameter_name", {"t_id", "t_def"}),
		Rule::SharedPtr("nt_parameter_name", {"epsilon"}),
		Rule::SharedPtr("nt_returning", {"t_def", "nt_datatypes"}), 
		Rule::SharedPtr("nt_returning", {"epsilon"}),



		Rule::SharedPtr("nt_datatypes", {"nt_datatypes", "t_comma", "nt_datatype"}),
		Rule::SharedPtr("nt_datatypes", {"nt_datatype"}),

		Rule::SharedPtr("nt_while", {"k_while", "nt_expression", "k_do", "nt_scope", "k_end"})

		});


	Factorization();

	RemoveLeftRecursion();


	CalculateEmpty();

	CalculateFirst();
	CalculateFollow();
	CalculatePredict();

	Symbol::PrintTable(Symbol::PRINT_FLAGS::SORTED | Symbol::PRINT_FLAGS::FIRST | Symbol::PRINT_FLAGS::RULES | Symbol::PRINT_FLAGS::EMPTY | Symbol::PRINT_FLAGS::FOLLOW);
	cout << "************************************************************************************\n";
	Rule::PrintRules();

	PrintLLTable();

	auto terminals = Terminal::Table();
	stringstream data;
	data << "typedef enum Terminal { ERROR, ";
	for (auto [id, terminal] : terminals)
	{
		string name = terminal->GetName();
		//if (name == "epsilon")
		//	continue;
		transform(name.begin(), name.end(), name.begin(), ::toupper);
		data << name << ", ";
	}
	data.seekp(-2, data.cur); 
	data << "} Terminal;";

	PrintToHeader("../../../../src/SyntaxAnalyzer/Terminal.h", data);

	auto nonterminals = NonTerminal::Table();
	data.str("");
	data << "typedef enum NonTerminal { ";
	for (auto [id, nonterminal] : nonterminals)
	{
		string name = nonterminal->GetName();
		transform(name.begin(), name.end(), name.begin(), ::toupper);
		data << name << ", ";
	}
	data.seekp(-2, data.cur);
	data << "} NonTerminal;";

	PrintToHeader("../../../../src/SyntaxAnalyzer/NonTerminal.h", data);

	ReplaceDefines("../../../../src/SyntaxAnalyzer/LLTable.h", terminals.size(), nonterminals.size(), Rule::Vector().size());

	PrintArrays("../../../../src/SyntaxAnalyzer/Arrays.c");

	return 0;
}