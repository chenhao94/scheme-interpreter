#include <iostream>
#include <string>
#include "extract.h"
#include "objects.h"
#include "environments.h"
#include "parse.h"
#include "execute.h"
#include "builtIn.h"
#include "compilingError.h"

int main()
{
	using namespace std;

	string cache, tmp, sentence;
	ParseTree_ptr tree = NULL;
	bool cacheEndFlag = false;
	Obj_ptr obj;

	while (!cin.eof())
	{
		cin.getline( tmp, 200 );
		cache.append(tmp);
		cacheEndFlag = false;

		while ( cacheEndFlag == false )
		{
			try
			{
				extract(sentence, cache, cacheEndFlag);
				tree = Parsing(sentence);
				obj = evaluate(tree);
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
