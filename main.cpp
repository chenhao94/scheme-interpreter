#include <iostream>
#include <string>
#include "extract.h"
#include "parse.h"
#include "execute.h"
#include "error.h"

int main()
{
	using namespace std;

	int commentPos;
	bool cacheEndFlag = false;
	string tmp;
	string cache, sentence;
	Obj_ptr obj;
	ParseTree_ptr tree = nullptr;
	env_ptr env(new Environment() );

	cout << "Welcome to CH's Scheme Interpreter!!" << endl;

	while (!cin.eof())
	{
		if (cache.size() == 0)
			cout << "> ";

		getline(cin, tmp);
		cache.append(tmp);

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
