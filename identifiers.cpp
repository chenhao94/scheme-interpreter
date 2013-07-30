/**
 * 	Deal with the identifiers
 * 	In accord with the README, a identifier available should be a combination of letters, numbers and underscores starting with letters.
 *
 */

#include "identifiers.h"

void DefineObj(std::string name, Obj_ptr obj)
{
	Identifiers[name] = obj;
}

Obj_ptr FindObj(std::string name)
{
	Obj_map::iterator itr=Identifiers.find(name);

	if (itr == Identifiers.end())
		return NULL;

	return itr->second; 
}

void DestroyObj(std::string name)
{
	Identifier.erase(name);
}

