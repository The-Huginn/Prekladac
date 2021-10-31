#include "fileoutput.h"

#include<algorithm>
#include<fstream>
#include<iostream>

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