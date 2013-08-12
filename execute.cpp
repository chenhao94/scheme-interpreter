/**
 *	give a parse tree, execute it and give the output
 */

#include "execute.h"
#include <cctype>

Obj_ptr evaluate(ParseTree_ptr root, const env_ptr & env)
{
	std::string token = root->getToken();
	Obj_ptr obj;

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
		return obGj;
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
		ParseTree_ptr name = root->getSon();
		std::string iden = name->getToken();

		//--------syntax------------
		if (builtInSyntax.find(iden) != builtInSyntax.end())
		{
			return evaluateSyntax(iden, name, env);
		}
		
		//--------procedure---------

		Obj_ptr obj = findIden(env, iden);

		if (obj==NULL)
		{
			if (builtInProcedure.find(iden) == builtInProcedure.end())
				throw syntaxError("Undefined identifier: " + iden);
		}
		else if (obj->Type != Procedure)
			throw syntaxError("\'" + iden + "\' is not a procedure");

		ParseTree_ptr para = name->getBrother();
		Parameter_ptr head=NULL, tail=NULL;

		while (para!=NULL)
		{
			if (head==NULL)
			{
				head = new Parameters(evaluate(para));
				tail = head;
			}
			else
			{
				tail.next = new Parameters(evaluate(para));
				tail = tail.next;
			}

			para = para->getBrother();
		}

		if (obj)	//define by user
		{
			// !!!!!!!ENVIRONMENT!!!!!!!!
			return evaluateUserDefined(obj, head, env);
		}
		else		// built-in
		{
			return evaluateBuiltInProcedure(iden, head, env);
		}

	}

	//----else----------
	else
	{
		throw syntaxError("Illegal expression or the function you use is not available now.");
	}

}

Obj_ptr findIden(env_ptr env, const std::string &name);
{
	Obj_ptr obj;
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

	idenFlag = false;
	numberFlag = true;
	rationalFlag = realFlag = false;

	if (!isdigit(token[0]) && token[0]!='.')
		numberFlag = false;
	if (token[token_size-1]=='/')
		numberFlag = false;

	if (numberFlag)
	{
		for (int i=0; i<token_size; ++i)
		{
			if (!isdigit(token[i]) && token[i]!='.' && token[i]!='/')
			{
				numberFlag = false;
				break;
			}
			if (token[i]=='.')
			{
				if (realFlag || rationalFlag)
				{
					numberFlag = false;
					break;
				}
				realFlag = true;
			}
			else if (token[i]=='/')
			{
				if (realFlag || rationalFlag)
				{
					numberFlag = false;
					break;
				}
				rationalFlag = true;
			}
		}
	}
	
	if (!numberFlag)
	{
		idenFlag = true;
		if (token[0] == '#')
			idenFlag = false;
		else
			for (int i=0; i<token_size; ++i)
				if (token[i]=='(' || token[i]==')' || token[i]=='\"' || token[i]=='`' || token[i]=='\'')
				{
					idenFlag = false;
					break;
				}

	}

	return;
}

