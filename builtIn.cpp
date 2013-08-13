/**
 * 	process the built-in syntaxs and procedures
 */

#include "builtIn.h"

Obj_ptr evaluateBuiltInProcedure(const std::string &name, const Para_ptr &para, env_ptr & env)
{
	if (name=="+")
	{
		Para_ptr now = para, last;
		Obj_ptr ans;

		if (now == nullptr)
			throw syntaxError("at least one arugument");

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
		Obj_ptr ans;

		if (now == nullptr)
			throw syntaxError("at least one arugument");

		if (now->obj->Type != Number)
			throw syntaxError("unexpected type");

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
			throw syntaxError("at least one arugument");

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
		Obj_ptr ans;

		if (now == nullptr)
			throw syntaxError("at least one arugument");

		if (now->obj->Type != Number)
			throw syntaxError("unexpected type");

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
	else if (name=="gcd")
	{
		//W.T.F.
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
	else if (name=="cons")
	{
		Para_ptr para1 = para, para2;
		
		if (para1 == nullptr || para1->next == NULL || para1->next->next != NULL)
			throw syntaxError("mcons: expect 2 arguments");

		para2 = para->next;
		Pair_ptr ptr( new PairObj(para1->obj, para2->obj) );
		return ptr;
	}
	else if (name=="car")
	{
		if (para == nullptr || para->next != NULL)
			throw syntaxError("mcar: expect 1 argument");
		
		Object* obj = para->obj.get();
		if (obj->Type != Pair)
			throw("unexpected type");

		return static_cast<PairObj*>(obj)->getCar();
	}
	else if (name=="cdr")
	{
		if (para == nullptr || para->next != NULL)
			throw syntaxError("mcar: expect 1 argument");
		
		Object *obj = para->obj.get();
		if (obj->Type != Pair)
			throw("unexpected type");

		return static_cast<PairObj*>(obj)->getCdr();
	}
	else if (name=="eq?" || name=="eqv?")
	{
		Para_ptr para1 = para, para2;
		
		if (para1 == nullptr || para1->next == NULL || para1->next->next != NULL)
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
		
		if (para1 == nullptr || para1->next == NULL || para1->next->next != NULL)
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
		if (alternate != nullptr && alternate->getBrother() != NULL)
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
		std::string iden = tree->getToken();
		if (tree->getSon() != nullptr || tree->getBrother() == NULL || tree->getBrother()->getBrother() != NULL)
			throw syntaxError("define: bad syntax(missing expression or multiple expression)");

		bool idenFlag, tmp1, tmp2, tmp3;
		checkToken(iden, tmp1, tmp2, tmp3, idenFlag);
		if (!idenFlag)
			throw syntaxError("define: bad identifier: " + iden);

		env->DefineObj( iden, evaluate(tree->getBrother(), env) );
		return nullptr;
	}
	else if (name=="set!")
	{
		std::string iden = tree->getToken();
		if (tree->getSon() != nullptr || tree->getBrother() == NULL || tree->getBrother()->getBrother() != NULL)
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
		//W.T.F.
	}
	else if (name=="quote")
	{
		//W.T.F.
	}
	else if (name=="quasiquote")
	{
		//W.T.F.
	}
}

Obj_ptr evaluateCondClause(const ParseTree_ptr & clause, env_ptr & env)
{
	//W.T.F.
}

Obj_ptr evaluateCaseClause(const Obj_ptr & key, const ParseTree_ptr & clause, env_ptr & env)
{
	//W.T.F.
}


