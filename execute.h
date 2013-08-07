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

static Environment *global = new Environment;

Object* evaluate(ParseTree* root);

#endif
