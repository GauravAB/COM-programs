#include "CFactory.h"
#include "interface.h"
#include "Component1.h"
#include "Component2.h"
#include "Component3.h"

//The FactoryData array contains the components that can be served


//The following array contains the data used by CFactory
//to create components, Each element in the array contains the CLSID ,
//the pointer to the creation function, and  the name of the component
//to place in the registry


CFactoryData g_FactoryDataArray[] = 
{
	{
		&CLSID_Component1,								//address of class id
		CA::CreateInstance,								//address CreateInstance
		"Inside COM ,Chapter 9 example , Component1",	//szName
		"Inside COM.Chap09.Cmpnt1.1",					//Ver dependent progID
		"Inside COM.Chap09.Cmpnt1",						//Ver Independent progID
	},

	{
		&CLSID_Component2,
		CB::CreateInstance,
		"Inside COM ,Chapter 9 example , Component2",
		"Inside COM.Chap09.Cmpn2.1",
		"Inside COM.Chap09.Cmpnt2", 
	},

	{
		 &CLSID_Component3,
		 CC::CreateInstance,
		 "Inside COM ,Chapter 9 example , Component3",
		 "Inside COM.Chap09.Cmpnt3.1",
		 "Inside COM.Chap09.Cmpnt3", 
	},

};


int g_cFactoryDataEntries = sizeof(g_FactoryDataArray) / sizeof(CFactoryData);