/**
 * 	process the user defined procedures
 */

#include "userDefined.h"

Obj_ptr evaluateUserDefined( const Obj_ptr &func, const Para_ptr & para, env_ptr & env)
{
	if ( func->Type != Procedure )
		throw syntaxError("not a procedure");

	env_ptr newEnv( new Environment(env) );

	//-----check argument list------------
	Para_ptr lastPara( para );
	Arg_ptr lastArg( static_cast<ProcedureObj*>(func.get())->getArgHead() );
	
	while (lastArg)
	{
		if (lastArg->spaceDelimited == false)
		{
			if (lastPara == nullptr)
				throw syntaxError("lack of arguments");

			newEnv->DefineObj(lastArg->name, lastPara->obj);
		}
		else
		{
			newEnv->DefineObj(lastArg->name, makeList(lastPara));
			lastPara = nullptr;
			break;
		}

		lastArg = lastArg->next;
		lastPara = lastPara->next;
	}
	if (lastPara != nullptr)
		throw syntaxError("too much arguments");
	//----end of check-------------------------
	
	return evaluate(static_cast<ProcedureObj*>(func.get())->getBody(), newEnv);
}

Obj_ptr makeList(const Para_ptr & para)
{
	Obj_ptr obj;

	if (para==nullptr)
	{
		obj = Obj_ptr( new PairObj(nullptr, nullptr) );
		return obj;
	}

	obj = Obj_ptr( new PairObj(para->obj, makeList(para->next)) );
	return obj;
}


