/**
 * 	This head file deal with the environments
 *
 */

#ifndef __ENVIRONMENTS_H___
#define __ENVIRONMENTS_H___

#include "objects.h"
#include <string>
#include <map>
#include <memory>

typedef std::shared_ptr<Object> Obj_ptr;
typedef std::map<std::string, Obj_ptr> Obj_map;

class Environment
{
	private:
		
		Obj_map env_map;

	public:

		Environment *next;

		Environment() : next(NULL) {}

		virtual ~Environment();

		void DefineObj(std::string, Obj_ptr);

		Obj_ptr FindObj(std::string);

		void DestroyObj(std::string);
};

#endif
