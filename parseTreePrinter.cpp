/**
 * 	Parse Tree Printer is used for debugging by outputing a parse tree generated by "parse.h" and "parse.cpp"
 *	without comments
 */

#include <iostream>
#include <string>
#include "parse.h"
#define width 100
#define height 5
#define fieldwidth 10 

using namespace std;

string s;

ParseTree_ptr tree;

char graph[500][500]={0};

//int layerMax[100]={0};
int layerMax=-1;
int depth=0;

void dfs( ParseTree_ptr root, long layer, long fieldpos)
{
	if (root == NULL)
		return;
	string s = root->getToken();
	int len = s.size(), pos = fieldwidth * fieldpos;
	int verPos = height * layer;

	ParseTree_ptr son = root->getSon(), brother = root->getBrother();

	if (layer>depth)
		depth = layer;
	if (fieldpos>layerMax/*[layer]*/)
		layerMax/*[layer]*/ = fieldpos;

	for (int i=0; i<len; ++i)
		graph[verPos][pos + i] = s[i];

	if (son)
	{
		for (int i=1; i<height; ++i)
			graph[verPos + i][pos] = '|';
		dfs(son, layer + 1, fieldpos);
	}

	if (brother)
	{
		int nextfpos = fieldpos + 1;
		int nextpos;
		if (layerMax/*[layer + 1]*/ + 1 >nextfpos)
			nextfpos = layerMax/*[layer + 1]*/ + 1;
		nextpos = fieldwidth * nextfpos - 1;

		for (int i = pos + len + 1; i<nextpos; ++i)
			graph[verPos][i] = '-';

		dfs(brother, layer, nextfpos);
	}
}

int main()
{
	char st[500];

	while (1)
	{
		gets(st);
		s = st;
		if (s[s.size()-1]=='\n')
			s.pop_back();

		depth = 0;

		try
		{
			tree = Parsing(s);
		}
		catch (syntaxError err) { cout << err.getMsg() << endl; continue; }

		for (int i=0; i<=width; ++i)
			for (int j=0; j<=width; ++j)
				graph[i][j]=' ';

		//for (int i=0; i<100; ++i)
		//	layerMax[i]=-1;
		layerMax=-1;

		dfs(tree, 0, 0);

		cout << endl;

		for (int i=0; i<=height * depth; ++i)//WTF
		{
			for (int j=0; j<width; ++j)
				if (graph[i][j]==0)
					cout << ' ';
				else
					cout << graph[i][j];
			cout << endl;

		}

		cout << endl;
	}


	return 0;
}
