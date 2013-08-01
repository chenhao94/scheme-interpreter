/**
 * 	exception class of compiling error
 * 	
 */

#ifndef ___COMPILING_ERROR_H____
#define ___COMPILING_ERROR_H____
#include <string>

class syntaxError
{
	public:

		syntaxError(std::string msg) : errMsg(msg) {}

		std::string getMsg() { return errMsg; }

	private:

		std::string errMsg;

};

#endif

