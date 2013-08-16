/**
 * 	process the user defined procedures
 */

#include "userDefined.h"

Obj_ptr evaluateUserDefined( const Obj_ptr &func, const Para_ptr & para, env_ptr & env)
{
	if ( func->Type != Procedure )
		throw syntaxError("not a procedure");

	if ( static_cast<ProcedureObj*>(func.get())->getFlag() ==true ) // ------ built-in procedure--------
		return evaluateBuiltInProcedure(static_cast<ProcedureObj*>(func.get())->getName(), para, env);

	env_ptr newEnv( new Environment(static_cast<ProcedureObj*>(func.get())->getEnv()) );

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
	
	ParseTree_ptr body = static_cast<ProcedureObj*>(func.get())->getBody();
	while (body->getBrother())
	{
		evaluate(body, newEnv);
		body = body->getBrother();
	}
	return evaluate(body, newEnv);
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


