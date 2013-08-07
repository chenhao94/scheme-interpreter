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

		typedef Environment* env_ptr;
		
		Obj_map env_map;

	public:

		Environment *next;

		Environment(const env_ptr &ptr=NULL) : next(ptr) {}

		virtual ~Environment();

		void DefineObj(std::string, Obj_ptr);

		Obj_ptr FindObj(std::string);

		void DestroyObj(std::string);
};

#endif
