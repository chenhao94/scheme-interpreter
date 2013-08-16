#include <iostream>
#include <string>
#include <cstring>
#include "extract.h"
#include "parse.h"
#include "execute.h"
#include "error.h"
#include "Import.h"
#include "Help.h"
int main( int argc, char *argv[] )
{
	using namespace std;

	bool cacheEndFlag = false;
	string tmp;
	string cache, sentence;
	Obj_ptr obj;
	ParseTree_ptr tree = nullptr;
	env_ptr env(new Environment() );

	ImportFile("./lib/stdlib.scm", env);

	if (argc > 1)
	{
		if ( strcmp(argv[1], "--help") == 0 )
		{
			displayHelp();
			return 0;
		}
		else if ( strcmp(argv[1], "-i") == 0 )
		{
			//import
			for (int i=2; i<argc; ++i)
				ImportFile(argv[i], env);
		}
		else
		{
			cout << "Unrecognized command line option, you can use \"--help\" to check it" << endl;
			return 0;
		}
	}

	cout << "Welcome to CH's Scheme Interpreter!!" << endl;

	while (!cin.eof())
	{
		if (cache.size() == 0)
			cout << "> ";

		getline(cin, tmp);
		cache.append("\n" + tmp);

		eliminateComment(cache);
		cacheEndFlag = false;

		while ( cacheEndFlag == false )
		{
			try
			{
				extract(sentence, cache, cacheEndFlag);
				tree = Parsing(sentence);
				obj = evaluate(tree, env);
				if (obj != nullptr)
					cout << obj->ExternalRep() << endl;
			}
			catch (syntaxError err)
			{
				cache.clear();
				cacheEndFlag = true;
				cout << err.getMsg() << endl;
			}
			catch (runtimeError err)
			{
				cache.clear();
				cacheEndFlag = true;
				cout << err.getMsg() << endl;
			}
		}
	}

	return 0;
}
