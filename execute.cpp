/**
 *	give a parse tree, execute it and give the output
 */

#include "execute.h"
#include <cctype>

builtInSet builtInSyntax({ "if","cond","case","else","define","set!","lambda","quote","quasiquote" });

builtInSet builtInProcedure({"+","-","*","/","<",">","<=",">=","gcd","max","min","exact->inexact","inexact->exact","cons","car","cdr","eq?","eqv?","equal?"});

Obj_ptr evaluate(const ParseTree_ptr &root, env_ptr & env)
{
	if (root == nullptr)
		return nullptr;

	std::string token = root->getToken();
	Obj_ptr obj;
	bool numberFlag = true, rationalFlag = false, realFlag = false, idenFlag = true;

	checkToken(token, numberFlag, rationalFlag, realFlag, idenFlag);

	//----number-----
	if (numberFlag)
	{
		//----integer-----
		if (!rationalFlag && !realFlag)
			obj = Obj_ptr( new IntegerObj( bigInteger(token) ) );
		//----rational----
		else if (rationalFlag)
			obj = Obj_ptr( new RationalObj( bigRational(token) ) );
		//----real--------
		else
			obj = Obj_ptr( new RealObj( bigReal(token) ) );
		return obj;
	}

	//----identifier----
	else if (idenFlag)
	{
		obj = findIden(env, token);
		if (obj.get() == nullptr)
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
				obj = Obj_ptr( new CharObj('\n') );
			else if (token=="#\\space")
				obj = Obj_ptr( new CharObj(' ') );
			else if (token=="#\\tab")
				obj = Obj_ptr( new CharObj('\t') );
			else
				obj = Obj_ptr( new CharObj(token[2]) );

			return obj;
		}
		//----bool-----
		else
		{
			
			if (token[1]=='t')
				obj = Obj_ptr( new BoolObj(true) );
			else
				obj = Obj_ptr( new BoolObj(false) );

			return obj;
		}
	}

	//----string-------
	else if (token[0]=='\"')
	{
		obj = Obj_ptr( new StringObj(token.substr(1, token.size()-2)) );
		return obj;
	}

	//----symbol-------
	else if (token[0]=='\'')
	{
		ParseTree_ptr context = root->getSon();

		if (token=="\'()") // list
		{
			obj = makeList(context);
			return obj;
		}
		else	//	symbol
		{
			obj = Obj_ptr( new SymbolObj( context->getToken() ) );
			return obj;
		}
	}

	//----syntax&procedure----
	else if (token=="()")
	{
		ParseTree_ptr name = root->getSon();

		if (name == nullptr)
			throw syntaxError("missing expression");

		std::string iden = name->getToken();

		//--------syntax------------
		if (builtInSyntax.find(iden) != builtInSyntax.end())
		{
			return evaluateSyntax(iden, name->getBrother(), env);
		}
		
		//--------procedure---------

		Obj_ptr obj = findIden(env, iden);

			//--------procedure object----
			if (iden == "()")
			{
				obj = evaluate(name, env);
				if (obj->Type != Procedure)
					throw syntaxError("not a procedure");
			}


		if (obj==nullptr)
		{
			if (builtInProcedure.find(iden) == builtInProcedure.end())
				throw syntaxError("Undefined identifier: " + iden);
		}
		else if (obj->Type != Procedure)
			throw syntaxError("\'" + iden + "\' is not a procedure");

		ParseTree_ptr para = name->getBrother();
		Para_ptr head=nullptr, tail=nullptr;

		while (para!=nullptr)
		{
			if (head==nullptr)
			{
				head = Para_ptr( new Parameters(evaluate(para, env)) );
				tail = head;
			}
			else
			{
				tail->next = Para_ptr( new Parameters(evaluate(para, env)) );
				tail = tail->next;
			}

			para = para->getBrother();
		}

		if (obj)	//define by user
		{
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

Obj_ptr findIden(env_ptr env, const std::string &name)
{
	Obj_ptr obj;
	while (env!=nullptr)
	{
		obj = env->FindObj(name);
		if (obj!=nullptr)
			return obj;
		env = env->next;
	}
	return nullptr;
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

Obj_ptr makeList(const ParseTree_ptr & tree)
{
	Obj_ptr obj;

	if (tree==nullptr)
	{
		obj = Obj_ptr( new PairObj(nullptr, nullptr) );
		return obj;
	}

	Obj_ptr o1( new SymbolObj(tree->getToken()) );

	obj = Obj_ptr( new PairObj(o1, makeList(tree->getBrother())) );
	return obj;
}

