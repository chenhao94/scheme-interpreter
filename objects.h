/** 
 * 	This head file aims to dispose the objects. 
 * 	A object must be in one of the following types
 *
 * 	boolean
 * 	char
 * 	number
 * 	string
 * 	pair
 * 	symbol
 * 	procedure	(!)
 * 	vector		(?)
 * 	port		(?)
 *
 */

#ifndef  __OBJECTS_H____
#define  __OBJECTS_H____
#include "useGMP.h"
#include "compilingError.h"
#include <string>
#include <memory>

enum objType {Bool, Char, Number, String, Pair, Symbol, Procedure};
enum NumberType { Integer, Rational, Real/*, Complex */};	// First 3 are available now


class Object;
class BoolObj;
class CharObj;
class NumberObj;
class IntegerObj;
class RationalObj;
class RealObj;
class StringObj;
class PairObj;
class SymbolObj;
class ProcedureObj;
class ParseTree;
class Arguments;
class Parameters;

typedef std::shared_ptr<Object> Obj_ptr;
typedef std::shared_ptr<BoolObj> Bool_ptr;
typedef std::shared_ptr<CharObj> Char_ptr;
typedef std::shared_ptr<NumberObj> Number_ptr;
typedef std::shared_ptr<IntegerObj> Int_ptr;
typedef std::shared_ptr<RationalObj> Rational_ptr;
typedef std::shared_ptr<RealObj> Real_ptr;
typedef std::shared_ptr<StringObj> String_ptr;
typedef std::shared_ptr<PairObj> Pair_ptr;
typedef std::shared_ptr<SymbolObj> Symbol_ptr;
typedef std::shared_ptr<ProcedureObj> Procedure_ptr;
typedef std::shared_ptr<Arguments> Arg_ptr;
typedef std::shared_ptr<Parameters> Para_ptr;
typedef std::shared_ptr<ParseTree> ParseTree_ptr;

template <class T, class U = typename T::__ObjectBaseType>
bool operator==(const T &a, const T &b)
{
	return (a.getValue() == b.getValue());
}

template <class T, class U = typename T::__ObjectBaseType>
T operator+ (const T &a, const T &b)
{
	T ans(a.getValue() + b.getValue());
	return ans;
}

template <class T, class U = typename T::__ObjectBaseType>
T operator- (const T &a, const T &b)
{
	T ans(a.getValue() - b.getValue());
	return ans;
}

template <class T, class U = typename T::__ObjectBaseType>
T operator* (const T &a, const T &b)
{
	T ans(a.getValue() * b.getValue());
	return ans;
}

template <class T, class U = typename T::__ObjectBaseType>
T operator/ (const T &a, const T &b)
{
	T ans(a.getValue() / b.getValue());
	return ans;
}

template <class T, class U = typename T::__ObjectBaseType>
bool operator< (const T &a, const T &b)
{
	return (a.getValue() < b.getValue());
}

template <class T, class U = typename T::__ObjectBaseType>
bool operator> (const T &a, const T &b)
{
	return (a.getValue() > b.getValue());
}

template <class T, class U = typename T::__ObjectBaseType>
bool operator<= (const T &a, const T &b)
{
	return (a.getValue() <= b.getValue());
}

template <class T, class U = typename T::__ObjectBaseType>
bool operator>= (const T &a, const T &b)
{
	return (a.getValue() >= b.getValue());
}

template <class T, class U = typename T::__ObjectBaseType>
bool operator!= (const T &a, const T &b)
{
	return (a.getValue() != b.getValue());
}

class Object
{
	public:
		typedef int __ObjectBaseType;

		objType Type;

		Object(const objType &T) : Type(T) {}

		virtual ~Object() {}

		virtual std::string ExternalRep()=0;

};

class BoolObj: public Object
{
	private:

		bool value;

	public:

		BoolObj(bool b): Object(Bool), value(b) {}

		virtual ~BoolObj() {}

		BoolObj(const BoolObj & obj) : Object(Bool), value(obj.getValue()) {}

		bool getValue() const { return value; }

		BoolObj operator! ();

		std::string ExternalRep();

		friend BoolObj operator&& (const BoolObj &, const BoolObj &);

		friend BoolObj operator|| (const BoolObj &, const BoolObj &);

};

class CharObj: public Object
{
	private:

		char value;

	public:

		CharObj(char c): Object(Char), value(c) {}

		virtual ~CharObj() {}

		CharObj(const CharObj & obj) : Object(Char), value(obj.getValue()) {}

		char getValue() const { return value; }

		std::string ExternalRep();

};

class NumberObj: public Object
{
	public:
		
		NumberType numType;

		NumberObj(const NumberType & T): Object(Number), numType(T) {}

		virtual std::string ExternalRep()=0;

		virtual ~NumberObj() {}

};

class IntegerObj: public NumberObj
{
	private:

		bigInteger value;

	public:

		IntegerObj(bigInteger i) : NumberObj(Integer), value(i) {}

		virtual ~IntegerObj() {}

		IntegerObj(const IntegerObj &obj) : NumberObj(Integer), value(obj.getValue()) {}

		bigInteger getValue() const { return value; }

		std::string ExternalRep();

		friend RationalObj operator/ (const IntegerObj &, const IntegerObj &);
		
		friend IntegerObj operator% (const IntegerObj &, const IntegerObj &);
};


class RationalObj: public NumberObj
{
	private:

		bigRational value;

	public:

		RationalObj(bigRational i) : NumberObj(Rational), value(i) {}

		RationalObj(bigInteger a, bigInteger b) : NumberObj(Rational), value(a, b) {}

		RationalObj(IntegerObj a, IntegerObj b) : NumberObj(Rational), value(a.getValue(), b.getValue()) {}

		virtual ~RationalObj() {}

		RationalObj(const RationalObj &obj) : NumberObj(Rational), value(obj.getValue()) {}

		RationalObj(const IntegerObj &obj) : NumberObj(Rational), value(obj.getValue()) {}

		bigRational getValue() const { return value; }

		std::string ExternalRep();

};

class RealObj: public NumberObj
{
	private:

		bigReal value;

	public:

		RealObj(bigReal i) : NumberObj(Real), value(i) {}

		virtual ~RealObj() {}

		RealObj(const RealObj &obj) : NumberObj(Real), value(obj.getValue()) {}

		RealObj(const RationalObj &obj) : NumberObj(Real), value(obj.getValue()) {}

		bigReal getValue() const { return value; }

		std::string ExternalRep();

};

class StringObj: public Object
{
	private:

		std::string value;

	public:

		StringObj(std::string s): Object(String), value(s) {}

		virtual ~StringObj() {}

		StringObj(const StringObj & obj) : Object(String), value(obj.getValue()) {}

		std::string getValue() const { return value; }

		std::string ExternalRep();

};

class PairObj: public Object
{
	private:

		Obj_ptr obj1, obj2;

	public:

		PairObj(Obj_ptr o1, Obj_ptr o2): Object(Pair), obj1(o1), obj2(o2) {}

		virtual ~PairObj() {}

		PairObj(const PairObj & obj) : Object(Pair), obj1(obj.obj1), obj2(obj.obj2) {}

		std::string ExternalRep();

		Obj_ptr getCar() const { return obj1; }

		Obj_ptr getCdr() const { return obj2; }

		friend bool operator== (const PairObj &, const PairObj &);

};

class SymbolObj: public Object
{
	private:

		std::string value;

	public:

		SymbolObj(std::string s): Object(Symbol), value(s) {}

		virtual ~SymbolObj() {}

		SymbolObj(const SymbolObj & obj ): Object(Symbol), value(obj.value) {}

		std::string getValue() const { return value; }

		std::string ExternalRep();

};

class ProcedureObj: public Object
{

	private:

		Arg_ptr argHead;

		ParseTree_ptr body;

	public:

		ProcedureObj(const Arg_ptr & h, const ParseTree_ptr & b):  Object(Procedure), argHead(h), body(b){}

		virtual ~ProcedureObj() {}

		std::string ExternalRep() { return "#<procedure>"; }

};

class Arguments
{
	private:

		std::string name;

		bool spaceDelimited;

	public:

		Arg_ptr next;

		Arguments(const std::string &nam, const bool & flag): name(nam), spaceDelimited(flag) {}

		virtual ~Arguments() {}
};

class Parameters
{
	private:

		Obj_ptr obj;

	public:

		Para_ptr next;

		Parameters(const Obj_ptr & o): obj(o), next(NULL) {}
};

bool operator== (const Obj_ptr &, const Obj_ptr &);

#endif
