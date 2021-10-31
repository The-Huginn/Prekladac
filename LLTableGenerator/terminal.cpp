#include "terminal.h"

map<string, shared_ptr<Terminal>> Terminal::table_terminal = {};

void Terminal::Print(unsigned int flags) const
{
	printf("%s - Terminal\n", name.c_str());
	if (flags & PRINT_FLAGS::EMPTY)
		cout << "\tEmpty: " << empty << endl;
	if (flags & PRINT_FLAGS::FIRST)
		cout << "\tFirst: { " << first << "}" << endl;
	if (flags & PRINT_FLAGS::FOLLOW)
		cout << "\tFollow: { " << follow << "}" << endl;
}

Terminal::Terminal(string name, bool empty) : Symbol(name)
{
	first = { name };
	this->empty = empty;
}

Terminal::~Terminal()
{
}
