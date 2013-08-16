/**
 *  this file is used for disposing the display requests
 */
#include "iostream"
#include "display.h"

void Display(const Obj_ptr & obj)
{
	using namespace std;

	if (obj->Type == String)
		cout << static_cast<StringObj*>(obj.get())->getValue() << endl;
	else if (obj->Type == Char)
		cout << static_cast<CharObj*>(obj.get())->getValue() << endl;
	else
	 cout << obj->ExternalRep() << endl;
}

