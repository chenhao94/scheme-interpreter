/**
 * 	Disposing the objects.
 * 	Corresponding to "objects.h"
 *
 */

#include "useGMP.h"
#include "objects.h"
#include <string>
#include <sstream>

std::string BoolObj::ExternalRep()
{
	if ( value==true )
		return "#t";
	else
		return "#f";
}

std::string CharObj::ExternalRep()
{
	if (value=='\n')
		return "#\\newline";
	else if (value==' ')
		return "#\\space";
	else
	{
		std::ostringstream os;
		os << "#\\" << value;
		return os.str();
	}
}

std::string IntegerObj::ExternalRep()
{
	return value.get_str();
}

std::string RationalObj::ExternalRep()
{
	value.canonicalize();
	return value.get_str();
}

std::string RealObj::ExternalRep()
{
	std::ostringstream os;
	os << value;
	return os.str();
}

std::string StringObj::ExternalRep()
{
	int pos;
	std::string s(value);
	while (1)
	{
		pos = s.find('\\');
		if (pos == std::string::npos)
			break;
		s.replace(pos,1,"\\\\");
	}

	while (1)
	{
		pos = s.find('\"');
		if (pos == std::string::npos)
			break;
		s.replace(pos,1,"\\\"");
	}

	while (1)
	{
		pos = s.find('\n');
		if (pos == std::string::npos)
			break;
		s.replace(pos,1,"\\n");
	}

	return s;
}

std::string PairObj::ExternalRep()
{
	return "( " + obj1->ExternalRep() + "  " + obj2->ExternalRep() + " )";
}

std::string SymbolObj::ExternalRep()
{
	if (value[0]=='\'')
		return value.substr(1);
	//else if (value[0]=='`')
}

//-------Determine if T is PairObj--------
template <class T>
struct TypeIsPairObj
{
	static const bool value = false;
};

template <>
struct TypeIsPairObj<PairObj>
{
	static const bool value = true;
};
//----------------------------------------

template <class T>
bool operator==(T a, T b)
{
	if (TypeIsPairObj<T>::value)	// T is PairObj
	{
		return ( ( a->obj1 == b->obj1 ) && ( a->obj2 == b->obj2 ) );
	}
	else
	{
		return (a.value == b.value);
	}
}

BoolObj BoolObj::operator!()
{
	BoolObj ans(!value);
	return ans;
}

BoolObj operator&& (BoolObj a, BoolObj b)
{
	BoolObj ans( a.value && b.value );
	return ans;
}

BoolObj operator|| (BoolObj a, BoolObj b)
{
	BoolObj ans( a.value || b.value );
	return ans;
}

template <class T>
T operator+ (T a, T b)
{
	T ans(a.value + b.value);
	return ans;
}

template <class T>
T operator- (T a, T b)
{
	T ans(a.value - b.value);
	return ans;
}

template <class T>
T operator* (T a, T b)
{
	T ans(a.value * b.value);
	return ans;
}

template <class T>
T operator/ (T a, T b)
{
	T ans(a.value / b.value);
	return ans;
}

RationalObj operator/ (IntegerObj a, IntegerObj b)
{
	RationalObj ans(a.value, b.value);
	return ans;
}

template <class T>
bool operator< (T a, T b)
{
	return (a.value < b.value);
}

template <class T>
bool operator> (T a, T b)
{
	return (a.value > b.value);
}

template <class T>
bool operator<= (T a, T b)
{
	return (a.value <= b.value);
}

template <class T>
bool operator>= (T a, T b)
{
	return (a.value >= b.value);
}

template <class T>
bool operator!= (T a, T b)
{
	return (a.value != b.value);
}
