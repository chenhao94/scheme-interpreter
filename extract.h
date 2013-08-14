/**
 * 	To extract one sentence from the cache
 */

#ifndef ___EXTRACT_H____
#define ___EXTRACT_H____

#include "compilingError.h"
#include <string>
void extract(std::string & sentence, std::string & cache, bool &);

void eliminateComment(std::string &);

#endif

