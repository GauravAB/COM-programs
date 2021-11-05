#ifndef __CLASSFACTORY_H_
#define __CLASSFACTORY_H_


void InitializeRegisterServer(void);
void UninitializeRegisterServer(void);
int MyCreateProcess(TCHAR *szPath);
STDAPI DllCallCubeOfNumber(int num, int* res);
STDAPI DllCallCubeRootOfNumber(int num, int* res);

#endif