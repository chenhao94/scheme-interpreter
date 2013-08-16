/**
 * To display the help information
 */

#include <iostream>
#include <fstream>
#include <string>
#include "Help.h"

void displayHelp()
{
	using namespace std;
	ifstream fin;
	string line;
	
	fin.open("./help/helpInfo", ios::in);
	
	if (!fin.is_open())
		cout << "Error: help info file does not exists!!" << endl;

	while (!fin.eof())
	{
		getline(fin, line);
		cout << line << endl;
	}
}
