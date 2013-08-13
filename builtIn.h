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

Obj_ptr evaluateBuiltInProcedure(const std::string &, const Para_ptr&, env_ptr &);

Obj_ptr evaluateSyntax(const std::string &, const ParseTree_ptr &, env_ptr &);

Obj_ptr evaluateCondClause(const ParseTree_ptr &, env_ptr &);

Obj_ptr evaluateCaseClause(const Obj_ptr &, const ParseTree_ptr &, env_ptr &);

#endif
