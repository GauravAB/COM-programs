#include <iostream>
#include <Windows.h>


using namespace std;

typedef IUnknown*(*CREATEFUNCPTR)();

IUnknown *callCreateInstance(char *name)
{
	//load dynamic link library into process

	HINSTANCE hComponent = LoadLibrary(name);

	if (hComponent == NULL)
	{
		cout << "Call CreateInstance: \t Error : Cannot Load component " << endl;
		return NULL;
	}
	else
	{
		std::cout << "Library " << name << "Loaded Successfulyl" << endl;
	}


	//Get address of Create Instance function
	
	CREATEFUNCPTR CreateInstance = (CREATEFUNCPTR)GetProcAddress(hComponent, "CreateInstance");

	if (CreateInstance == NULL)
	{
		cout << "CallCreateInstance: \tError" << endl;
		return NULL;
	}
	else
	{
		std::cout<<"Instance Function found" << std::endl;
	}

	std::cout << "CreateInstance returning Instance.." << std::endl;
	return CreateInstance();
}


