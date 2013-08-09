/**
 * 	process the built-in syntaxs and procedures
 */

#ifndef ___BUILTIN_H___
#define ___BUILTIN_H___
#include "objects.h"
#include "parse.h"
#include "environments.h"
#include "compilingError.h"
#include <string>
#include <set>

typedef std::set<std::string> builtInSet;

std::string builtInName[50]={"+","-","*","/","<",">","<=",">=","gcd","max","min","exact->inexact","inexact->exact","if","cond","else","cons","car","cdr","define","set!","lambda","eq?","eqv?","equal?","quote","quasiquote"};

Obj_ptr evaluateBuiltIn(const std::string &, const Para_ptr&);

#endif
