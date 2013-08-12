/**
 * 	process the built-in syntaxs and procedures
 */

#ifndef ___BUILTIN_H___
#define ___BUILTIN_H___
#include "objects.h"
#include "parse.h"
#include "environments.h"
#include "execute.h"
#include "compilingError.h"
#include <string>
#include <set>

typedef std::set<std::string> builtInSet;

builtInSet builtInSyntax({ "if","cond","case","else","define","set!","lambda","quote","quasiquote" });

builtInSet builtInProcedure({"+","-","*","/","<",">","<=",">=","gcd","max","min","exact->inexact","inexact->exact","cons","car","cdr","eq?","eqv?","equal?"});

Obj_ptr evaluateBuiltInProcedure(const std::string &, const Para_ptr&, env_ptr &);

Obj_ptr evaluateSyntax(const std::string &, const ParseTree_ptr &, env_ptr &);

#endif
