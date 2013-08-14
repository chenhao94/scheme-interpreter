/**
 * 	exception class of compiling error
 * 	
 */

#ifndef ___ERROR_H____
#define ___ERROR_H____
#include <string>

class syntaxError
{
	public:

		syntaxError(std::string msg) : errMsg(msg) {}

		std::string getMsg() { return errMsg; }

	private:

		std::string errMsg;

};

class runtimeError
{
	public:

		runtimeError(std::string msg) : errMsg(msg) {}

		std::string getMsg() { return errMsg; }

	private:

		std::string errMsg;

};

#endif

