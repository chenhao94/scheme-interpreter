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
#include <string>
#include <memory>

enum objType {Bool, Char, Number, String, Pair, Symbol, Procedure};

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

class Object
{
	private:

		objType Type;

	public:

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

		friend bool operator== (BoolObj, BoolObj);
			
		friend BoolObj operator&& (BoolObj, BoolObj);

		friend BoolObj operator|| (BoolObj, BoolObj);

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

		friend bool operator== (CharObj, CharObj);

};

class NumberObj: public Object
{
	protected:
		
		enum NumberType { Integer, Rational, Real/*, Complex */};	// First 3 are available now

	private:

		NumberType numType;

	public:

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

		friend IntegerObj operator+ (IntegerObj, IntegerObj);
		
		friend IntegerObj operator- (IntegerObj, IntegerObj);
		
		friend IntegerObj operator* (IntegerObj, IntegerObj);
		
		friend RationalObj operator/ (IntegerObj, IntegerObj);
		
		friend bool operator< (IntegerObj, IntegerObj);
		
		friend bool operator> (IntegerObj, IntegerObj);
		
		friend bool operator== (IntegerObj, IntegerObj);
		
		friend bool operator<= (IntegerObj, IntegerObj);
		
		friend bool operator>= (IntegerObj, IntegerObj);
		
		friend bool operator!= (IntegerObj, IntegerObj);
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

		friend RationalObj operator+ (RationalObj, RationalObj);
		
		friend RationalObj operator- (RationalObj, RationalObj);
		
		friend RationalObj operator* (RationalObj, RationalObj);
		
		friend RationalObj operator/ (RationalObj, RationalObj);
		
		friend bool operator< (RationalObj, RationalObj);
		
		friend bool operator> (RationalObj, RationalObj);
		
		friend bool operator== (RationalObj, RationalObj);
		
		friend bool operator<= (RationalObj, RationalObj);
		
		friend bool operator>= (RationalObj, RationalObj);
		
		friend bool operator!= (RationalObj, RationalObj);

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

		friend RealObj operator+ (RealObj, RealObj);
		
		friend RealObj operator- (RealObj, RealObj);
		
		friend RealObj operator* (RealObj, RealObj);
		
		friend RealObj operator/ (RealObj, RealObj);
		
		friend bool operator< (RealObj, RealObj);
		
		friend bool operator> (RealObj, RealObj);
		
		friend bool operator== (RealObj, RealObj);
		
		friend bool operator<= (RealObj, RealObj);
		
		friend bool operator>= (RealObj, RealObj);
		
		friend bool operator!= (RealObj, RealObj);

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

		friend bool operator== (StringObj, StringObj);

		friend StringObj operator+ (StringObj, StringObj);

};

class PairObj: public Object
{
	private:

		std::shared_ptr<Object> obj1, obj2;

	public:

		PairObj(std::shared_ptr<Object> o1, std::shared_ptr<Object> o2): Object(Pair), obj1(o1), obj2(o2) {}

		virtual ~PairObj() {}

		PairObj(const PairObj & obj) : Object(Pair), obj1(obj.obj1), obj2(obj.obj2) {}

		std::string ExternalRep();

		std::shared_ptr<Object> getCar() const { return obj1; }

		std::shared_ptr<Object> getCdr() const { return obj2; }
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

#endif
