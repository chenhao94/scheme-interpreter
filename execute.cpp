/**
 *	give a parse tree, execute it and give the output
 */

#include "execute.h"
#include <cctype>

builtInSet builtInSyntax({ "if","cond","case","else","define","set!","lambda","quote","quasiquote","and","or","delay" });

builtInSet builtInProcedure({"+","-","*","/","=","<",">","<=",">=","modulo","gcd","lcm","quotient","remainder","max","min","exact->inexact","inexact->exact","symbol?","string?","number?","boolean?","list?","string=?","string<?","string>?","string<=?","string>=?","cons","car","cdr","eq?","eqv?","equal?","force"});

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
		return descend(obj);
	}

	//----comma--------
	else if (token[0]==',')
		throw syntaxError("unexpected comma here");

	//----identifier----
	else if (idenFlag)
	{
		obj = findIden(env, token);
		if (obj.get() == nullptr)
		{
			if (builtInProcedure.find(token) == builtInProcedure.end())
				throw syntaxError("Undefined identifier: " + token);
			Arg_ptr nullArg(nullptr);
			ParseTree_ptr nullTree(nullptr);
			env_ptr nullEnv(nullptr);
			obj = Obj_ptr(new ProcedureObj(nullArg, nullTree, nullEnv, true, token));
		}
		return descend(obj);
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

			return descend(obj);
		}
		//----bool-----
		else
		{
			
			if (token[1]=='t')
				obj = Obj_ptr( new BoolObj(true) );
			else
				obj = Obj_ptr( new BoolObj(false) );

			return descend(obj);
		}
	}

	//----string-------
	else if (token[0]=='\"')
	{
		obj = Obj_ptr( new StringObj(token.substr(1, token.size()-2)) );
		return descend(obj);
	}

	//----quote-------
	else if (token=="\'")
	{
		obj = Quote(root);
		return descend(obj);
	}

	else if (token=="`")
	{
		obj = Quasiquote(root, env);
		return descend(obj);
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
			return descend(evaluateSyntax(iden, name->getBrother(), env));
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
			return descend(evaluateUserDefined(obj, head, env));
		}
		else		// built-in
		{
			return descend(evaluateBuiltInProcedure(iden, head, env));
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
			return descend(obj);
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

	if (!isdigit(token[0]) && token[0]!='.' && token[0]!='-')
		numberFlag = false;
	if (token[token_size-1]=='/')
		numberFlag = false;
	if (token == "." || token=="-.")
		numberFlag = false;

	if (numberFlag)
	{
		for (int i=0; i<token_size; ++i)
		{
			if (!isdigit(token[i]) && token[i]!='.' && token[i]!='/' && (token[i]!='-' || i!=0))
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

Obj_ptr quoteMakeList(const ParseTree_ptr & tree)
{
	Obj_ptr obj;

	if (tree==nullptr)
	{
		obj = Obj_ptr( new PairObj(nullptr, nullptr) );
		return descend(obj);
	}

	std::string token = tree->getToken();

	if (token==".")
	{
		if (tree->getBrother() == nullptr || tree->getBrother()->getBrother() != nullptr)
			throw syntaxError("illegal use: \'.\'");
		return Quote(tree->getBrother());
	}
	else
	{
		Obj_ptr o1( Quote(tree) );
		obj = Obj_ptr( new PairObj(o1, quoteMakeList(tree->getBrother())) );
	}
	return descend(obj);
}

Obj_ptr quasiquoteMakeList(const ParseTree_ptr & tree, env_ptr & env)
{
	Obj_ptr obj;

	if (tree==nullptr)
	{
		obj = Obj_ptr( new PairObj(nullptr, nullptr) );
		return descend(obj);
	}

	std::string token = tree->getToken();

	if (token==".")
	{
		if (tree->getBrother() == nullptr || tree->getBrother()->getBrother() != nullptr)
			throw syntaxError("illegal use: \'.\'");
		return Quasiquote(tree->getBrother(), env);
	}
	else
	{
		Obj_ptr o1( Quasiquote(tree, env) );
		obj = Obj_ptr( new PairObj(o1, quasiquoteMakeList(tree->getBrother(), env)) );
	}
	return descend(obj);
}

Obj_ptr Quote(const ParseTree_ptr &root)
{
	if (root == nullptr)
		return nullptr;
	std::string token = root->getToken();
	bool numberFlag = true, rationalFlag = false, realFlag = false, idenFlag = true;

	checkToken(token, numberFlag, rationalFlag, realFlag, idenFlag);

	if ( numberFlag==true )		// numerical constant
	{
		//----integer-----
		if (!rationalFlag && !realFlag)
			return Obj_ptr( new IntegerObj( bigInteger(token) ) );
		//----rational----
		else if (rationalFlag)
			return Obj_ptr( new RationalObj( bigRational(token) ) );
		//----real--------
		else
			return Obj_ptr( new RealObj( bigReal(token) ) );
	}
	else if (token[0]=='\"')	// string constant
		return Obj_ptr( new StringObj(token.substr(1, token.size()-2)) );
	else if (token[0]=='#')
	{
		// character constant
		if (token[1]=='\\')
		{
			if (token=="#\\newline")
				return Obj_ptr( new CharObj('\n') );
			else if (token=="#\\space")
				return Obj_ptr( new CharObj(' ') );
			else if (token=="#\\tab")
				return Obj_ptr( new CharObj('\t') );
			else
				return Obj_ptr( new CharObj(token[2]) );
		}
		// boolean constant
		else
		{
			if (token[1]=='t')
				return Obj_ptr( new BoolObj(true) );
			else
				return Obj_ptr( new BoolObj(false) );
		}
	}
	else if (token=="\'")
		return Quote(root->getSon());
	else if (token=="`")
		throw syntaxError("unexpected \'`\' here");	
	else if (token=="()")
		return quoteMakeList(root->getSon());
	else if (token[0]==',')
		throw syntaxError("unexpected comma here");	
	else
		return Obj_ptr( new SymbolObj( token ) );
}

Obj_ptr Quasiquote(const ParseTree_ptr &root, env_ptr & env)
{
	if (root == nullptr)
		return nullptr;
	std::string token = root->getToken();
	bool numberFlag = true, rationalFlag = false, realFlag = false, idenFlag = true;

	checkToken(token, numberFlag, rationalFlag, realFlag, idenFlag);

	if ( numberFlag==true )		// numerical constant
	{
		//----integer-----
		if (!rationalFlag && !realFlag)
			return Obj_ptr( new IntegerObj( bigInteger(token) ) );
		//----rational----
		else if (rationalFlag)
			return Obj_ptr( new RationalObj( bigRational(token) ) );
		//----real--------
		else
			return Obj_ptr( new RealObj( bigReal(token) ) );
	}
	else if (token[0]=='\"')	// string constant
		return Obj_ptr( new StringObj(token.substr(1, token.size()-2)) );
	else if (token[0]=='#')
	{
		// character constant
		if (token[1]=='\\')
		{
			if (token=="#\\newline")
				return Obj_ptr( new CharObj('\n') );
			else if (token=="#\\space")
				return Obj_ptr( new CharObj(' ') );
			else if (token=="#\\tab")
				return Obj_ptr( new CharObj('\t') );
			else
				return Obj_ptr( new CharObj(token[2]) );
		}
		// boolean constant
		else
		{
			if (token[1]=='t')
				return Obj_ptr( new BoolObj(true) );
			else
				return Obj_ptr( new BoolObj(false) );
		}
	}
	else if (token=="\'")
		return Quote(root->getSon());
	else if (token=="`")
		return Quasiquote(root->getSon(), env);
	else if (token=="()")
		return quasiquoteMakeList(root->getSon(), env);
	else if (token==",")
		return evaluate(root->getSon(), env);
	else if (token==",@")
	{
		Obj_ptr obj(evaluate(root->getSon(), env));
		if (!isList(obj))
			throw syntaxError("there must be a list after \',@\'");	//W.T.F.
		throw syntaxError("sorry for no support of \',@\' now");
	}
	else
		return Obj_ptr( new SymbolObj( token ) );
}
