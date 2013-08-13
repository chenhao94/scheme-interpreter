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
		
		if (pos>=cacheSize || cache[pos]=='\'' || cache[pos]=='`' || cache[pos]==',' || cache[pos]=='@' || cache[pos]==')')
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
	else
	{
		commonplace:
		while (pos<cacheSize)
		{
			if (cache[pos]=='(' || cache[pos]==')' || cache[pos]=='\'' || cache[pos]=='`' || cache[pos]=='\"' || cache[pos]==',' || isspace(cache[pos]))
				break;
			else
				sentence.push_back(cache[pos++]);
		}
		cacheEndFlag = false;
		cache.erase(0, pos);
		return;
	}
}
