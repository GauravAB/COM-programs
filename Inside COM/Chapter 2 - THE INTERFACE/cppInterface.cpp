#include <iostream>
#include "objbase.h"

//The Two abstract base classes(interface internally structs) represent COM interfaces (similar) because their memory layout is 
//similar to what is expected by the COM specification


//first interface
interface IX
{
	virtual __stdcall void Fx1() = 0;
	virtual __stdcall void Fx2() = 0;
};

//second interface

interface IY
{
	virtual __stdcall void Fy1() = 0;
	virtual __stdcall void Fy2() = 0;
};

class CA : IX ,IY		//multiple interface inheritance
{
public:
	//implementation of abstract base class IX
	virtual void Fx1() { cout << "Fx1" << endl; }
	virtual void Fx2() { cout << "Fx2" << endl; }

	//implementation of abstract base class IY
	virtual void Fy1() { cout << "Fy1" << endl; }
	virtual void Fy2() { cout << "Fy2" << endl; }
};








