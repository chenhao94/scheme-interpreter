/**
 * 	To import the library files
 */

#ifndef __IMPORT_H____
#define __IMPORT_H____
#include <iostream>
#include <fstream>
#include <string>
#include "extract.h"
#include "parse.h"
#include "execute.h"
#include "error.h"

void ImportFile(const std::string &, env_ptr & env);

#endif
