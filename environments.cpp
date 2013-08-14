/**
 * 	Deal with the environments
 *
 */

#include "environments.h"

Environment::~Environment()
{}

void Environment::DefineObj(std::string name, Obj_ptr obj)
{
	env_map[name] = obj;
}

Obj_ptr Environment::FindObj(std::string name)
{
	Obj_map::iterator itr=env_map.find(name);

	if (itr == env_map.end())
		return NULL;

	return itr->second; 
}

void Environment::DestroyObj(std::string name)
{
	env_map.erase(name);
}

