/**
 *	give a parse tree, execute it and give the output
 */
#ifndef	___EXECUTE_H___
#define ___EXECUTE_H___
#include "useGMP.h"
#include "objects.h"
#include "environments.h"
#include "parse.h"
#include "builtIn.h"
#include "userDefined.h"
#include "compilingError.h"
#include <string>

static env_ptr global( new Environment );

Obj_ptr evaluate(const ParseTree_ptr &root, env_ptr &);

void checkToken(const std::string &, bool &, bool &, bool &, bool &);

Obj_ptr findIden(env_ptr env, const std::string&);

#endif
