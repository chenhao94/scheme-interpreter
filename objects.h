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
#include <useGMP.h>
#include <string>

enum objType {Boolean, Char, Number, String, Pair, Symbol, Procedure};

class Object
{
	private:

		objType Type;

	public:

		Object(const objType &T) : Type(T), refNumber(0) {}

		virtual ~Object() {}

		virtual string ExternalRep()=0;

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

		string ExternalRep();

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

		string ExternalRep();

		friend bool operator== (CharObj, CharObj);

};

class NumberObj: public Object
{
	private:

		static enum NumberType { Interger, Rational, Real/*, Complex */};	// First 3 are available now

		NumberType numType;

	public:

		NumberObj(const NumberType & T): Object(Number), numType(T) {}

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

		string ExternalRep();

		friend bigInteger operator+ (IntegerObj, IntegerObj);
		
		friend bigInteger operator- (IntegerObj, IntegerObj);
		
		friend bigInteger operator* (IntegerObj, IntegerObj);
		
		friend bigInteger operator/ (IntegerObj, IntegerObj);
		
		friend bigInteger operator< (IntegerObj, IntegerObj);
		
		friend bigInteger operator> (IntegerObj, IntegerObj);
		
		friend bigInteger operator== (IntegerObj, IntegerObj);
		
		friend bigInteger operator<= (IntegerObj, IntegerObj);
		
		friend bigInteger operator>= (IntegerObj, IntegerObj);
		
		friend bigInteger operator!= (IntegerObj, IntegerObj);

};


class RationalObj: public NumberObj
{
	private:

		bigRational value;

	public:

		RationalObj(bigRational i) : NumberObj(Rational), value(i) {}

		RationalObj(bigInteger a, bigInteger b) : NumberObj(Rational), value(a, b) {}

		virtual ~RationalObj() {}

		RationalObj(const RationalObj &obj) : NumberObj(Rational), value(obj.getValue()) {}

		RationalObj(const IntegerObj &obj) : NumberObj(Rational), value(obj.getValue()) {}

		bigRational getValue() const { return value; }

		string ExternalRep();

		friend bigRational operator+ (RationalObj, RationalObj);
		
		friend bigRational operator- (RationalObj, RationalObj);
		
		friend bigRational operator* (RationalObj, RationalObj);
		
		friend bigRational operator/ (RationalObj, RationalObj);
		
		friend bigRational operator< (RationalObj, RationalObj);
		
		friend bigRational operator> (RationalObj, RationalObj);
		
		friend bigRational operator== (RationalObj, RationalObj);
		
		friend bigRational operator<= (RationalObj, RationalObj);
		
		friend bigRational operator>= (RationalObj, RationalObj);
		
		friend bigRational operator!= (RationalObj, RationalObj);

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

		string ExternalRep();

		friend bigReal operator+ (RealObj, RealObj);
		
		friend bigReal operator- (RealObj, RealObj);
		
		friend bigReal operator* (RealObj, RealObj);
		
		friend bigReal operator/ (RealObj, RealObj);
		
		friend bigReal operator< (RealObj, RealObj);
		
		friend bigReal operator> (RealObj, RealObj);
		
		friend bigReal operator== (RealObj, RealObj);
		
		friend bigReal operator<= (RealObj, RealObj);
		
		friend bigReal operator>= (RealObj, RealObj);
		
		friend bigReal operator!= (RealObj, RealObj);

};

class StringObj: public Object
{
	private:

		string value;

	public:

		StringObj(string s): Object(String), value(s) {}

		virtual ~StringObj() {}

		StringObj(const StringObj & obj) : Object(String), value(obj.getValue()) {}

		string getValue() const { return value; }

		string ExternalRep();

		friend bool operator== (StringObj, StringObj);

		friend StringObj operator+ (StringObj, StringObj);

};

class PairObj: public Object
{
	private:

		Object o1, o2;

	public:

		PairObj(Object o1, Object o2): Object(Pair), obj1(o1), obj2(o2) {}

		virtual ~PairObj() {}

		PairObj(const PairObj & obj) : Object(Pair), o1(obj.getCar()), o2(obj.getCdr()) {}

		string ExternalRep();

		Object getCar() { return o1; }

		Object getCdr() { return o2; }
};

class SymbolObj: public Object
{
	private:

		string value;

	public:

		SymbolObj(string s): Object(Symbol), value(s) {}

		virtual ~SymbolObj() {}

		SymbolObj(const SymbolObj & obj ): Object(Symbol), value(obj.getValue()) {}

		string getValue() { retrun value; }

		string ExternalRep();

}

#endif
