/**
 * 	process the built-in syntaxs and procedures
 */

#include "builtIn.h"

Obj_ptr evaluateBuiltIn(const std::string &name, const Para_ptr &para);
{
	if (name=="+")
	{
		//W.T.F.
	}
	else if (name=="-")
	{
		//W.T.F.
	}
	else if (name=="*")
	{
		//W.T.F.
	}
	else if (name=="/")
	{
		//W.T.F.
	}
	else if (name=="<")
	{
		Para_ptr para2 = para->next;

		if (para2==NULL || para2->next!=NULL)
			throw syntaxError("W.T.F.");

		if (para.Type!=number || para2.Type!=number)
			throw syntaxError("W.T.F.");

		bool value = false;
		//W.T.F.
		Bool_ptr ptr( new BoolObj(value) );
		return ptr;
	}
	else if (name==">")
	{
		//W.T.F.
	}
	else if (name=="<=")
	{
		//W.T.F.
	}
	else if (name==">=")
	{
		//W.T.F.
	}
	else if (name=="gcd")
	{
		//W.T.F.
	}
	else if (name=="max")
	{
		//W.T.F.
	}
	else if (name=="min")
	{
		//W.T.F.
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
		//W.T.F.
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
		//W.T.F.
	}
	else if (name=="car")
	{
		//W.T.F.
	}
	else if (name=="cdr")
	{
		//W.T.F.
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
	else if (name=="eq?")
	{
		//W.T.F.
	}
	else if (name=="eqv?")
	{
		//W.T.F.
	}
	else if (name=="equal?")
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
