#include <iostream>
#include <string>
#include "extract.h"
#include "parse.h"
#include "execute.h"
#include "compilingError.h"

int main()
{
	using namespace std;

	bool cacheEndFlag = false;
	char tmp[205];
	string cache, sentence;
	Obj_ptr obj;
	ParseTree_ptr tree = NULL;
	env_ptr env(new Environment() );

	cout << "Welcome to CH's Scheme Interpreter!!" << endl;

	while (!cin.eof())
	{
		if (cache.size() == 0)
			cout << "> ";
		cin.getline( tmp, 200 );
		cache.append(tmp);
		cacheEndFlag = false;

		while ( cacheEndFlag == false )
		{
			try
			{
				extract(sentence, cache, cacheEndFlag);
				tree = Parsing(sentence);
				obj = evaluate(tree, env);
				if (obj != NULL)
					cout << obj->ExternalRep() << endl;
			}
			catch (syntaxError err)
			{
				cache.clear();
				cacheEndFlag = true;
				cout << err.getMsg() << endl;
			}
		}
	}

	return 0;
}
