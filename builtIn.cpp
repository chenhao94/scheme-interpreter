/**
 * 	process the built-in syntaxs and procedures
 */

#include "builtIn.h"

ObjectSet ListCheckVisitedSet;

Obj_ptr evaluateBuiltInProcedure(const std::string &name, const Para_ptr &para, env_ptr & env)
{
	if (name=="+")
	{
		Para_ptr now = para, last;
		Obj_ptr ans;

		if (now == nullptr)
			return Obj_ptr( new IntegerObj(0) );

		if (now->obj->Type != Number)
			throw syntaxError("unexpected type");

		ans = now->obj;
		last = now;
		now = now->next;

		while (now != nullptr)
		{
			ans = Add(ans, now->obj);
			last = now;
			now = now->next;
		}

		return ans;
	}
	else if (name=="-")
	{
		Para_ptr now = para, last;
		Obj_ptr ans(new IntegerObj(0));

		if (now == nullptr)
			throw syntaxError("at least one arugument");

		if (now->obj->Type != Number)
			throw syntaxError("unexpected type");

		if (now->next == nullptr)
			return Subtract(ans, now->obj);

		ans = now->obj;
		last = now;
		now = now->next;

		while (now != nullptr)
		{
			ans = Subtract(ans, now->obj);
			last = now;
			now = now->next;
		}

		return ans;
	}
	else if (name=="*")
	{
		Para_ptr now = para, last;
		Obj_ptr ans;

		if (now == nullptr)
			return Obj_ptr( new IntegerObj(1) );

		if (now->obj->Type != Number)
			throw syntaxError("unexpected type");

		ans = now->obj;
		last = now;
		now = now->next;

		while (now != nullptr)
		{
			ans = Multiply(ans, now->obj);
			last = now;
			now = now->next;
		}

		return ans;
	}
	else if (name=="/")
	{
		Para_ptr now = para, last;
		Obj_ptr ans(new IntegerObj(1));

		if (now == nullptr)
			throw syntaxError("at least one arugument");

		if (now->obj->Type != Number)
			throw syntaxError("unexpected type");

		if (now->next == nullptr)
			return Divide(ans, now->obj);

		ans = now->obj;
		last = now;
		now = now->next;

		while (now != nullptr)
		{
			ans = Divide(ans, now->obj);
			last = now;
			now = now->next;
		}

		return ans;
	}
	else if (name=="quotient")
	{
		Para_ptr para1 = para, para2;
		Obj_ptr ans;

		if (para1 == nullptr || para1->next == nullptr || para1->next->next != nullptr)
			throw syntaxError("quotient: expect 2 arguments");

		para2 = para1->next;
		ans = Quotient(para1->obj, para2->obj);

		return ans;
	}
	else if (name=="modulo")
	{
		Para_ptr para1 = para, para2;
		Obj_ptr ans;

		if (para1 == nullptr || para1->next == nullptr || para1->next->next != nullptr)
			throw syntaxError("quotient: expect 2 arguments");

		para2 = para1->next;
		ans = Modulo(para1->obj, para2->obj);

		return ans;
	}
	else if (name=="remainder")
	{
		Para_ptr para1 = para, para2;
		Obj_ptr ans;

		if (para1 == nullptr || para1->next == nullptr || para1->next->next != nullptr)
			throw syntaxError("quotient: expect 2 arguments");

		para2 = para1->next;
		ans = Remainder(para1->obj, para2->obj);

		return ans;
	}
	else if (name=="gcd")
	{
		Para_ptr now = para, last;
		Obj_ptr ans;

		if (now == nullptr)
			return Obj_ptr( new IntegerObj(0) );

		if (now->obj->Type != Number)
			throw syntaxError("unexpected type");

		ans = now->obj;
		last = now;
		now = now->next;

		while (now != nullptr)
		{
			ans = GCD(ans, now->obj);
			last = now;
			now = now->next;
		}

		return ans;
	}
	else if (name=="lcm")
	{
		Para_ptr now = para, last;
		Obj_ptr ans;

		if (now == nullptr)
			return Obj_ptr( new IntegerObj(1) );

		if (now->obj->Type != Number)
			throw syntaxError("unexpected type");

		ans = now->obj;
		last = now;
		now = now->next;

		while (now != nullptr)
		{
			ans = LCM(ans, now->obj);
			last = now;
			now = now->next;
		}

		return ans;
	}
	else if (name=="=")
	{
		if (para == nullptr)
			throw syntaxError("at least two aruguments");

		Para_ptr now = para->next, last = para;

		if (now == nullptr)
			throw syntaxError("at least two aruguments");

		bool value = true;
		
		while (now != nullptr)
		{
			if (now->obj->Type!=Number)
				throw syntaxError("expect numbers here");
			value = value && equal(last->obj, now->obj);
			last = now;
			now = now->next;
		}

		Bool_ptr ptr( new BoolObj(value) );
		return ptr;
	}
	else if (name=="<")
	{
		if (para == nullptr)
			throw syntaxError("at least two aruguments");

		Para_ptr now = para->next, last = para;

		if (now == nullptr)
			throw syntaxError("at least two aruguments");

		bool value = true;
		
		while (now != nullptr)
		{
			value = value && lessThan(last->obj, now->obj);
			last = now;
			now = now->next;
		}

		Bool_ptr ptr( new BoolObj(value) );
		return ptr;
	}
	else if (name==">")
	{
		if (para == nullptr)
			throw syntaxError("at least two aruguments");

		Para_ptr now = para->next, last = para;

		if (now == nullptr)
			throw syntaxError("at least two aruguments");

		bool value = true;
		
		while (now != nullptr)
		{
			value = value && greaterThan(last->obj, now->obj);
			last = now;
			now = now->next;
		}

		Bool_ptr ptr( new BoolObj(value) );
		return ptr;
	}
	else if (name=="<=")
	{
		if (para == nullptr)
			throw syntaxError("at least two aruguments");

		Para_ptr now = para->next, last = para;

		if (now == nullptr)
			throw syntaxError("at least two aruguments");

		bool value = true;
		
		while (now != nullptr)
		{
			value = value && lessEq(last->obj, now->obj);
			last = now;
			now = now->next;
		}

		Bool_ptr ptr( new BoolObj(value) );
		return ptr;
	}
	else if (name==">=")
	{
		if (para == nullptr)
			throw syntaxError("at least two aruguments");

		Para_ptr now = para->next, last = para;

		if (now == nullptr)
			throw syntaxError("at least two aruguments");

		bool value = true;
		
		while (now != nullptr)
		{
			value = value && greaterEq(last->obj, now->obj);
			last = now;
			now = now->next;
		}

		Bool_ptr ptr( new BoolObj(value) );
		return ptr;
	}
	else if (name=="string=?")
	{
		if (para == nullptr)
			throw syntaxError("at least two aruguments");

		Para_ptr now = para->next, last = para;

		if (now == nullptr)
			throw syntaxError("at least two aruguments");

		bool value = true;
		
		while (now != nullptr)
		{
			if (now->obj->Type!=String)
				throw syntaxError("expect strings here");
			value = value && equal(last->obj, now->obj);
			last = now;
			now = now->next;
		}

		Bool_ptr ptr( new BoolObj(value) );
		return ptr;
	}
	else if (name=="string<?")
	{
		if (para == nullptr)
			throw syntaxError("at least two aruguments");

		Para_ptr now = para->next, last = para;

		if (now == nullptr)
			throw syntaxError("at least two aruguments");

		bool value = true;
		
		while (now != nullptr)
		{
			if (now->obj->Type!=String)
				throw syntaxError("expect strings here");
			value = value && lessThan(last->obj, now->obj);
			last = now;
			now = now->next;
		}

		Bool_ptr ptr( new BoolObj(value) );
		return ptr;
	}
	else if (name=="string>?")
	{
		if (para == nullptr)
			throw syntaxError("at least two aruguments");

		Para_ptr now = para->next, last = para;

		if (now == nullptr)
			throw syntaxError("at least two aruguments");

		bool value = true;
		
		while (now != nullptr)
		{
			if (now->obj->Type!=String)
				throw syntaxError("expect strings here");
			value = value && greaterThan(last->obj, now->obj);
			last = now;
			now = now->next;
		}

		Bool_ptr ptr( new BoolObj(value) );
		return ptr;
	}
	else if (name=="string<=?")
	{
		if (para == nullptr)
			throw syntaxError("at least two aruguments");

		Para_ptr now = para->next, last = para;

		if (now == nullptr)
			throw syntaxError("at least two aruguments");

		bool value = true;
		
		while (now != nullptr)
		{
			if (now->obj->Type!=String)
				throw syntaxError("expect strings here");
			value = value && lessEq(last->obj, now->obj);
			last = now;
			now = now->next;
		}

		Bool_ptr ptr( new BoolObj(value) );
		return ptr;
	}
	else if (name=="string>=?")
	{
		if (para == nullptr)
			throw syntaxError("at least two aruguments");

		Para_ptr now = para->next, last = para;

		if (now == nullptr)
			throw syntaxError("at least two aruguments");

		bool value = true;
		
		while (now != nullptr)
		{
			if (now->obj->Type!=String)
				throw syntaxError("expect strings here");
			value = value && greaterEq(last->obj, now->obj);
			last = now;
			now = now->next;
		}

		Bool_ptr ptr( new BoolObj(value) );
		return ptr;
	}
	else if (name=="max")
	{
		Para_ptr now = para;
		Obj_ptr ans;

		if (now == nullptr)
			throw syntaxError("at least one arugument");

		if (now->obj->Type != Number)
			throw syntaxError("unexpected type");

		ans = now->obj;
		now = now->next;

		while (now != nullptr)
		{
			if ( greaterThan(now->obj, ans) )
				ans = now->obj;
			now = now->next;
		}

		return ans;
	}
	else if (name=="min")
	{
		Para_ptr now = para;
		Obj_ptr ans;

		if (now == nullptr)
			throw syntaxError("at least one arugument");

		if (now->obj->Type != Number)
			throw syntaxError("unexpected type");

		ans = now->obj;
		now = now->next;

		while (now != nullptr)
		{
			if ( lessThan(now->obj, ans) )
				ans = now->obj;
			now = now->next;
		}

		return ans;
	}
	else if (name=="exact->inexact")
	{
		//W.T.F.
	}
	else if (name=="inexact->exact")
	{
		//W.T.F.
	}
	else if (name=="symbol?")
	{
		Para_ptr now = para;
		if (para == nullptr || para->next != nullptr)
			throw syntaxError("symbol?: expect 1 argument");
		Obj_ptr obj = para->obj;
		
		return Obj_ptr( new BoolObj(obj->Type == Symbol) );
	}
	else if (name=="number?")
	{
		Para_ptr now = para;
		if (para == nullptr || para->next != nullptr)
			throw syntaxError("number?: expect 1 argument");
		Obj_ptr obj = para->obj;
		
		return Obj_ptr( new BoolObj(obj->Type == Number) );
	}
	else if (name=="boolean?")
	{
		Para_ptr now = para;
		if (para == nullptr || para->next != nullptr)
			throw syntaxError("boolean?: expect 1 argument");
		Obj_ptr obj = para->obj;
		
		return Obj_ptr( new BoolObj(obj->Type == Bool) );
	}
	else if (name=="list?")
	{
		Para_ptr now = para;
		if (para == nullptr || para->next != nullptr)
			throw syntaxError("list?: expect 1 argument");
		Obj_ptr obj = para->obj;
		
		ListCheckVisitedSet.clear();
		return Obj_ptr( new BoolObj( isList(obj) ) );
	}
	else if (name=="string?")
	{
		Para_ptr now = para;
		if (para == nullptr || para->next != nullptr)
			throw syntaxError("string?: expect 1 argument");
		Obj_ptr obj = para->obj;
		
		return Obj_ptr( new BoolObj(obj->Type == String) );
	}
	else if (name=="cons")
	{
		Para_ptr para1 = para, para2;
		
		if (para1 == nullptr || para1->next == nullptr || para1->next->next != nullptr)
			throw syntaxError("mcons: expect 2 arguments");

		para2 = para->next;
		Pair_ptr ptr( new PairObj(para1->obj, para2->obj) );
		return ptr;
	}
	else if (name=="car")
	{
		if (para == nullptr || para->next != nullptr)
			throw syntaxError("mcar: expect 1 argument");
		
		Object* obj = para->obj.get();
		if (obj->Type != Pair)
			throw syntaxError("unexpected type");

		return static_cast<PairObj*>(obj)->getCar();
	}
	else if (name=="cdr")
	{
		if (para == nullptr || para->next != nullptr)
			throw syntaxError("mcar: expect 1 argument");
		
		Object *obj = para->obj.get();
		if (obj->Type != Pair)
			throw syntaxError("unexpected type");

		return static_cast<PairObj*>(obj)->getCdr();
	}
	else if (name=="eq?" || name=="eqv?")
	{
		Para_ptr para1 = para, para2;
		
		if (para1 == nullptr || para1->next == nullptr || para1->next->next != nullptr)
			throw syntaxError("eq? or eqv?: expect 2 arguments");

		para2 = para->next;
		Obj_ptr obj1 = para1->obj, obj2 = para2->obj;
		bool value;

		if (obj1->Type != obj2->Type)
			value = false;
		else
			value = equal(obj1, obj2);

		Bool_ptr ptr( new BoolObj(value) );

		return ptr;
	}
	else if (name=="equal?")
	{
		Para_ptr para1 = para, para2;
		
		if (para1 == nullptr || para1->next == nullptr || para1->next->next != nullptr)
			throw syntaxError("eq? or eqv?: expect 2 arguments");

		para2 = para->next;
		Obj_ptr obj1 = para1->obj, obj2 = para2->obj;
		bool value;

		if (obj1->Type != obj2->Type)
			value = false;
		else if (obj1->Type != Pair)
			value = equal(obj1, obj2);
		else
			value = (obj1->ExternalRep() == obj2->ExternalRep());

		Bool_ptr ptr( new BoolObj(value) );

		return ptr;
	}
	else if (name=="force")
	{
		if (para == nullptr || para->next != nullptr)
			throw syntaxError("force: expect 1 argument");
		
		Obj_ptr obj = para->obj;
		if (obj->Type != Promise)
			return obj;

		PromiseObj* _promise = static_cast<PromiseObj*>(obj.get());

		if (_promise->getAnswer() != nullptr)
			return _promise->getAnswer();

		return _promise->getAnswer() = evaluate(_promise->getBody(), _promise->getEnv());
	}
}

Obj_ptr evaluateSyntax(const std::string &name, const ParseTree_ptr &tree, env_ptr &env)
{
	if (name=="if")
	{
		ParseTree_ptr test = tree, consequence, alternate;
		
		if (test == nullptr)
			goto badIfSyntax;
		consequence = test->getBrother();
		if (consequence == nullptr)
			goto badIfSyntax;
		alternate = consequence->getBrother();
		if (alternate != nullptr && alternate->getBrother() != nullptr)
			goto badIfSyntax;

		if (false)
		{
			badIfSyntax:
			throw syntaxError("if: bad syntax");
		}

		Obj_ptr testObj = evaluate(test, env);

		if (!( testObj->Type==Bool && static_cast<BoolObj*>(testObj.get())->getValue() == false ))
			return evaluate(consequence, env);
		if (alternate == nullptr)
			return nullptr;
		return evaluate(alternate, env);
	}
	else if (name=="cond")
	{
		Obj_ptr obj;
		ParseTree_ptr clause = tree;

		while (clause)
		{
			obj = evaluateCondClause(clause, env);
			if (obj != nullptr)
				return obj;
			clause = clause->getBrother();
		}

		return nullptr;
	}
	else if (name=="case")
	{
		Obj_ptr obj, stdObj;
		ParseTree_ptr clause = tree;

		if (tree == nullptr)
			throw syntaxError("bad syntax: case");
		stdObj = evaluate(clause, env);
		clause = clause->getBrother();

		while (clause)
		{
			obj = evaluateCaseClause(stdObj, clause, env);
			if (obj != nullptr)
				return obj;
			clause = clause->getBrother();
		}

		return nullptr;
	}
	else if (name=="define")
	{
		std::string iden = tree->getToken(), varName;
		bool idenFlag, tmp1, tmp2, tmp3;

		if (iden == "()")
		{
			if (tree->getBrother() == nullptr)
				throw syntaxError("define: bad syntax(missing expression)");

			ParseTree_ptr vars = tree->getSon();
			Arg_ptr now, last = nullptr, head = nullptr;
			Procedure_ptr proc;
			if (vars == nullptr)
				throw syntaxError("define: bad syntax in: ()");

			iden = vars->getToken();
			checkToken(iden, tmp1, tmp2, tmp3, idenFlag);
			if (!idenFlag)
				throw syntaxError("define: bad identifier: " + iden);

			vars = vars->getBrother();
			while (vars)
			{
				varName = vars->getToken();
				if (varName == ".")
				{
					vars = vars->getBrother();
					if (vars == nullptr || vars->getBrother() != nullptr)
						throw syntaxError("illegal use of \'.\'");
					varName = vars->getToken();

					checkToken(varName, tmp1, tmp2, tmp3, idenFlag);
					if (!idenFlag)
						throw syntaxError("define: bad identifier: " + iden);

					now = Arg_ptr( new Arguments(varName, true) );
					if (last != nullptr)
						last->next = now;
					else
						head = now;
					last = now;
				}
				else
				{
					checkToken(varName, tmp1, tmp2, tmp3, idenFlag);
					if (!idenFlag)
						throw syntaxError("define: bad identifier: " + iden);

					now = Arg_ptr( new Arguments(varName, false) );
					if (last != nullptr)
						last->next = now;
					else
						head = now;
					last = now;
				}

				vars = vars->getBrother();
			}

			proc = Procedure_ptr( new ProcedureObj(head, tree->getBrother(), env) );
			env->DefineObj(iden, proc);
		}
		else
		{
			if (tree->getSon() != nullptr || tree->getBrother() == nullptr || tree->getBrother()->getBrother() != nullptr)
				throw syntaxError("define: bad syntax(missing expression or multiple expression)");

			checkToken(iden, tmp1, tmp2, tmp3, idenFlag);
			if (!idenFlag)
				throw syntaxError("define: bad identifier: " + iden);

			env->DefineObj( iden, evaluate(tree->getBrother(), env) );
		}
		return nullptr;
	}
	else if (name=="set!")
	{
		std::string iden = tree->getToken();
		if (tree->getSon() != nullptr || tree->getBrother() == nullptr || tree->getBrother()->getBrother() != nullptr)
			throw syntaxError("set!: bad syntax(missing expression or multiple expression)");

		bool idenFlag, tmp1, tmp2, tmp3;
		checkToken(iden, tmp1, tmp2, tmp3, idenFlag);
		if (!idenFlag)
			throw syntaxError("set!: bad identifier: " + iden);

		env_ptr envTmp = env;
		while (envTmp)
		{
			if (envTmp->FindObj(iden) != nullptr)
			{
				envTmp->DefineObj( iden, evaluate(tree->getBrother(), env) );
				return nullptr;
			}
			envTmp = envTmp->next;
		}

		throw syntaxError("undefined variable: " + iden);
	}
	else if (name=="lambda")
	{
		std::string iden = tree->getToken(), varName;
		bool idenFlag, tmp1, tmp2, tmp3;
		ParseTree_ptr vars;
		Arg_ptr now, last = nullptr, head = nullptr;
		Procedure_ptr proc;

		if (iden == "()")
		{
			vars = tree->getSon();
			while (vars)
			{
				varName = vars->getToken();
				if (varName == ".")
				{
					vars = vars->getBrother();
					if (vars == nullptr || vars->getBrother() != nullptr)
						throw syntaxError("illegal use of \'.\'");
					varName = vars->getToken();

					checkToken(varName, tmp1, tmp2, tmp3, idenFlag);
					if (!idenFlag)
						throw syntaxError("define: bad identifier: " + iden);

					now = Arg_ptr( new Arguments(varName, true) );
					if (last != nullptr)
						last->next = now;
					else
						head = now;
					last = now;
				}
				else
				{
					checkToken(varName, tmp1, tmp2, tmp3, idenFlag);
					if (!idenFlag)
						throw syntaxError("define: bad identifier: " + iden);

					now = Arg_ptr( new Arguments(varName, false) );
					if (last != nullptr)
						last->next = now;
					else
						head = now;
					last = now;
				}

				vars = vars->getBrother();
			}

			proc = Procedure_ptr( new ProcedureObj(head, tree->getBrother(), env) );
		}
		else
		{
			vars = tree;
			varName = iden;
			checkToken(varName, tmp1, tmp2, tmp3, idenFlag);
			if (!idenFlag)
				throw syntaxError("define: bad identifier: " + iden);

			head = Arg_ptr( new Arguments(varName, true) );
			proc = Procedure_ptr( new ProcedureObj(head, tree->getBrother(), env) );
		}

		return proc;
	}
	else if (name=="quote")
	{
		return Quote(tree);
	}
	else if (name=="quasiquote")
	{
		return Quasiquote(tree, env);
	}
	else if (name=="and")
	{
		ParseTree_ptr now = tree;
		Obj_ptr ans;

		if (now == nullptr)
			return Obj_ptr( new BoolObj(true) );

		ans = evaluate(now, env);
		now = now->getBrother();

		while (now != nullptr)
		{
			if ( ans->Type==Bool && static_cast<BoolObj*>(ans.get())->getValue() == false )
				return ans;
			ans = evaluate(now, env);
			now = now->getBrother();
		}

		return ans;
	}
	else if (name=="or")
	{
		ParseTree_ptr now = tree;
		Obj_ptr ans;

		if (now == nullptr)
			return Obj_ptr( new BoolObj(false) );

		ans = evaluate(tree, env);
		now = now->getBrother();

		while (now != nullptr)
		{
			if ( !(ans->Type==Bool && static_cast<BoolObj*>(ans.get())->getValue() == false) )
				return ans;
			ans = evaluate(now, env);
			now = now->getBrother();
		}

		return ans;
	}
	else if (name=="delay")
	{
		ParseTree_ptr now = tree;
		if (now == nullptr || now->getBrother() != nullptr)
			throw syntaxError("delay: expect 1 argument here");

		return Promise_ptr(new PromiseObj(now, env));
	}
	else if (name=="append")
	{
		ParseTree_ptr now = tree;
		return Append(now, env);
	}
}

Obj_ptr evaluateCondClause(const ParseTree_ptr & clause, env_ptr & env)
{
	ParseTree_ptr test = clause->getSon();
	Obj_ptr obj;

	if (test == nullptr)
		throw syntaxError("missing expression");

	ParseTree_ptr expression = test->getBrother();

	if (test->getToken() == "else")
		goto Execute;

	obj = evaluate(test, env);
	if (!( obj->Type==Bool && static_cast<BoolObj*>(obj.get())->getValue() == false ))
	{
		if ( expression->getToken() == "=>" )  // ----- => -------
		{
			expression = expression->getBrother();
			if ( expression == nullptr || expression->getBrother() != nullptr )
				throw syntaxError("illegal use of: \'=>\'");

			Obj_ptr func(evaluate(expression, env));
			Para_ptr para( new Parameters(obj) );
			return evaluateUserDefined(func, para, env);
		}
		else
		{
			Execute:
			obj = nullptr;
			while (expression != nullptr)
			{
				obj = evaluate(expression, env);
				expression = expression->getBrother();
			}
			return obj;
		}
	}
	return nullptr;
}

Obj_ptr evaluateCaseClause(const Obj_ptr & key, const ParseTree_ptr & clause, env_ptr & env)
{
	ParseTree_ptr dat = clause->getSon();
	Obj_ptr obj;

	if (dat == nullptr)
		throw syntaxError("missing expression");

	ParseTree_ptr expression = dat->getBrother();

	if (dat->getToken() == "else")
		goto Execute;
	if (dat->getToken() != "()")
		throw syntaxError("case: bad syntax");

	dat = dat->getSon();
	while (dat)
	{
		if ( equal(key, evaluate(dat, env)) )
			goto Execute;
		dat = dat->getBrother();
	}

	if (false)
	{
		Execute:
		obj = nullptr;
		while (expression != nullptr)
		{
			obj = evaluate(expression, env);
			expression = expression->getBrother();
		}
		return obj;
	}
	return nullptr;
}

bool isList( const Obj_ptr & obj )
{
	if (obj->Type != Pair)
		return false;
	if (emptyPair(obj))
		return true;
	if ( ListCheckVisitedSet.find( obj ) !=
			ListCheckVisitedSet.end() )
		return false;

	ListCheckVisitedSet.insert( obj );
	return isList( static_cast<PairObj*>(obj.get())->getCdr() );
}

Obj_ptr Append( const ParseTree_ptr & tree, env_ptr & env)
{
	if (tree == nullptr)
		return Pair_ptr( new PairObj(nullptr, nullptr) );

	Obj_ptr obj = evaluate(tree, env);

	if (tree->getBrother() == nullptr)
		return obj;

	//--------check obj and find the last empty list------
	if (obj->Type != Pair)
		throw syntaxError("unexpected type");
	if (emptyPair(obj))	
		return Append(tree->getBrother(), env);

	Obj_ptr copyObj( new PairObj( *static_cast<PairObj*>(obj.get()) ) ), last, copyLast, copyTmp;
	last = copyLast = copyObj;
	ObjectSet AppendCheckSet({});

	while ( !emptyPair(static_cast<PairObj*>(last.get())->obj2) )
	{
		if ( AppendCheckSet.find(last) !=
				AppendCheckSet.end())
			throw syntaxError("unexpected type");

		last = static_cast<PairObj*>(last.get())->obj2;

		if (last->Type != Pair)
			throw syntaxError("unexpected type");

		copyTmp = Obj_ptr( new PairObj(*static_cast<PairObj*>(last.get())) );
		static_cast<PairObj*>(copyLast.get())->obj2 = copyTmp;
		copyLast = copyTmp;
		last = copyLast;
	}
	//----------------------------------------------------

	static_cast<PairObj*>(last.get())->obj2 = Append(tree->getBrother(), env);
	
	return copyObj;
}

