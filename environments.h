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

class Environment;
typedef std::map<std::string, Obj_ptr> Obj_map;
typedef std::shared_ptr<Environment> env_ptr;

class Environment
{
	private:

		Obj_map env_map;

	public:

		env_ptr next;

		Environment(const env_ptr &ptr=nullptr) : next(ptr) {}

		virtual ~Environment();

		void DefineObj(std::string, Obj_ptr);

		Obj_ptr FindObj(std::string);

		void DestroyObj(std::string);
};

#endif
