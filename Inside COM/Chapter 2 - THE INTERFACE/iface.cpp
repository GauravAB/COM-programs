#include <iostream>
#include "objbase.h"

using namespace std;


void trace(const char* pMsg)
{
	cout << pMsg << endl;
}

//Abstract Interfaces

interface IX
{
	virtual void __stdcall Fx1() = 0;
	virtual void __stdcall Fx2() = 0;
};

interface IY
{
	virtual void __stdcall Fy1() = 0;
	virtual void __stdcall Fy2() = 0;
};

//Interface Implementation

class CA : public IX, public IY
{
	//Implement interface IX
	virtual void __stdcall Fx1() { cout << "Fx1" << endl; }
	virtual void __stdcall Fx2() { cout << "Fx2" << endl; }

	//Implement interface IY
	virtual void __stdcall Fy1() { cout << "Fy1" << endl; }
	virtual void __stdcall Fy2() { cout << "Fy2" << endl; }

};

//Client

int main()
{
	trace("Client: Create an instance of the component.");
	CA* pA = new CA;

	//Get an IX pointer 
	IX* pIX = pA;

	trace("Client: use the IX interface.");
	pIX->Fx1();
	pIX->Fx2();

	//Get an IY pointer

	IY *pIY = pA;

	trace("Client: Use the IY interface");
	pIY->Fy1();
	pIY->Fy2();

	trace("Client : Delete the Component.");
	delete pA;
	return 0;
}














