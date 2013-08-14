/**
 * 	process the user defined procedures
 */

#ifndef ___USERDEFINED_H__
#define ___USERDEFINED_H__
#include "objects.h"
#include "parse.h"
#include "environments.h"
#include "execute.h"
#include "error.h"
#include "builtIn.h"
#include <string>

Obj_ptr evaluateUserDefined( const Obj_ptr & , const Para_ptr &, env_ptr &  env);

Obj_ptr makeList(const Para_ptr &);

#endif

