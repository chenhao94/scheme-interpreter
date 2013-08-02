/**
 * 	This head file is designed to generate and process the parse tree
 *
 */
#ifndef ___PARSE_H__
#define ___PARSE_H__

#include "objects.h"
#include "environments.h"
#include "compilingError.h"
#include <string>

class ParseTree;

bool getToken( std::string &name, const std::string &code, int &pos ); // distract the first token from the list, start with 'pos', put it in 'name', if failed, return false

ParseTree* Parsing(std::string);

class ParseTree
{	
	public:

		typedef ParseTree* ParseTree_ptr;

	private:

		std::string token;

		ParseTree *son, *brother;

	public:

		ParseTree(const std::string &tok, const ParseTree_ptr &s=NULL, const ParseTree_ptr &b=NULL): token(tok), son(s), brother(b) {}

		virtual ~ParseTree();

		void evaluate();	// evaluate the subtree and turn it to a node.

		ParseTree* getSon() { return son; }

		ParseTree* getBrother() { return brother; }

		std::string getToken() { return token; }

	friend ParseTree* Parsing(std::string);
};

#endif

