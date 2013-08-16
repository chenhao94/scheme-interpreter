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
#include "error.h"
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
class Environment;

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
typedef std::shared_ptr<Environment> env_ptr;

bool equal (const Obj_ptr &aPtr, const Obj_ptr &bPtr);
bool notEqual (const Obj_ptr &aPtr, const Obj_ptr &bPtr);
bool lessThan (const Obj_ptr &aPtr, const Obj_ptr &bPtr);
bool greaterThan (const Obj_ptr &aPtr, const Obj_ptr &bPtr);
bool lessEq (const Obj_ptr &aPtr, const Obj_ptr &bPtr);
bool greaterEq (const Obj_ptr &aPtr, const Obj_ptr &bPtr);
Obj_ptr Add (const Obj_ptr &aPtr, const Obj_ptr &bPtr);
Obj_ptr Subtract (const Obj_ptr &aPtr, const Obj_ptr &bPtr);
Obj_ptr Multiply (const Obj_ptr &aPtr, const Obj_ptr &bPtr);
Obj_ptr Divide (const Obj_ptr &aPtr, const Obj_ptr &bPtr);
Obj_ptr Quotient(const Obj_ptr &aPtr, const Obj_ptr &bPtr);
Obj_ptr Modulo (const Obj_ptr &aPtr, const Obj_ptr &bPtr);
Obj_ptr Remainder (const Obj_ptr &aPtr, const Obj_ptr &bPtr);
Obj_ptr GCD (const Obj_ptr &aPtr, const Obj_ptr &bPtr);
Obj_ptr LCM (const Obj_ptr &aPtr, const Obj_ptr &bPtr);
Obj_ptr Not (const Obj_ptr &aPtr);
Obj_ptr And (const Obj_ptr &aPtr, const Obj_ptr &bPtr);
Obj_ptr Or (const Obj_ptr &aPtr, const Obj_ptr &bPtr);
Obj_ptr descend (const Obj_ptr &aPtr);

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
	if (b.getValue() == 0)
		throw runtimeError("divided by zero");
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

		virtual void getInt(Number_ptr &, IntegerObj *&)=0;

		virtual void getRational(Number_ptr &, RationalObj *&)=0;

		virtual void getReal(Number_ptr &, RealObj *&)=0;

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

		void getInt(Number_ptr & ptr, IntegerObj *& obj);

		void getRational(Number_ptr & ptr, RationalObj *& obj);

		void getReal(Number_ptr & ptr, RealObj *& obj);

		friend RationalObj operator/ (const IntegerObj &, const IntegerObj &);
		
		friend IntegerObj Mod(const IntegerObj &, const IntegerObj &);

		friend IntegerObj Rem(const IntegerObj &, const IntegerObj &);
};


class RationalObj: public NumberObj
{
	private:

		bigRational value;

	public:

		RationalObj(bigRational i) : NumberObj(Rational), value(i) { value.canonicalize(); }

		RationalObj(bigInteger a, bigInteger b) : NumberObj(Rational), value(a, b) { value.canonicalize(); }

		RationalObj(IntegerObj a, IntegerObj b) : NumberObj(Rational), value(a.getValue(), b.getValue()) { value.canonicalize(); }

		virtual ~RationalObj() {}

		RationalObj(const RationalObj &obj) : NumberObj(Rational), value(obj.getValue()) {}

		bigRational getValue() const { return value; }

		std::string ExternalRep();

		void getInt(Number_ptr & ptr, IntegerObj *& obj);

		void getRational(Number_ptr & ptr, RationalObj *& obj);

		void getReal(Number_ptr & ptr, RealObj *& obj);

};

class RealObj: public NumberObj
{
	private:

		bigReal value;

	public:

		RealObj(bigReal i) : NumberObj(Real), value(i) {}

		virtual ~RealObj() {}

		RealObj(const RealObj &obj) : NumberObj(Real), value(obj.getValue()) {}

		//RealObj(const RationalObj &obj) : NumberObj(Real), value(obj.getValue()) {}

		bigReal getValue() const { return value; }

		std::string ExternalRep();

		void getInt(Number_ptr & ptr, IntegerObj *& obj);

		void getRational(Number_ptr & ptr, RationalObj *& obj);

		void getReal(Number_ptr & ptr, RealObj *& obj);

};

class StringObj: public Object
{
	private:

		std::string value;

	public:

		StringObj(std::string);

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

		env_ptr env;

		bool builtInFlag;	// indicate whether are build-in procedure or not

		std::string name;	// coordinate with builtInFlag

	public:

		ProcedureObj(const Arg_ptr & h, const ParseTree_ptr & b, env_ptr & _env, const bool & flag = false, const std::string & _name = ""):  Object(Procedure), argHead(h), body(b), env(_env), builtInFlag(flag), name(_name) {}

		virtual ~ProcedureObj() {}

		std::string ExternalRep() { return "#<procedure>"; }

		const Arg_ptr & getArgHead() { return argHead; }

		const ParseTree_ptr & getBody() { return body; }

		env_ptr & getEnv() { return env; }

		const bool & getFlag() { return builtInFlag; }

		const std::string & getName() { return name; }

};

class Arguments
{
	public:

		std::string name;

		bool spaceDelimited;

		Arg_ptr next;

		Arguments(const std::string &nam, const bool & flag): name(nam), spaceDelimited(flag) {}

		virtual ~Arguments() {}
};

class Parameters
{
	public:

		Obj_ptr obj;

		Para_ptr next;

		Parameters(const Obj_ptr & o): obj(o), next(nullptr) {}
};

#endif
