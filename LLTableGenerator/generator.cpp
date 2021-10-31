#include<iostream>
#include<memory>
#include<vector>
#include<map>
#include<algorithm>
#include<ranges>

#include"nonterminal.h"
#include"terminal.h"
#include"rule.h"

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
				// category[0]->Print();
				// category[1]->Print();
				string nonterminal_name = category[0]->GetLeft() + "'";
				shared_ptr<NonTerminal> new_nonterminal = dynamic_pointer_cast<NonTerminal>(Symbol::AddSymbol(NonTerminal::Pair(nonterminal_name)));
				Rule::AddRule(Rule::SharedPtr(category[0]->GetLeft(), { category[0]->GetRight(0,1)[0], nonterminal_name }));
				for (auto rule : category)
				{
					auto test = rule->GetRight(1, -1);
					Rule::AddRule(Rule::SharedPtr(nonterminal_name, (rule->GetRight().size() > 1) ? rule->GetRight(1, -1) : vector<string>{ "epsilon" })); //  not sure
					Rule::RemoveRule(rule);
				}
				to_check.push_back(new_nonterminal);
			}
		}

		to_check.erase(to_check.begin());
	}
}

/**
 * @brief Removes left recursion from rules by creating right recursion with new nonterminal and epsilon transition
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
			cout << right << endl;
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

	unsigned int cell_size = 22;
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
		NonTerminal::Pair("<prog>", true),
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
		Terminal::Pair("int"),
		Terminal::Pair("epsilon", true),
		Terminal::Pair("$")
	});
	/
	Rule::InitVector({
		Rule::SharedPtr("<prog>", {"begin", "<st-list>"}),
		Rule::SharedPtr("<st-list>", {"<stat>", ";", "<st-list>"}),
		Rule::SharedPtr("<st-list>", {"end"}),
		Rule::SharedPtr("<stat>", {"read", "id"}),
		Rule::SharedPtr("<stat>", {"write", "<item>"}),
		Rule::SharedPtr("<stat>", {"id", ":=", "add", "(", "<item>", "<it-list>"}),
		Rule::SharedPtr("<it-list>", {",", "<item>", "<it-list>"}),
		Rule::SharedPtr("<it-list>", {")"}),
		Rule::SharedPtr("<item>", {"int"}),
		Rule::SharedPtr("<item>", {"id"}),
		Rule::SharedPtr("<item>", { "<item>", ")" }),   random
		Rule::SharedPtr("<it-list>", {",", "add"}),   random
		Rule::SharedPtr("<it-list>", {",", "<item>", "id"}),   random

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
		NonTerminal::Pair("<program>", true),
		NonTerminal::Pair("<global_scope>"),
		NonTerminal::Pair("<function_declare>"),
		NonTerminal::Pair("<function_define>"),
		NonTerminal::Pair("<function_call>"),

		NonTerminal::Pair("<parameters>"),
		NonTerminal::Pair("<parameter>"),
		NonTerminal::Pair("<parameter_name>"),
		NonTerminal::Pair("<parameters_defined>"),
		NonTerminal::Pair("<parameter_defined>"),

		NonTerminal::Pair("<returning>"),

		NonTerminal::Pair("<scope>"),
		NonTerminal::Pair("<declare>"),
		NonTerminal::Pair("<id>"),
		NonTerminal::Pair("<if>"),
		NonTerminal::Pair("<while>"),
		NonTerminal::Pair("<return>"),
		NonTerminal::Pair("<declare_assign>"),
		NonTerminal::Pair("<assign>"),

		NonTerminal::Pair("<condition>"),
		NonTerminal::Pair("<condition_branch>"),
		
		NonTerminal::Pair("<lvalues>"),
		NonTerminal::Pair("<lvalue>"),
		NonTerminal::Pair("<rvalues>"),
		NonTerminal::Pair("<rvalue>"),

		NonTerminal::Pair("<expression>"),
		NonTerminal::Pair("<expression_2>"),
		NonTerminal::Pair("<expression_3>"),

		NonTerminal::Pair("<datatypes>"),
		NonTerminal::Pair("<datatype>"),

		NonTerminal::Pair("<unary_operator>"),
		NonTerminal::Pair("<binary_operator>"),


		Terminal::Pair("global"),
		Terminal::Pair("function"),
		Terminal::Pair("f_id"),
		Terminal::Pair("f_integer"),
		Terminal::Pair("f_number"),
		Terminal::Pair("f_string"),
		Terminal::Pair("f_minus"),
		Terminal::Pair("f_plus"),
		Terminal::Pair("f_mul"),
		Terminal::Pair("f_div"),
		Terminal::Pair("f_int_div"),
		Terminal::Pair("f_def"),
		Terminal::Pair("f_comma"),
		Terminal::Pair("f_len"),
		Terminal::Pair("f_left"),
		Terminal::Pair("f_right"),
		Terminal::Pair("f_less"),
		Terminal::Pair("f_leq"),
		Terminal::Pair("f_grt"),
		Terminal::Pair("f_geq"),
		Terminal::Pair("f_concat"),
		Terminal::Pair("f_ass"),
		Terminal::Pair("f_eq"),
		Terminal::Pair("f_neq"),
		Terminal::Pair("integer"),
		Terminal::Pair("number"),
		Terminal::Pair("string"),
		Terminal::Pair("nil"),
		Terminal::Pair("local"),
		Terminal::Pair("boolean"),
		Terminal::Pair("require"),
		Terminal::Pair("true"),
		Terminal::Pair("false"),
		Terminal::Pair("and"),
		Terminal::Pair("or"),
		Terminal::Pair("not"),

		Terminal::Pair("if"),
		Terminal::Pair("then"),
		Terminal::Pair("else"),
		Terminal::Pair("elseif"),
		Terminal::Pair("end"),

		Terminal::Pair("return"),
		Terminal::Pair("while"),
		Terminal::Pair("do"),

		Terminal::Pair("epsilon", true),
		Terminal::Pair("$")
	});


	Rule::InitVector({
		Rule::SharedPtr("<declare>", {"local", "<lvalues>", "f_def", "<datatypes>", "<declare_assign>"}), 
		Rule::SharedPtr("<declare_assign>", {"<assign>"}), 
		Rule::SharedPtr("<declare_assign>", {"epsilon"}), 
		Rule::SharedPtr("<lvalues>", {"<lvalues>", "f_comma", "<lvalue>"}), 
		Rule::SharedPtr("<lvalues>", {"<lvalue>"}), 
		Rule::SharedPtr("<lvalue>", {"f_id"}), 
		Rule::SharedPtr("<rvalues>", {"<rvalues>", "f_comma", "<rvalue>"}), 
		Rule::SharedPtr("<rvalues>", {"<rvalue>"}), 
		Rule::SharedPtr("<rvalue>", {"<expression>"}), 
		
		Rule::SharedPtr("<id>", {"f_id", "f_left", "<rvalues>", "f_right"}), 
		Rule::SharedPtr("<id>", {"f_id", "<assign>" }), 
		Rule::SharedPtr("<id>", {"f_id", "f_comma", "<lvalues>", "<assign>"}), 
		Rule::SharedPtr("<assign>", {"f_ass", "<rvalues>"}), 
		
		Rule::SharedPtr("<expression>", {"<expression>", "<binary_operator>", "<expression_2>"}), 
		Rule::SharedPtr("<expression>", {"<expression_2>"}), 
		Rule::SharedPtr("<expression_2>", {"<unary_operator>", "<expression_3>"}), 
		Rule::SharedPtr("<expression_2>", {"<expression_3>"}), 
		Rule::SharedPtr("<expression_3>", {"f_left", "<expression>", "f_right" }), 
		Rule::SharedPtr("<expression_3>", {"f_id", "f_left", "<rvalues>", "f_right"}),
		Rule::SharedPtr("<expression_3>", {"f_id"}), 
		Rule::SharedPtr("<expression_3>", {"f_integer"}), 
		Rule::SharedPtr("<expression_3>", {"f_number"}), 
		Rule::SharedPtr("<expression_3>", {"f_string"}),
		Rule::SharedPtr("<expression_3>", {"nil"}), 
		Rule::SharedPtr("<expression_3>", {"true"}),
		Rule::SharedPtr("<expression_3>", {"false"}),  

		Rule::SharedPtr("<unary_operator>", {"f_len"}),  
		Rule::SharedPtr("<unary_operator>", {"not"}),  
		Rule::SharedPtr("<binary_operator>", {"f_plus"}),  
		Rule::SharedPtr("<binary_operator>", {"f_minus"}),  
		Rule::SharedPtr("<binary_operator>", {"f_mul"}),  
		Rule::SharedPtr("<binary_operator>", {"f_div"}),  
		Rule::SharedPtr("<binary_operator>", {"f_int_div"}),  
		Rule::SharedPtr("<binary_operator>", {"f_concat"}),  

		Rule::SharedPtr("<datatype>", {"integer"}),  
		Rule::SharedPtr("<datatype>", {"number"}),  
		Rule::SharedPtr("<datatype>", {"string"}),  
		Rule::SharedPtr("<datatype>", {"boolean"}),  

		Rule::SharedPtr("<binary_operator>", {"f_less"}),  
		Rule::SharedPtr("<binary_operator>", {"f_leq"}),  
		Rule::SharedPtr("<binary_operator>", {"f_grt"}),  
		Rule::SharedPtr("<binary_operator>", {"f_geq"}),  
		Rule::SharedPtr("<binary_operator>", {"f_eq"}),  
		Rule::SharedPtr("<binary_operator>", {"f_neq"}),  
		Rule::SharedPtr("<binary_operator>", {"and"}),  
		Rule::SharedPtr("<binary_operator>", {"or"}),  

		Rule::SharedPtr("<if>", {"if", "<condition>", "end"}),  
		Rule::SharedPtr("<condition>", {"<expression>", "then", "<scope>", "<condition_branch>"}),  
		Rule::SharedPtr("<condition_branch>", {"else", "<scope>"}),  
		Rule::SharedPtr("<condition_branch>", {"elseif", "<condition>"}),  
		Rule::SharedPtr("<condition_branch>", {"epsilon"}),  
		Rule::SharedPtr("<scope>", {"<if>"}),  
		Rule::SharedPtr("<scope>", {"<declare>"}),  
		Rule::SharedPtr("<scope>", {"<id>"}),  
		Rule::SharedPtr("<scope>", {"<while>"}),  
		Rule::SharedPtr("<scope>", {"<return>"}),  
		Rule::SharedPtr("<global_scope>", {"<function_declare>"}),  
		Rule::SharedPtr("<global_scope>", {"<function_define>"}),  
		Rule::SharedPtr("<global_scope>", {"<function_call>"}),  
		Rule::SharedPtr("<program>", {"require", "f_string", "<global_scope>"}),  

		Rule::SharedPtr("<function_call>", {"f_id", "f_left", "<rvalues>", "f_right"}),  
		Rule::SharedPtr("<function_declare>", {"global", "f_id", "f_def", "function", "f_left", "<parameters>", "f_right", "<returning>"}),  
		Rule::SharedPtr("<function_define>", {"function", "f_id", "f_left", "<parameters_defined>", "f_right", "<returning>", "<scope>", "end"}),  
		Rule::SharedPtr("<parameters>", {"<parameters>", "f_comma", "<parameter>"}),  
		Rule::SharedPtr("<parameters>", {"<parameter>"}),  
		Rule::SharedPtr("<parameter>", {"<parameter_name>", "<datatype>"}),
		Rule::SharedPtr("<parameters>", {"epsilon"}),
		Rule::SharedPtr("<parameters_defined>", {"<parameters_defined>", "f_comma", "<parameter_defined>"}),
		Rule::SharedPtr("<parameters_defined>", {"<parameter_defined>"}),
		Rule::SharedPtr("<parameters_defined>", {"epsilon"}),
		Rule::SharedPtr("<parameter_defined>", {"f_id", "f_def", "<datatype>"}),
		Rule::SharedPtr("<return>", {"return", "<rvalues>"}),
		Rule::SharedPtr("<return>", {"return"}),
		Rule::SharedPtr("<parameter_name>", {"f_id", "f_def"}),
		Rule::SharedPtr("<parameter_name>", {"epsilon"}),
		Rule::SharedPtr("<returning>", {"f_def", "<datatypes>"}), 
		Rule::SharedPtr("<returning>", {"epsilon"}),



		Rule::SharedPtr("<datatypes>", {"<datatypes>", "f_comma", "<datatype>"}),
		Rule::SharedPtr("<datatypes>", {"<datatype>"}),

		Rule::SharedPtr("<while>", {"while", "<expression>", "do", "<scope>", "end"})

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

	return 0;
}