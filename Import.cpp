/**
 * 	To import the library files
 */

#include "Import.h"

void ImportFile(const std::string & addr, env_ptr & env)
{
	using namespace std;
	ifstream fin;

	bool cacheEndFlag = false;
	string tmp;
	string cache, sentence;
	Obj_ptr obj;
	ParseTree_ptr tree = nullptr;

	fin.open(addr, ios::in);

	if (!fin.is_open())
	{
		cout << "Warning: " << addr << ": file not exists !!" << endl;
		return;
	}

	while (!fin.eof())
	{
		getline(fin, tmp);
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

	if (!cache.empty())
		cout << "Warning: " << addr << ": not end normally!!" << endl;

}
