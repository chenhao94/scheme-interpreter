/**
 * 	Disposing the objects.
 * 	Corresponding to "objects.h"
 *
 */

#include "useGMP.h"
#include "objects.h"
#include <string>
#include <sstream>
#include <cassert>

void IntegerObj::getInt(Number_ptr & ptr, IntegerObj *& obj)
{
	ptr = nullptr;
	obj = this;
}

void IntegerObj::getRational(Number_ptr & ptr, RationalObj *& obj)
{
	ptr = Rational_ptr( obj = new RationalObj(value) );
}

void IntegerObj::getReal(Number_ptr & ptr, RealObj *& obj)
{
	ptr = Real_ptr( obj = new RealObj(value) );
}

void RationalObj::getInt(Number_ptr & ptr, IntegerObj *& obj)
{
	assert(false);
}

void RationalObj::getRational(Number_ptr & ptr, RationalObj *& obj)
{
	ptr = nullptr; 
	obj = this;
}

void RationalObj::getReal(Number_ptr & ptr, RealObj *& obj)
{
	ptr = Real_ptr( obj = new RealObj(value) );
}

void RealObj::getInt(Number_ptr & ptr, IntegerObj *& obj)
{
	assert(false);
}

void RealObj::getRational(Number_ptr & ptr, RationalObj *& obj)
{
	assert(false);
}

void RealObj::getReal(Number_ptr & ptr, RealObj *& obj)
{
	ptr = nullptr;
	obj = this;
}

StringObj::StringObj(std::string s): Object(String)
{
	int pos = 0;

	while (1)
	{
		pos = s.find('\\', pos);
		if (pos == std::string::npos)
			break;
		if (s[pos + 1]=='\\') 
			s.replace(pos,2,"\\");
		else if (s[pos + 1]=='\'') 
			s.replace(pos,2,"\'");
		else if (s[pos + 1]=='\"') 
			s.replace(pos,2,"\"");
		else if (s[pos + 1]=='n') 
			s.replace(pos,2,"\n");
		else if (s[pos + 1]=='t') 
			s.replace(pos,2,"\t");
		else
			throw syntaxError(std::string("unrecognized character: \\") + std::string(1, s[pos + 1]));
		++pos;
	}

	value = s;
}

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
	int pos = 0;
	std::string s(value);
	while (1)
	{
		pos = s.find('\\', pos);
		if (pos == std::string::npos)
			break;
		s.replace(pos,1,"\\\\");
		pos += 2;
	}

	pos = 0;
	while (1)
	{
		pos = s.find('\"', pos);
		if (pos == std::string::npos)
			break;
		s.replace(pos,1,"\\\"");
		pos +=2;
	}

	pos = 0;
	while (1)
	{
		pos = s.find('\n');
		if (pos == std::string::npos)
			break;
		s.replace(pos,1,"\\n");
		pos += 2;
	}

	pos = 0;
	while (1)
	{
		pos = s.find('\t');
		if (pos == std::string::npos)
			break;
		s.replace(pos,1,"\\t");
		pos += 2;
	}

	return "\"" + s + "\"";
}

std::string PairObj::ExternalRep()
{
	if ( obj1 == nullptr && obj2 == nullptr)
		return "( )";

	std::string str("(");
	ObjectSet ExternalCheckSet({});
	Obj_ptr obj = obj2;

	str.append( " " + obj1->ExternalRep() );

	while (!emptyPair(obj))
	{
		if ( ExternalCheckSet.find(obj) != 
				ExternalCheckSet.end())
			return "#<closure>";

		ExternalCheckSet.insert(obj);

		if (obj->Type != Pair)
		{
			str.append(" . " + obj->ExternalRep() + " )");
			return str;
		}
		else
		{
			str.append( " " + static_cast<PairObj*>(obj.get())->obj1->ExternalRep() );
			obj = static_cast<PairObj*>(obj.get())->obj2;
		}
	}

	str.append(" )");
	return str;
}

std::string SymbolObj::ExternalRep()
{
	return value;
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
	if (b.value == 0)
		throw runtimeError("divided by zero");
	RationalObj ans(a.value, b.value);
	return ans;
}

IntegerObj Mod (const IntegerObj &a, const IntegerObj &b)
{
	if (b.value == 0)
		throw runtimeError("divided by zero");
	bigInteger modAns = a.value % b.value;
	if ((b.value<0 && modAns>0) || (b.value>0 && modAns<0))
		modAns+=b.value;
	IntegerObj ans(modAns);
	return ans;
}

IntegerObj Rem (const IntegerObj &a, const IntegerObj &b)
{
	if (b.value == 0)
		throw runtimeError("divided by zero");
	bigInteger remAns = a.value % b.value;
	if (a.value<0 && remAns>0)
		remAns-=abs(b.value);
	if (a.value>0 && remAns<0)
		remAns+=abs(b.value);
	IntegerObj ans(remAns);
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
		{
			RealObj *pa, *pb;
			Number_ptr paPtr, pbPtr;
			static_cast<NumberObj*>(a)->getReal(paPtr, pa);
			static_cast<NumberObj*>(b)->getReal(pbPtr, pb);
			return (*pa == *pb );
		}
		if (aT == Rational || bT == Rational)
		{
			RationalObj *pa, *pb;
			Number_ptr paPtr, pbPtr;
			static_cast<NumberObj*>(a)->getRational(paPtr, pa);
			static_cast<NumberObj*>(b)->getRational(pbPtr, pb);
			return ( *pa == *pb );
		}
		IntegerObj *pa, *pb;
		Number_ptr paPtr, pbPtr;
		static_cast<NumberObj*>(a)->getInt(paPtr, pa);
		static_cast<NumberObj*>(b)->getInt(pbPtr, pb);
		return ( *pa == *pb );

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

bool lessThan (const Obj_ptr &aPtr, const Obj_ptr &bPtr)
{
	Object *aa = aPtr.get(), *bb = bPtr.get();

	if (aPtr->Type == String && bPtr->Type == String)
		return ( *static_cast<StringObj*>(aPtr.get()) < *static_cast<StringObj*>(bPtr.get()) );
	
	if (aa->Type!=Number || bb->Type!=Number)
		throw syntaxError("Unexpected type");

	NumberObj *a(static_cast<NumberObj*>(aa)), *b(static_cast<NumberObj*>(bb));

	NumberType aT( a->numType ), bT( b->numType );
	if (aT == Real || bT == Real)
	{
		RealObj *pa, *pb;
		Number_ptr paPtr, pbPtr;
		a->getReal(paPtr, pa);
		b->getReal(pbPtr, pb);
		return (*pa < *pb );
	}
	if (aT == Rational || bT == Rational)
	{
		RationalObj *pa, *pb;
		Number_ptr paPtr, pbPtr;
		a->getRational(paPtr, pa);
		b->getRational(pbPtr, pb);
		return ( *pa < *pb );
	}
	IntegerObj *pa, *pb;
	Number_ptr paPtr, pbPtr;
	a->getInt(paPtr, pa);
	b->getInt(pbPtr, pb);
	return ( *pa < *pb );
}

bool greaterThan (const Obj_ptr &aPtr, const Obj_ptr &bPtr)
{
	Object *aa = aPtr.get(), *bb = bPtr.get();

	if (aPtr->Type == String && bPtr->Type == String)
		return ( *static_cast<StringObj*>(aPtr.get()) > *static_cast<StringObj*>(bPtr.get()) );
	
	if (aa->Type!=Number || bb->Type!=Number)
		throw syntaxError("Unexpected type");

	NumberObj *a(static_cast<NumberObj*>(aa)), *b(static_cast<NumberObj*>(bb));

	NumberType aT( a->numType ), bT( b->numType );
	if (aT == Real || bT == Real)
	{
		RealObj *pa, *pb;
		Number_ptr paPtr, pbPtr;
		a->getReal(paPtr, pa);
		b->getReal(pbPtr, pb);
		return (*pa > *pb );
	}
	if (aT == Rational || bT == Rational)
	{
		RationalObj *pa, *pb;
		Number_ptr paPtr, pbPtr;
		a->getRational(paPtr, pa);
		b->getRational(pbPtr, pb);
		return ( *pa > *pb );
	}
	IntegerObj *pa, *pb;
	Number_ptr paPtr, pbPtr;
	a->getInt(paPtr, pa);
	b->getInt(pbPtr, pb);
	return ( *pa > *pb );
}

bool lessEq (const Obj_ptr &aPtr, const Obj_ptr &bPtr)
{
	return !greaterThan(aPtr, bPtr);
}

bool greaterEq (const Obj_ptr &aPtr, const Obj_ptr &bPtr)
{
	return !lessThan(aPtr, bPtr);
}

Obj_ptr Add (const Obj_ptr &aPtr, const Obj_ptr &bPtr)
{
	Object *aa = aPtr.get(), *bb = bPtr.get();

	if (aa->Type == String && bb->Type == String)
	{
		String_ptr ptr( new StringObj( *static_cast<StringObj*>(aa) + *static_cast<StringObj*>(bb) ) );
		return ptr;
	}

	if (aa->Type != Number || bb->Type != Number)
		throw syntaxError("Unexpected type");

	NumberObj *a(static_cast<NumberObj*>(aa)), *b(static_cast<NumberObj*>(bb));

	NumberType aT( a->numType ), bT( b->numType );
	if (aT == Real || bT == Real)
	{
		RealObj *pa, *pb;
		Number_ptr paPtr, pbPtr;
		a->getReal(paPtr, pa);
		b->getReal(pbPtr, pb);
		return Obj_ptr(new RealObj(*pa + *pb ));
	}
	if (aT == Rational || bT == Rational)
	{
		RationalObj *pa, *pb;
		Number_ptr paPtr, pbPtr;
		a->getRational(paPtr, pa);
		b->getRational(pbPtr, pb);
		return Obj_ptr(new RationalObj( *pa + *pb ));
	}
	IntegerObj *pa, *pb;
	Number_ptr paPtr, pbPtr;
	a->getInt(paPtr, pa);
	b->getInt(pbPtr, pb);
	return Obj_ptr(new IntegerObj( *pa + *pb ));
}

Obj_ptr Subtract (const Obj_ptr &aPtr, const Obj_ptr &bPtr)
{
	Object *aa = aPtr.get(), *bb = bPtr.get();
	if (aa->Type != Number || bb->Type != Number)
		throw syntaxError("Unexpected type");

	NumberObj *a(static_cast<NumberObj*>(aa)), *b(static_cast<NumberObj*>(bb));

	NumberType aT( a->numType ), bT( b->numType );
	if (aT == Real || bT == Real)
	{
		RealObj *pa, *pb;
		Number_ptr paPtr, pbPtr;
		a->getReal(paPtr, pa);
		b->getReal(pbPtr, pb);
		return Obj_ptr(new RealObj( *pa - *pb ));
	}
	if (aT == Rational || bT == Rational)
	{
		RationalObj *pa, *pb;
		Number_ptr paPtr, pbPtr;
		a->getRational(paPtr, pa);
		b->getRational(pbPtr, pb);
		return Obj_ptr(new RationalObj( *pa - *pb ));
	}
	IntegerObj *pa, *pb;
	Number_ptr paPtr, pbPtr;
	a->getInt(paPtr, pa);
	b->getInt(pbPtr, pb);
	return Obj_ptr(new IntegerObj( *pa - *pb ));
}

Obj_ptr Multiply (const Obj_ptr &aPtr, const Obj_ptr &bPtr)
{
	Object *aa = aPtr.get(), *bb = bPtr.get();
	if (aa->Type != Number || bb->Type != Number)
		throw syntaxError("Unexpected type");

	NumberObj *a(static_cast<NumberObj*>(aa)), *b(static_cast<NumberObj*>(bb));

	NumberType aT( a->numType ), bT( b->numType );
	if (aT == Real || bT == Real)
	{
		RealObj *pa, *pb;
		Number_ptr paPtr, pbPtr;
		a->getReal(paPtr, pa);
		b->getReal(pbPtr, pb);
		return Obj_ptr(new RealObj( *pa * *pb ));
	}
	if (aT == Rational || bT == Rational)
	{
		RationalObj *pa, *pb;
		Number_ptr paPtr, pbPtr;
		a->getRational(paPtr, pa);
		b->getRational(pbPtr, pb);
		return Obj_ptr(new RationalObj( *pa * *pb ));
	}
	IntegerObj *pa, *pb;
	Number_ptr paPtr, pbPtr;
	a->getInt(paPtr, pa);
	b->getInt(pbPtr, pb);
	return Obj_ptr(new IntegerObj( *pa * *pb ));
}

Obj_ptr Divide (const Obj_ptr &aPtr, const Obj_ptr &bPtr)
{
	Object *aa = aPtr.get(), *bb = bPtr.get();
	if (aa->Type != Number || bb->Type != Number)
		throw syntaxError("Unexpected type");

	NumberObj *a(static_cast<NumberObj*>(aa)), *b(static_cast<NumberObj*>(bb));

	NumberType aT( a->numType ), bT( b->numType );
	if (aT == Real || bT == Real)
	{
		RealObj *pa, *pb;
		Number_ptr paPtr, pbPtr;
		a->getReal(paPtr, pa);
		b->getReal(pbPtr, pb);
		return Obj_ptr(new RealObj( *pa / *pb ));
	}
	if (aT == Rational || bT == Rational)
	{
		RationalObj *pa, *pb;
		Number_ptr paPtr, pbPtr;
		a->getRational(paPtr, pa);
		b->getRational(pbPtr, pb);
		return Obj_ptr(new RationalObj( *pa / *pb ));
	}
	IntegerObj *pa, *pb;
	Number_ptr paPtr, pbPtr;
	a->getInt(paPtr, pa);
	b->getInt(pbPtr, pb);
	return Obj_ptr(new RationalObj( *pa / *pb ));
}

Obj_ptr Quotient(const Obj_ptr &aPtr, const Obj_ptr &bPtr)
{
	Object *aa = aPtr.get(), *bb = bPtr.get();
	if (aa->Type != Number || bb->Type != Number)
		throw syntaxError("Unexpected type");

	NumberType aT( static_cast<NumberObj*>(aa)->numType ),
			   bT( static_cast<NumberObj*>(bb)->numType );
	if (aT != Integer || bT != Integer)
		throw syntaxError("Unexpected type");

	IntegerObj *a(static_cast<IntegerObj*>(aa)), *b(static_cast<IntegerObj*>(bb));
	if (b->getValue() == 0)
		throw runtimeError("divided by zero");

	Int_ptr ptr( new IntegerObj( a->getValue() / b->getValue() ) );
	return ptr;
}

Obj_ptr Modulo (const Obj_ptr &aPtr, const Obj_ptr &bPtr)
{
	Object *aa = aPtr.get(), *bb = bPtr.get();
	if (aa->Type != Number || bb->Type != Number)
		throw syntaxError("Unexpected type");

	NumberType aT( static_cast<NumberObj*>(aa)->numType ),
			   bT( static_cast<NumberObj*>(bb)->numType );
	if (aT != Integer || bT != Integer)
		throw syntaxError("Unexpected type: 1");

	IntegerObj *a(static_cast<IntegerObj*>(aa)), *b(static_cast<IntegerObj*>(bb));

	Int_ptr ptr( new IntegerObj( Mod( *a, *b) ) );
	return ptr;
}

Obj_ptr Remainder (const Obj_ptr &aPtr, const Obj_ptr &bPtr)
{
	Object *aa = aPtr.get(), *bb = bPtr.get();
	if (aa->Type != Number || bb->Type != Number)
		throw syntaxError("Unexpected type");

	NumberType aT( static_cast<NumberObj*>(aa)->numType ),
			   bT( static_cast<NumberObj*>(bb)->numType );
	if (aT != Integer || bT != Integer)
		throw syntaxError("Unexpected type");

	IntegerObj *a(static_cast<IntegerObj*>(aa)), *b(static_cast<IntegerObj*>(bb));

	Int_ptr ptr( new IntegerObj( Rem(*a, *b) ) );
	return ptr;
}

Obj_ptr GCD (const Obj_ptr &aPtr, const Obj_ptr &bPtr)
{
	Object *aa = aPtr.get(), *bb = bPtr.get();
	if (aa->Type != Number || bb->Type != Number)
		throw syntaxError("Unexpected type");

	NumberType aT( static_cast<NumberObj*>(aa)->numType ),
			   bT( static_cast<NumberObj*>(bb)->numType );
	if (aT != Integer || bT != Integer)
		throw syntaxError("Unexpected type");

	IntegerObj *a(static_cast<IntegerObj*>(aa)), *b(static_cast<IntegerObj*>(bb));
	bigInteger av( a->getValue() ), bv( b->getValue() );
	bigInteger ans;

	Int_ptr ptr( new IntegerObj( countGCD(av, bv) ) );
	return ptr;
}

Obj_ptr LCM (const Obj_ptr &aPtr, const Obj_ptr &bPtr)
{
	Object *aa = aPtr.get(), *bb = bPtr.get();
	if (aa->Type != Number || bb->Type != Number)
		throw syntaxError("Unexpected type");

	NumberType aT( static_cast<NumberObj*>(aa)->numType ),
			   bT( static_cast<NumberObj*>(bb)->numType );
	if (aT != Integer || bT != Integer)
		throw syntaxError("Unexpected type");

	IntegerObj *a(static_cast<IntegerObj*>(aa)), *b(static_cast<IntegerObj*>(bb));
	bigInteger av = a->getValue(), bv = b->getValue();

	Int_ptr ptr( new IntegerObj( countLCM(av, bv) ) );
	return ptr;
}

Obj_ptr Not (const Obj_ptr &aPtr)
{
	Object *a = aPtr.get();
	if (a->Type != Bool)
		throw syntaxError("Unexpected type");
	
	Bool_ptr ptr( new BoolObj( !( *static_cast<BoolObj*>(a) ) ) );
	return ptr;
}

Obj_ptr And (const Obj_ptr &aPtr, const Obj_ptr &bPtr)
{
	Object *a = aPtr.get(), *b = bPtr.get();
	if (a->Type != Bool || b->Type !=Bool)
		throw syntaxError("Unexpected type");
	
	Bool_ptr ptr( new BoolObj(  *static_cast<BoolObj*>(a) && *static_cast<BoolObj*>(b)  ) );
	return ptr;
}

Obj_ptr Or (const Obj_ptr &aPtr, const Obj_ptr &bPtr)
{
	Object *a = aPtr.get(), *b = bPtr.get();
	if (a->Type != Bool || b->Type !=Bool)
		throw syntaxError("Unexpected type");
	
	Bool_ptr ptr( new BoolObj(  *static_cast<BoolObj*>(a) || *static_cast<BoolObj*>(b)  ) );
	return ptr;
}

Obj_ptr descend (const Obj_ptr &aPtr)
{
	if (aPtr == nullptr || aPtr->Type != Number )
		return aPtr;

	NumberType Type = static_cast<NumberObj*>(aPtr.get())->numType;

	if (Type == Integer)
		return aPtr;
	else if (Type == Rational)
	{
		bigRational value( static_cast<RationalObj*>(aPtr.get())->getValue() );
		value.canonicalize();
		if (value.get_den() == 1)
			return Int_ptr( new IntegerObj(value.get_num()) );
		else
			return aPtr;
	}
	else if (Type == Real)
	{
		bigReal value( static_cast<RealObj*>(aPtr.get())->getValue() );
		if (mpf_integer_p(value.get_mpf_t())!=0)
			return Int_ptr(new IntegerObj( realToInt(value) ));
		return aPtr;
	}

	return aPtr;
}

bool emptyPair(const Obj_ptr & obj)
{
	if (obj->Type != Pair)
		return false;
	if (static_cast<PairObj*>(obj.get())->getCar() == nullptr &&
		static_cast<PairObj*>(obj.get())->getCdr() == nullptr)
		return true;
	return false;
}
