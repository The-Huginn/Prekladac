#include "fileoutput.h"

#include<algorithm>
#include<fstream>
#include<iostream>

#include"nonterminal.h"
#include"terminal.h"
#include"rule.h"

void PrintToHeader(string path, stringstream& data)
{
	string name = path.substr(path.find_last_of("/\\") + 1);

	ofstream file;
	file.open(path);
	if (!file)
		cout << "File [" << name << "] not found!" << endl;

	transform(name.begin(), name.end(), name.begin(), ::toupper);
	replace(name.begin(), name.end(), '.', '_');

	file << "#ifndef __" << name << "__" << endl;
	file << "#define __" << name << "__" << endl << endl;

	file << data.str() << endl << endl;

	file << "#endif // !__" << name << "__" << endl;
	
	file.close();
}

void ReplaceDefines(string path, int term_size, int nonterm_size, int rule_size)
{
	string name = path.substr(path.find_last_of("/\\") + 1);
	string dir = path.substr(0, path.find_last_of("/\\"));
	string temp_path = dir + "/temp.h";

	ifstream input;
	ofstream output;
	input.open(path, ios::binary);
	output.open(temp_path, ios::binary);
	if (!input)
		cout << "File [" << name << "] not found!" << endl;

	string line;
	while (getline(input, line))
	{
		if (line.find("#define LLTABLE_WIDTH") != string::npos)
			line = "#define LLTABLE_WIDTH " + to_string(term_size);
		if (line.find("#define LLTABLE_HEIGHT") != string::npos)
			line = "#define LLTABLE_HEIGHT " + to_string(nonterm_size);
		if (line.find("#define RULE_ARRAY_SIZE") != string::npos)
			line = "#define RULE_ARRAY_SIZE " + to_string(rule_size);

		output << line << "\n";
	}

	input.close();
	output.close();
	remove(path.c_str());
	rename(temp_path.c_str(), path.c_str());
}

void PrintArrays(string path)
{
	string name = path.substr(path.find_last_of("/\\") + 1);

	ofstream file;
	file.open(path);
	if (!file)
		cout << "File [" << name << "] not found!" << endl;

	file << "#include\"LLTable.h\"\n\n";

	file << "Rule rules[RULE_ARRAY_SIZE] = {\n";

	map<string, int> terminals, nonterminals;

	auto non_table = NonTerminal::Table();
	int index = 0;
	for (auto [id, nonterminal] : non_table)
		nonterminals[nonterminal->GetName()] = index++;

	auto ter_table = Terminal::Table();
	index = 1;
	for (auto [id, terminal] : ter_table)
		terminals[terminal->GetName()] = index++;
	
	for (auto rule : Rule::Vector())
	{
		auto right = rule->GetRight();
		file << "{ .size = " << right.size() << ", ";
		file << ".left_side = { .isTerminal = false, " << ".value = " << nonterminals[rule->GetLeft()] << " }, ";
		file << ".right_side = { ";

		for (string r : right)
			if (nonterminals.find(r) != nonterminals.end())
				file << "{ .isTerminal = false, " << ".value = " << nonterminals[r] << "}, ";
			else
				file << "{ .isTerminal = true, " << ".value = " << terminals[r] << "}, ";


		file.seekp(-2, file.cur);
		file << " }";

		file << " },\n";
	}
	file.seekp(-3, file.cur);
	file << " };\n\n\n";


	file << "int LLTable[LLTABLE_HEIGHT][LLTABLE_WIDTH] = {\n";
	auto rules = Rule::Vector();
	for (auto [id, nonterminal] : non_table)
	{
		file << "{ ";
		for (auto [id, terminal] : ter_table)
		{
			int index = -1;
			for (auto rule : nonterminal->GetRules())
			{
				auto predict = rule->GetPredict();
				if (predict.find(terminal->GetName()) != predict.end())
				{
					auto it = find(rules.begin(), rules.end(), rule);
					index = it - rules.begin();
				}
			}
			file << index << ", ";
		}
		file.seekp(-2, file.cur);
		file << " },\n";
	}
	file.seekp(-3, file.cur);
	file << " };\n\n\n";

	file.close();
}
