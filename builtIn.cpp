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

		if (now == NULL)
			throw syntaxError("at least one arugument");

		if (now->obj->Type != Number)
			throw syntaxError("unexpected type");

		ans = now->obj;
		last = now;
		now = now->next;

		while (now != NULL)
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

		if (now == NULL)
			throw syntaxError("at least one arugument");

		if (now->obj->Type != Number)
			throw syntaxError("unexpected type");

		ans = now->obj;
		last = now;
		now = now->next;

		while (now != NULL)
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

		if (now == NULL)
			throw syntaxError("at least one arugument");

		if (now->obj->Type != Number)
			throw syntaxError("unexpected type");

		ans = now->obj;
		last = now;
		now = now->next;

		while (now != NULL)
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

		if (now == NULL)
			throw syntaxError("at least one arugument");

		if (now->obj->Type != Number)
			throw syntaxError("unexpected type");

		ans = now->obj;
		last = now;
		now = now->next;

		while (now != NULL)
		{
			ans = Divide(ans, now->obj);
			last = now;
			now = now->next;
		}

		return ans;
	}
	else if (name=="<")
	{
		if (para == NULL)
			throw syntaxError("at least two aruguments");

		Para_ptr now = para->next, last = para;

		if (now == NULL)
			throw syntaxError("at least two aruguments");

		bool value = false;
		
		while (now != NULL)
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
		if (para == NULL)
			throw syntaxError("at least two aruguments");

		Para_ptr now = para->next, last = para;

		if (now == NULL)
			throw syntaxError("at least two aruguments");

		bool value = false;
		
		while (now != NULL)
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
		if (para == NULL)
			throw syntaxError("at least two aruguments");

		Para_ptr now = para->next, last = para;

		if (now == NULL)
			throw syntaxError("at least two aruguments");

		bool value = false;
		
		while (now != NULL)
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
		if (para == NULL)
			throw syntaxError("at least two aruguments");

		Para_ptr now = para->next, last = para;

		if (now == NULL)
			throw syntaxError("at least two aruguments");

		bool value = false;
		
		while (now != NULL)
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

		if (now == NULL)
			throw syntaxError("at least one arugument");

		if (now->obj->Type != Number)
			throw syntaxError("unexpected type");

		ans = now->obj;
		now = now->next;

		while (now != NULL)
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

		if (now == NULL)
			throw syntaxError("at least one arugument");

		if (now->obj->Type != Number)
			throw syntaxError("unexpected type");

		ans = now->obj;
		now = now->next;

		while (now != NULL)
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
		
		if (para1 == NULL || para1->next == NULL || para1->next->next != NULL)
			throw syntaxError("mcons: expect 2 arguments");

		para2 = para->next;
		Pair_ptr ptr( new PairObj(para1->obj, para2->obj) );
		return ptr;
	}
	else if (name=="car")
	{
		if (para == NULL || para->next != NULL)
			throw syntaxError("mcar: expect 1 argument");
		
		Object* obj = para->obj.get();
		if (obj->Type != Pair)
			throw("unexpected type");

		return static_cast<PairObj*>(obj)->getCar();
	}
	else if (name=="cdr")
	{
		if (para == NULL || para->next != NULL)
			throw syntaxError("mcar: expect 1 argument");
		
		Object *obj = para->obj.get();
		if (obj->Type != Pair)
			throw("unexpected type");

		return static_cast<PairObj*>(obj)->getCdr();
	}
	else if (name=="eq?" || name=="eqv?")
	{
		Para_ptr para1 = para, para2;
		
		if (para1 == NULL || para1->next == NULL || para1->next->next != NULL)
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
		
		if (para1 == NULL || para1->next == NULL || para1->next->next != NULL)
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
		
		if (test == NULL)
			goto badIfSyntax;
		consequence = test->getBrother();
		if (consequence == NULL)
			goto badIfSyntax;
		alternate = consequence->getBrother();
		if (alternate != NULL && alternate->getBrother() != NULL)
			goto badIfSyntax;

		if (false)
		{
			badIfSyntax:
			throw syntaxError("if: bad syntax");
		}

		Obj_ptr testObj = evaluate(test, env);

		if (!( testObj->Type==Bool && static_cast<BoolObj*>(testObj.get())->getValue() == false ))
			return evaluate(consequence, env);
		if (alternate == NULL)
			return NULL;
		return evaluate(alternate, env);
	}
	else if (name=="cond")
	{
		//W.T.F.
	}
	else if (name=="case")
	{
		//W.T.F.
	}
	else if (name=="else")
	{
		//W.T.F.
	}
	else if (name=="define")
	{
		std::string iden = tree->getToken();
		if (tree->getSon() != NULL || tree->getBrother() == NULL || tree->getBrother()->getBrother() != NULL)
			throw syntaxError("define: bad syntax(missing expression or multiple expression)");

		bool idenFlag, tmp1, tmp2, tmp3;
		checkToken(iden, tmp1, tmp2, tmp3, idenFlag);
		if (!idenFlag)
			throw syntaxError("define: bad identifier: " + iden);

		env->DefineObj( iden, evaluate(tree->getBrother(), env) );
		return NULL;
	}
	else if (name=="set!")
	{
		std::string iden = tree->getToken();
		if (tree->getSon() != NULL || tree->getBrother() == NULL || tree->getBrother()->getBrother() != NULL)
			throw syntaxError("set!: bad syntax(missing expression or multiple expression)");

		bool idenFlag, tmp1, tmp2, tmp3;
		checkToken(iden, tmp1, tmp2, tmp3, idenFlag);
		if (!idenFlag)
			throw syntaxError("set!: bad identifier: " + iden);

		env_ptr envTmp = env;
		while (envTmp)
		{
			if (envTmp->FindObj(iden) != NULL)
			{
				envTmp->DefineObj( iden, evaluate(tree->getBrother(), env) );
				return NULL;
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
