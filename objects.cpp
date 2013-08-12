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

bool operator==(const PairObj &a, const PairObj &b)
{
	return ( ( a.obj1 == b.obj1 ) && ( a.obj2 == b.obj2 ) );
}

BoolObj BoolObj::operator!()
{
	BoolObj ans(!value);
	return ans;
}

BoolObj operator&& (const BoolObj &a, const BoolObj &b)
{
	BoolObj ans( a.value && b.value );
	return ans;
}

BoolObj operator|| (const BoolObj &a, const BoolObj &b)
{
	BoolObj ans( a.value || b.value );
	return ans;
}

RationalObj operator/ (const IntegerObj &a, const IntegerObj &b)
{
	RationalObj ans(a.value, b.value);
	return ans;
}

IntegerObj operator% (const IntegerObj &a, const IntegerObj &b)
{
	IntegerObj ans(a.value % b.value);
	return ans;
}

bool equal (const Obj_ptr &aPtr, const Obj_ptr &bPtr)
{
	Object *a = aPtr.get(), *b = bPtr.get();
	if (a->Type != b->Type)
		return false;
	if (a->Type == Bool)
		return operator==( *static_cast<BoolObj*>(a), *static_cast<BoolObj*>(b) );
	if (a->Type == Char)
		return operator==( *static_cast<CharObj*>(a), *static_cast<CharObj*>(b) );
	if (a->Type == Number)
	{
		NumberType aT(static_cast<NumberObj*>(a)->numType), bT(static_cast<NumberObj*>(b)->numType);
		if (aT == Real || bT == Real)
			return operator==( *static_cast<RealObj*>(a), *static_cast<RealObj*>(b) );
		if (aT == Rational || bT == Rational)
			return operator==( *static_cast<RationalObj*>(a), *static_cast<RationalObj*>(b) );
		return operator==( *static_cast<IntegerObj*>(a), *static_cast<IntegerObj*>(b) );

	}
	if (a->Type == String)
		return operator==( *static_cast<StringObj*>(a), *static_cast<StringObj*>(b) );
	if (a->Type == Pair)
		return operator==( *static_cast<PairObj*>(a), *static_cast<PairObj*>(b) );
	if (a->Type == Symbol)
		return operator==( *static_cast<SymbolObj*>(a), *static_cast<SymbolObj*>(b) );
	if (a->Type == Procedure)
		return a==b;
}

bool notEqual (const Obj_ptr &aPtr, const Obj_ptr &bPtr)
{
	return !equal(aPtr, bPtr);
}

bool operator< (const Obj_ptr &aPtr, const Obj_ptr &bPtr)
{
	Object *a = aPtr.get(), *b = bPtr.get();
	if (a->Type != Number || b->Type != Number)
		throw syntaxError("Unexpected type");

	NumberType aT(static_cast<NumberObj*>(a)->numType), bT(static_cast<NumberObj*>(b)->numType);
	if (aT == Real || bT == Real)
		return operator<( *static_cast<RealObj*>(a), *static_cast<RealObj*>(b) );
	if (aT == Rational || bT == Rational)
		return operator<( *static_cast<RationalObj*>(a), *static_cast<RationalObj*>(b) );
	return operator<( *static_cast<IntegerObj*>(a), *static_cast<IntegerObj*>(b) );
}

bool operator> (const Obj_ptr &aPtr, const Obj_ptr &bPtr)
{
	Object *a = aPtr.get(), *b = bPtr.get();
	if (a->Type!=Number || b->Type!=Number)
		throw syntaxError("Unexpected type");

	NumberType aT( static_cast<NumberObj*>(a)->numType ), bT( static_cast<NumberObj*>(b)->numType );
	if (aT == Real || bT == Real)
		return operator>( *static_cast<RealObj*>(a), *static_cast<RealObj*>(b) );
	if (aT == Rational || bT == Rational)
		return operator>( *static_cast<RationalObj*>(a), *static_cast<RationalObj*>(b) );
	return operator>( *static_cast<IntegerObj*>(a), *static_cast<IntegerObj*>(b) );
}

bool operator<= (const Obj_ptr &aPtr, const Obj_ptr &bPtr)
{
	Object *a = aPtr.get(), *b = bPtr.get();
	if (a->Type != Number || b->Type != Number)
		throw syntaxError("Unexpected type");

	NumberType aT( static_cast<NumberObj*>(a)->numType ), bT( static_cast<NumberObj*>(b)->numType );
	if (aT == Real || bT == Real)
		return operator<=( *static_cast<RealObj*>(a), *static_cast<RealObj*>(b) );
	if (aT == Rational || bT == Rational)
		return operator<=( *static_cast<RationalObj*>(a), *static_cast<RationalObj*>(b) );
	return operator<=( *static_cast<IntegerObj*>(a), *static_cast<IntegerObj*>(b) );
}

bool operator>= (const Obj_ptr &aPtr, const Obj_ptr &bPtr)
{
	Object *a = aPtr.get(), *b = bPtr.get();
	if (a->Type != Number || b->Type != Number)
		throw syntaxError("Unexpected type");

	NumberType aT(static_cast<NumberObj*>(a)->numType), bT(static_cast<NumberObj*>(b)->numType);
	if (aT == Real || bT == Real)
		return operator>=( *static_cast<RealObj*>(a), *static_cast<RealObj*>(b) );
	if (aT == Rational || bT == Rational)
		return operator>=( *static_cast<RationalObj*>(a), *static_cast<RationalObj*>(b) );
	return operator>=( *static_cast<IntegerObj*>(a), *static_cast<IntegerObj*>(b) );
}

Obj_ptr operator+ (const Obj_ptr &aPtr, const Obj_ptr &bPtr)
{
	Object *a = aPtr.get(), *b = bPtr.get();

	if (a->Type == String && b->Type == String)
	{
		String_ptr ptr( new StringObj( *static_cast<StringObj*>(a) + *static_cast<StringObj*>(b) ) );
		return ptr;
	}

	if (a->Type != Number || b->Type != Number)
		throw syntaxError("Unexpected type");

	NumberType aT(static_cast<NumberObj*>(a)->numType), bT(static_cast<NumberObj*>(b)->numType);
	if (aT == Real || bT == Real)
	{
		Real_ptr ptr( new RealObj( *static_cast<RealObj*>(a) + *static_cast<RealObj*>(b) ) );
		return ptr;
	}
	if (aT == Rational || bT == Rational)
	{
		Rational_ptr ptr( new RationalObj( *static_cast<RationalObj*>(a) + *static_cast<RationalObj*>(b) ) );
		return ptr;
	}
	Int_ptr ptr( new IntegerObj( *static_cast<IntegerObj*>(a) + *static_cast<IntegerObj*>(b) ) );
	return ptr;
}

Obj_ptr operator- (const Obj_ptr &aPtr, const Obj_ptr &bPtr)
{
	Object *a = aPtr.get(), *b = bPtr.get();
	if (a->Type != Number || b->Type != Number)
		throw syntaxError("Unexpected type");

	NumberType aT(static_cast<NumberObj*>(a)->numType), bT(static_cast<NumberObj*>(b)->numType);
	if (aT == Real || bT == Real)
	{
		Real_ptr ptr( new RealObj( *static_cast<RealObj*>(a) - *static_cast<RealObj*>(b) ) );
		return ptr;
	}
	if (aT == Rational || bT == Rational)
	{
		Rational_ptr ptr( new RationalObj( *static_cast<RationalObj*>(a) - *static_cast<RationalObj*>(b) ) );
		return ptr;
	}
	Int_ptr ptr( new IntegerObj( *static_cast<IntegerObj*>(a) - *static_cast<IntegerObj*>(b) ) );
	return ptr;
}

Obj_ptr operator* (const Obj_ptr &aPtr, const Obj_ptr &bPtr)
{
	Object *a = aPtr.get(), *b = bPtr.get();
	if (a->Type != Number || b->Type != Number)
		throw syntaxError("Unexpected type");

	NumberType aT(static_cast<NumberObj*>(a)->numType), bT(static_cast<NumberObj*>(b)->numType);
	if (aT == Real || bT == Real)
	{
		Real_ptr ptr( new RealObj( *static_cast<RealObj*>(a) * *static_cast<RealObj*>(b) ) );
		return ptr;
	}
	if (aT == Rational || bT == Rational)
	{
		Rational_ptr ptr( new RationalObj( *static_cast<RationalObj*>(a) * *static_cast<RationalObj*>(b) ) );
		return ptr;
	}
	Int_ptr ptr( new IntegerObj( *static_cast<IntegerObj*>(a) * *static_cast<IntegerObj*>(b) ) );
	return ptr;
}

Obj_ptr operator/ (const Obj_ptr &aPtr, const Obj_ptr &bPtr)
{
	Object *a = aPtr.get(), *b = bPtr.get();
	if (a->Type != Number || b->Type != Number)
		throw syntaxError("Unexpected type");

	NumberType aT(static_cast<NumberObj*>(a)->numType), bT(static_cast<NumberObj*>(b)->numType);
	if (aT == Real || bT == Real)
	{
		Real_ptr ptr( new RealObj( *static_cast<RealObj*>(a) / *static_cast<RealObj*>(b) ) );
		return ptr;
	}
	if (aT == Rational || bT == Rational)
	{
		Rational_ptr ptr( new RationalObj( *static_cast<RationalObj*>(a) / *static_cast<RationalObj*>(b) ) );
		return ptr;
	}
	Rational_ptr ptr( new RationalObj( *static_cast<IntegerObj*>(a) / *static_cast<IntegerObj*>(b) ) );
	return ptr;
}

Obj_ptr operator% (const Obj_ptr &aPtr, const Obj_ptr &bPtr)
{
	Object *a = aPtr.get(), *b = bPtr.get();
	if (a->Type != Number || b->Type != Number)
		throw syntaxError("Unexpected type");

	NumberType aT(static_cast<NumberObj*>(a)->numType), bT(static_cast<NumberObj*>(b)->numType);
	if (aT != Integer || bT != Integer)
		throw syntaxError("Unexpected type");

	Int_ptr ptr( new IntegerObj( *static_cast<IntegerObj*>(a) % *static_cast<IntegerObj*>(b) ) );
	return ptr;
}

Obj_ptr operator! (const Obj_ptr &aPtr)
{
	Object *a = aPtr.get();
	if (a->Type != Bool)
		throw syntaxError("Unexpected type");
	
	Bool_ptr ptr( new BoolObj( !( *static_cast<BoolObj*>(a) ) ) );
	return ptr;
}

Obj_ptr operator&& (const Obj_ptr &aPtr, const Obj_ptr &bPtr)
{
	Object *a = aPtr.get(), *b = bPtr.get();
	if (a->Type != Bool || b->Type !=Bool)
		throw syntaxError("Unexpected type");
	
	Bool_ptr ptr( new BoolObj(  *static_cast<BoolObj*>(a) && *static_cast<BoolObj*>(b)  ) );
	return ptr;
}

Obj_ptr operator|| (const Obj_ptr &aPtr, const Obj_ptr &bPtr)
{
	Object *a = aPtr.get(), *b = bPtr.get();
	if (a->Type != Bool || b->Type !=Bool)
		throw syntaxError("Unexpected type");
	
	Bool_ptr ptr( new BoolObj(  *static_cast<BoolObj*>(a) || *static_cast<BoolObj*>(b)  ) );
	return ptr;
}

