/**
 * 	Generate and process the parse tree
 *
 */

#include "parse.h"
#include <cctype>
#include <string>
#include <sstream>

ParseTree::~ParseTree()
{
	if ( son!=NULL )
		son->~ParseTree();
	if ( brother!=NULL )
		brother->~ParseTree();
}

void ParseTree::evaluate()
{
	// W.T.F
}

ParseTree* Parsing(std::string code) // code should be refined first
{
	int code_size = code.size();
	if (code_size==0)
		return NULL;

	if (code[0]=='\'' || code[0]=='`')
	{
		// quotation
		if (code_size==1)
			throw syntaxError("bad syntax \'" + code + "\'");
		else if (code[1]!='(')
		{
			if (code[1]==')')
				throw syntaxError("unexpected \')\'");
			else if (code[1]=='#')
			{
				code.erase(0,1);
				--code_size;
				goto sharp;
			}
			else if (code[1]=='\"')
			{
				code.erase(0,1);
				--code_size;
				goto doubleQuote;
			}
			else
			{
				code.erase(0,1);
				ParseTree *context = new ParseTree(code);
				ParseTree *quote = new ParseTree("\'", context);
				return quote;
			}

		}
		else if (code[0]=='\'')		//	'( ... )
		{
			bool firstName = true;
			ParseTree *quote, *lastName = NULL, *newName;
			std::string name;
			int pos = 1;

			while (getToken(name, code, pos))
			{
				newName = new ParseTree(name);

				if (lastName==NULL)
					quote = new ParseTree("\'", newName);
				else
					lastName.brother = newName;

				lastName = newName;
			}
		}
		else // quasiquotation		`( ... )
		{
			// W.T.F.
		}
		return;
	}
	else if ( code[0]=='#' )
	{
		sharp:
		if (code_size==1)
			throw syntaxError("bad syntax \'#\'");
		else if ( code[1]=='\\' )
		{
			//char
			if (code_size==3 || ( code_size<10 && code == "#\\space" || code == "#\\tab" || code=="#\\newline" ))
			{
				ParseTree *node = new ParseTree(code);
				return node;
			}
			else 
				throw syntaxError("bad character constant: " + code);
		}
		else if ( code[1]=='(' )
		{
			// vector
			throw syntaxError("vectors are not available now");
			// W.T.F.
		}
		else if (code_size==2)
		{
			// bool value
			if (code[1]=='t' || code[1]=='f')
			{
				ParseTree *node = new ParseTree(code);
				return node;
			}
			else
				throw syntaxError("bad syntax \'" + code + "\'");
		}
		else
		{
			// other external representation unavailable
			// or bad syntax
			std::istringstream ins(code);
			std::string stmp;
			ins >> stmp;
			throw syntaxError("bad syntax \'" + stmp + "\'");
		}
	}
	else if ( code[0]=='(' )
	{
		// list (syntax or procedure)
		ParseTree *listToken, *newName, *lastName = NULL;
		std::string name;
		int pos = 1;

		while ( getToken( name, code, pos ) )
		{
			newName = Parsing(name);
			if (lastName==NULL)
				listToken = new ParseTree("()", newName);
			else
				lastName.brother = newName;
			lastName = newName;
		}

		if (lastName == NULL)
			throw syntaxError("missing procedure expression");

		return listToken;
	}
	else if ( code[0]=='\"' )
	{
		// string
		doubleQuote:
		ParseTree *node = new ParseTree(code);
		return node;
	}
	else // identifiers or numbers without redundant whitspaces
	{
		bool rationalFlag = false, realFlag = false, numbersFlag = true;
		
		for (int pos = 0; pos<code_size; ++pos)
		{
			if ( code[pos]=='.' )
			{
				if ( code_size==1 || rationalFlag || realFlag )
				{
					numbersFlag = false;
					break;
				}
				realFlag = true;
			}
			else if (code[pos]=='/' )
			{
				if (pos==0 || pos==code_size-1 || rationalFlag || realFlag)
				{
					numbersFlag = false;
					break;
				}
				rationalFlag = true;
			}
			else if ( !isdigit(code[pos]) )
			{
				numbersFlag = false;
				break;
			}
		}

		if ( numbersFlag==false )
		{
			if ( isdigit(code[0]) || code[0]=='.' )
				throw syntaxError("Illegal identifier: " + code );
		}

		ParseTree *node = new ParseTree(code);
		return node;

	}
}

bool getToken( std::string &name, const std::string &code, int &pos )
{}
