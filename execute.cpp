/**
 *	give a parse tree, execute it and give the output
 */

#include "execute.h"
#include <cctype>

typedef Environment* env_ptr;

void checkToken(const std::string &, bool &, bool &, bool &, bool &);

Object* findIden(env_ptr env, std::string);

Object* evaluate(ParseTree* root, const env_ptr & env)
{
	std::string token = root->getToken();
	Object *obj;

	bool numberFlag = true, rationalFlag = false, realFlag = false, idenFlag = true;

	checkToken(token, numberFlag, rationalFlag, realFlag, idenFlag);

	//----number-----
	if (numberFlag)
	{
		//----integer-----
		if (!rationalFlag && !realFlag)
			obj = new IntegerObj(token);
		//----rational----
		else if (rationalFlag)
			obj = new RationalObj(token);
		//----real--------
		else
			obj = new RealObj(token);
		return obj;
	}

	//----identifier----
	else if (idenFlag)
	{
		obj = findIden(env);
		if (obj==NULL)
			throw syntaxError("Undefined identifier: " + token);
		return obj;
	}

	//----char&bool-----
	else if (token[0]=='#')
	{
		//----char-----
		if (token[1]=='\\')
		{
			if (token=="#\\newline")
				obj = new CharObj('\n');
			else if (token=="#\\space")
				obj = new CharObj(' ');
			else if (token="#\\tab")
				obj = new CharObj('\t');
			else
				obj = new CharObj(token[2]);

			return obj;
		}
		//----bool-----
		else
		{
			
			if (token[1]=='t')
				obj = new BoolObj(true);
			else
				obj = new BoolObj(false);

			return obj;
		}
	}

	//----string-------
	else if (token[0]=='\"')
	{
		obj = new StringObj(token.substr(1, npos-1));
		return obj;
	}

	//----symbol-------
	else if (token[0]=='\'')
	{
		//W.T.F.
	}

	//----syntax&procedure----
	else if (token=="()")
	{
		ParseTree *name = root->getSon();
		std::string iden = name->getToken();
		Object *obj = findIden(env, iden);

		if (obj==NULL)
		{
			// use set to check if it is a built-in keyword
			// W.T.F.
			throw syntaxError("Undefined identifier: " + iden);
		}
		else if (obj->Type != Procedure)
			throw syntaxError("\'" + iden + "\' is not a procedure");

		if (obj)	//define by user
		{
			//W.T.F.
		}
		else		// built-in
		{
			//W.T.F.
		}

	}

	//----else----------
	else
	{
		throw syntaxError("Illegal expression or the function you use is not available now.");
	}

}

Object* findIden(env_ptr env, std::string name);
{
	Object *obj;
	while (env!=NULL)
	{
		obj = env.FindObj(name);
		if (obj!=NULL)
			return obj;
		env = env->next;
	}
	return NULL;
}

void checkToken(const std::string &token, bool &numberFlag, bool &rationalFlag, bool &realFlag, bool &idenFlag)
{

	int token_size = token.size();

	if (!isalpha(token[0]))
		idenFlag = false;
	if (!isdigit(token[0]) && token[0]!='.')
		numberFlag = false;
	if (token[token_size-1]=='/')
		numberFlag = false;

	if (idenFlag)
	{
		for (int i=1; i<token_size; ++i)
			if (!isdigit(token[i]) && !isalpha(token[i]) && token[i]!='_')
			{
				idenFlag = false;
				return;
			}

	}
	else if (numberFlag)
	{
		for (int i=0; i<token_size; ++i)
		{
			if (!isdigit(token[i]) && token[i]!='.' && token[i]!='/')
			{
				numberFlag = false;
				return;
			}
			if (token[i]=='.')
			{
				if (realFlag || rationalFlag)
				{
					numberFlag = false;
					return;
				}
				realFlag = true;
			}
			else if (token[i]=='/')
			{
				if (realFlag || rationalFlag)
				{
					numberFlag = false;
					return;
				}
				rationalFlag = true;
			}
		}
	}
}
