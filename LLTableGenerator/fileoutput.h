#pragma once

#include<string>
#include<sstream>

using namespace std;

void PrintToHeader(string path, stringstream& data);

void ReplaceDefines(string path, int term_size, int nonterm_size, int rule_size);

void PrintArrays(string path);