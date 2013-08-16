/**
 * 	process the built-in syntaxs and procedures
 */
#ifndef ___BUILTIN_H___
#define ___BUILTIN_H___
#include "objects.h"
#include "parse.h"
#include "environments.h"
#include "execute.h"
#include "userDefined.h"
#include "error.h"
#include <string>
#include <set>

typedef std::set<std::string> builtInSet;
typedef std::set<Obj_ptr> ObjectSet;

Obj_ptr evaluateBuiltInProcedure(const std::string &, const Para_ptr&, env_ptr &);

Obj_ptr evaluateSyntax(const std::string &, const ParseTree_ptr &, env_ptr &);

Obj_ptr evaluateCondClause(const ParseTree_ptr &, env_ptr &);

Obj_ptr evaluateCaseClause(const Obj_ptr &, const ParseTree_ptr &, env_ptr &);

bool isList( const Obj_ptr & );

Obj_ptr Append( const Para_ptr &);

Obj_ptr Apply(const Obj_ptr &, const Para_ptr &, env_ptr &);

#endif
