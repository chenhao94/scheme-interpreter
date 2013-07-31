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
		else
		{
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
			// W.T.F.
		}
		else if ( code[1]=='(' )
		{
			// vector
			// W.T.F.
		}
		else if (code_size==2 && code[1]=='t')
		{
			// bool value "true"
			// W.T.F.
		}
		else if (code_size==2 && code[1]=='f')
		{
			// bool value "false"
			// W.T.F.
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
		// W.T.F.
	}
	else if ( code[0]=='\"' )
	{
		// string
		doubleQuote:
		// W.T.F.
	}
	else if ( code[0] ) // identifiers or numbers without redundant whitspaces
	{
		// 
	}
}
