#include "terminal.h"

void Terminal::Print(unsigned int flags) const
{
	printf("%s - Terminal\n", name.c_str());
	if (flags && PRINT_FLAGS::FIRST)
		cout << "\tFirst: { " << first << "}" << endl;
}

Terminal::Terminal(string name) : Symbol(name)
{
	first = { name };

}

Terminal::~Terminal()
{
}
