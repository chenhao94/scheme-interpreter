/**
 * 	To extract one sentence from the cache
 */

#include "extract.h"
#include <cctype>

void extract(std::string & sentence, std::string & cache, bool & cacheEndFlag)
{
	int pos = 0, cacheSize = cache.size();
	int pareLevel = 0;
	bool quoteFlag = false;

	sentence.clear();
	
	init:
	while ( pos<cacheSize && isspace(cache[pos]) ) ++pos;

	if (pos>=cacheSize)
	{
		cache.clear();
		cacheEndFlag = true;
		return;
	}

	if (cache[pos] == '\'' || cache[pos]=='`')
	{
		quotation:
		sentence.push_back(cache[pos++]);
		
		if (pos>=cacheSize)
		{
			cache.clear();
			cacheEndFlag = true;
			return;
		}
		
		if (cache[pos]=='\'' || cache[pos]=='`')
			goto quotation;
		
		if (cache[pos]==',')
			goto comma;
		
		if (cache[pos]=='@' || cache[pos]==')')
			throw syntaxError("Illegal use of quotation");
		
		if (cache[pos]=='\"')
			goto doubleQuotation;
		else if (cache[pos]=='(')
			goto parentheses;
		else
			goto commonplace;

	}
	else if (cache[pos]=='#')
	{
		sentence.push_back(cache[pos++]);

		if (cache[pos]=='\\')
		{
			sentence.push_back(cache[pos++]);
			sentence.push_back(cache[pos++]);
		}

		goto commonplace;
	}
	else if (cache[pos]=='(')
	{
		parentheses:
		++pareLevel;
		sentence.push_back(cache[pos++]);

		while (pos<cacheSize)
		{
			if (quoteFlag==true)
			{
				if (cache[pos]=='\"')
				{
					quoteFlag = false;
					sentence.push_back(cache[pos++]);
				}
				else if (cache[pos]=='\\')
				{
					sentence.push_back(cache[pos++]);
					sentence.push_back(cache[pos++]);
				}
				else
				{
					sentence.push_back(cache[pos++]);
				}
			}
			else
			{
				if (cache[pos]=='\"')
				{
					quoteFlag = true;
					sentence.push_back(cache[pos++]);
				}
				else if (cache[pos]=='(')
				{
					++pareLevel;
					sentence.push_back(cache[pos++]);
				}
				else if (cache[pos]==')')
				{
					--pareLevel;
					if (pareLevel < 0)
						throw syntaxError("unexpected: \')\'");
					sentence.push_back(cache[pos++]);
					if (pareLevel==0)
						break;
				}
				else
					sentence.push_back(cache[pos++]);
			}
		}

		if (pos>cacheSize || pareLevel!=0 || quoteFlag==true)
		{
			cacheEndFlag = true;
			sentence.clear();
			return;
		}
		else
		{
			cacheEndFlag = false;
			cache.erase(0, pos);
			return;
		}

	}
	else if (cache[pos]=='\"')
	{
		doubleQuotation:
		sentence.push_back(cache[pos++]);
		quoteFlag = true;

		while (pos<cacheSize)
		{
			if (cache[pos]=='\"')
			{
				quoteFlag = false;
				sentence.push_back(cache[pos++]);
				break;
			}
			else if (cache[pos]=='\\')
			{
				sentence.push_back(cache[pos++]);
				sentence.push_back(cache[pos++]);
			}
			else
			{
				sentence.push_back(cache[pos++]);
			}
		}

		if (pos>cacheSize || quoteFlag==true)
		{
			cacheEndFlag = true;
			sentence.clear();
			return;
		}
		else
		{
			cacheEndFlag = false;
			cache.erase(0, pos);
			return;
		}
	}
	else if (cache[pos] == ',')
	{
		comma:
		sentence.push_back(cache[pos++]);
		if (cache[pos]=='@')
			sentence.push_back(cache[pos++]);
		goto init;

		throw syntaxError("illegal use: \',\'");
	}
	else
	{
		commonplace:
		while (pos<cacheSize)
		{
			if (cache[pos]==')')
				throw syntaxError("unexpected: \')\'");
				
			if (cache[pos]=='(' || cache[pos]=='\'' || cache[pos]=='`' || cache[pos]=='\"' || cache[pos]==',' || isspace(cache[pos]))
				break;
			else
				sentence.push_back(cache[pos++]);
		}
		cacheEndFlag = false;
		cache.erase(0, pos);
		return;
	}
}

void eliminateComment(std::string &s)
{
	int pos = 0, size = s.size();
	bool quoteFlag = false;
	while (pos < size)
	{
		if (s[pos]=='#')
		{
			++pos;

			if (s[pos]=='\\')
				pos+=2;

			goto commonplace;
		}
		else if (s[pos]=='\"')
		{
			doubleQuotation:
			++pos;
			quoteFlag = true;

			while (pos<size)
			{
				if (s[pos]=='\"')
				{
					quoteFlag = false;
					++pos;
					break;
				}
				else if (s[pos]=='\\')
					pos += 2;
				else
					++pos;
			}
		}
		else
		{
			commonplace:
			while (pos<size)
			{
				if (s[pos]=='(' || s[pos]==')' || s[pos]=='\'' || s[pos]=='`' || s[pos]==',' || isspace(s[pos]))
				{
					++pos;
					break;
				}
				else if (s[pos]=='\"')
					break;
				else if (s[pos]==';')
				{
					s.erase(pos);
					return;
				}
				else
					++pos;
			}
		}
	}
}
