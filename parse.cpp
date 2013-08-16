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
	son = nullptr;
	brother = nullptr;
}

ParseTree_ptr Parsing(std::string code) // code should be refined first
{
	while ( isspace(code.back()) ) code.pop_back();

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
				ParseTree_ptr context( Parsing(code) );
				if (code[0]=='\'')
					return ParseTree_ptr( new ParseTree("\'", context));
				else
					return ParseTree_ptr( new ParseTree("`", context));
			}

		}
		else		//	'( ... ) || `( ... )
		{
			ParseTree_ptr context( Parsing(code.substr(1))), quote;
			if (code[0] == '\'')
				quote = ParseTree_ptr( new ParseTree("\'", context) );
			else
				quote = ParseTree_ptr( new ParseTree("`", context));
			return quote;
		}
	}
	else if ( code[0]==',' )
	{
		std::string sign, token;
		ParseTree_ptr tree, subtree;
		int pos = 0;

		sign.clear();
		sign.push_back(code[pos++]);
		if (code[pos]=='@')
			sign.push_back(code[pos++]);

		subtree = ParseTree_ptr( Parsing( code.substr(pos) ) );
		tree = ParseTree_ptr(new ParseTree(sign, subtree) );

		return tree;

	}
	else if ( code[0]=='#' )
	{
		sharp:
		if (code_size==1)
			throw syntaxError("bad syntax \'#\'");
		else if ( code[1]=='\\' )
		{
			//char
			
			// ------lowercase-------
			if (code_size>3)
				for (int i=0; i<code_size; ++i)
					code[i] = tolower(code[i]);

			if (code_size==3 || ( code_size<10 && code == "#\\space" || code == "#\\tab" || code=="#\\newline" ))
			{
				ParseTree_ptr node( new ParseTree(code) );
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
				ParseTree_ptr node( new ParseTree(code) );
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
		ParseTree_ptr listToken, newName, lastName = NULL;
		std::string name;
		int pos = 1;

		if (code.back()!=')')
			throw syntaxError("expected a \')\' to close \'(\'");

		while ( getToken( name, code, pos ) )
		{
			newName = Parsing(name);
			if (lastName==NULL)
				listToken = ParseTree_ptr( new ParseTree("()", newName) );
			else
				lastName->brother = newName;
			lastName = newName;
			if (name[0] == ',')
			{
				if ( !getToken( name, code, pos ) )
					throw syntaxError("illegal use: \',\' or \',@\'");
				lastName->son = Parsing(name);
			}
		}

		if (lastName == NULL)
			listToken = ParseTree_ptr( new ParseTree("()") );

		return listToken;
	}
	else if ( code[0]=='\"' )
	{
		// string
		doubleQuote:
		ParseTree_ptr node( new ParseTree(code) );
		return node;
	}
	else if (code == ".")
	{
		// ( x . y )
		ParseTree_ptr node( new ParseTree(code) );
		return node;
	}
	else // identifiers or numbers without redundant whitspaces
	{
		bool rationalFlag = false, realFlag = false, numbersFlag = true;
		
		// ------lowercase-------
		for (int i=0; i<code_size; ++i)
			code[i] = tolower(code[i]);

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

		ParseTree_ptr node( new ParseTree(code) );
		return node;

	}
}

bool getToken( std::string &name, const std::string &code, int &pos )
{
	// distract the first token from list, start with 'pos', put it in 'name', if failed, return false
	
	int pareLevel = 0; // level of parentheses
	bool quoteFlag = false; // flag of quotaion

	int code_size = code.size();

	name.clear();

	while ( isspace(code[pos]) && pos<code_size-1 ) ++pos;

	if (pos>=code_size-1)
		return false;

	if (code[pos]==')')
		throw syntaxError("unexpected \')\' here");

	if (code[pos]=='\"')						//string
	{
		doubleQuotation:
		name.push_back(code[pos++]);
		quoteFlag = true;

		while (pos<code_size-1)
		{
			if (code[pos]=='\"')
			{
				quoteFlag = false;
				name.push_back(code[pos++]);
				break;
			}
			else if (code[pos]=='\\')
			{
				name.push_back(code[pos++]);
				name.push_back(code[pos++]);
			}
			else
			{
				name.push_back(code[pos++]);
			}
		}

		if (pos>code_size-1 || quoteFlag==true)
			throw syntaxError("expect a \'\"\' here");

		return true;
	}
	else if (code[pos]==',')					//comma or comma_at
	{
		name.push_back(code[pos++]);

		while ( isspace(code[pos]) && pos<code_size-1 ) ++pos;

		if (code[pos]=='@')
			name.push_back(code[pos++]);

		return true;
	}
	else if (code[pos]=='\'' || code[pos]=='`')	// quotation
	{
		quotation:
		name.push_back(code[pos++]);
		while ( isspace(code[pos]) && pos<code_size-1 ) ++pos;
		
		if (pos>=code_size-1 || /*code[pos]=='\'' || code[pos]=='`' || code[pos]==',' || code[pos]=='@' ||*/ code[pos]==')')
			throw syntaxError("Illegal use of quotation");
		
		if (code[pos]=='\"')
			goto doubleQuotation;
		else if (code[pos]=='(')
			goto parentheses;
		else
			goto commonplace;
	}
	else if (code[pos]=='(')					// list
	{
		parentheses:
		++pareLevel;
		name.push_back(code[pos++]);

		while (pos<code_size-1)
		{
			if (quoteFlag==true)
			{
				if (code[pos]=='\"')
				{
					quoteFlag = false;
					name.push_back(code[pos++]);
				}
				else if (code[pos]=='\\')
				{
					name.push_back(code[pos++]);
					name.push_back(code[pos++]);
				}
				else
				{
					name.push_back(code[pos++]);
				}
			}
			else
			{
				if (code[pos]=='\"')
				{
					quoteFlag = true;
					name.push_back(code[pos++]);
				}
				else if (code[pos]=='(')
				{
					++pareLevel;
					name.push_back(code[pos++]);
				}
				else if (code[pos]==')')
				{
					--pareLevel;
					name.push_back(code[pos++]);
					if (pareLevel==0)
						break;
				}
				else
					name.push_back( tolower(code[pos++]) );
			}
		}

		if (pos>code_size-1 || pareLevel!=0 || quoteFlag==true)
			throw syntaxError("Illegal list expression");

		return true;
	}
	else if (code[pos]=='#')
	{
		name.push_back(code[pos++]);

		if (code[pos]=='\\')
		{
			name.push_back(code[pos++]);
			name.push_back(code[pos++]);
		}

		goto commonplace;
	}
	else										//commonplace
	{
		commonplace:
		while (pos<code_size-1)
		{
			if (code[pos]=='(' || code[pos]==')' || code[pos]=='\'' || code[pos]=='`' || code[pos]=='\"' || code[pos]==',' || isspace(code[pos]))
				break;
			else
				name.push_back( tolower(code[pos++]) );
		}
		return true;
	}

}
