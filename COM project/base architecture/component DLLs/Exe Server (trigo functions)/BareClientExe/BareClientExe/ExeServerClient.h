#ifndef __CLASSFACTORY_H_
#define __CLASSFACTORY_H_


void InitializeRegisterServer(void);
void UninitializeRegisterServer(void);
int MyCreateProcess(TCHAR *szPath);

STDAPI DllCallgetSine(float,float*);
STDAPI DllCallgetCosine(float,float*);
STDAPI DllCallgetTangent(float,float*);
STDAPI DllActivateTrigoLibrary(void);



#endif