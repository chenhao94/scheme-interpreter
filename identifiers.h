/**
 * 	This head file deal with the identifiers
 * 	In accord with the README, a identifier available should be a combination of letters, numbers and underscores starting with letters.
 *
 */

#ifndef __IDENTIFIERS_H_____
#define __IDENTIFIERS_H_____
#include "objects.h"
#include <string>
#include <map>
#include <memory>

typedef std::shared_ptr<Object> Obj_ptr;
typedef std::map<std::string, Obj_ptr> Obj_map;

Obj_map Identifiers;

void DefineObj(std::string, Obj_ptr);

Obj_ptr FindObj(std::string);

void DestroyObj(std::string);

#endif
