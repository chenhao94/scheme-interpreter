/**
 * 	process the built-in syntaxs and procedures
 */

#include "builtIn.h"

Obj_ptr evaluateBuiltIn(const std::string &name, const Para_ptr &para);
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
			ans = ans + now->obj;
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
			ans = ans - now->obj;
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
			ans = ans * now->obj;
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
			ans = ans / now->obj;
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
			value = value && (last->obj < now->obj);
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
			value = value && (last->obj > now->obj);
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
			value = value && (last->obj <= now->obj);
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
			value = value && (last->obj >= now->obj);
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
			if (now->obj > ans)
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
			if (now->obj < ans)
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
	else if (name=="if")
	{
		Para_ptr test = para, consequence, alternate;
		
		if (para == NULL)
			goto badIfSyntax;
		consequence = para->next;
		if (consequence == NULL)
			goto badIfSyntax;
		alternate = consequence->next;
		if (alternate != NULL && alternate->next != NULL)
			goto badIfSyntax;

		if (false)
		{
			badIfSyntax:
			throw syntaxError("if: bad syntax");
		}

		if (!( test->obj->Type==Bool && static_cast<BoolObj*>(test->obj.get())->getValue() == false ))
			return consequence->obj;
		if (alternate == NULL)
			return NULL;
		return alternate->obj;
	}
	else if (name=="cond")
	{
		//W.T.F.
	}
	else if (name=="else")
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
		
		Obj_ptr obj = para->obj.get();
		if (obj->Type != Pair)
			throw("unexpected type");

		return static_cast<PairObj*>(obj)->getCar();
	}
	else if (name=="cdr")
	{
		if (para == NULL || para->next != NULL)
			throw syntaxError("mcar: expect 1 argument");
		
		Obj_ptr obj = para->obj.get();
		if (obj->Type != Pair)
			throw("unexpected type");

		return static_cast<PairObj*>(obj)->getCdr();
	}
	else if (name=="define")
	{
		//W.T.F.
	}
	else if (name=="set!")
	{
		//W.T.F.
	}
	else if (name=="lambda")
	{
		//W.T.F.
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
			value = (obj1 == obj2);

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
			value = (obj1 == obj2);
		else
		{
			//W.T.F.
		}

		Bool_ptr ptr( new BoolObj(value) );

		return ptr;
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
