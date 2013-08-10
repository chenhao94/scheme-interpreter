/**
 *	give a parse tree, execute it and give the output
 */
#ifndef	___EXECUTE_H___
#define ___EXECUTE_H___
#include "useGMP.h"
#include "objects.h"
#include "enviornment.h"
#include "parse.h"
#include "compilingError.h"
#include <string>

static env_ptr global = new Environment;

Obj_ptr evaluate(ParseTree_ptr root);

#endif
