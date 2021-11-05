#ifndef __AUTOMATION_H_
#define __AUTOMATION_H_


void InitializeRegisterServer(void);
void UninitializeRegisterServer(void);
int MyCreateProcess(TCHAR *szPath);

STDAPI DllCallCubeOfNumber(int num, int* res);
STDAPI DllCallCubeRootOfNumber(int num, int* res);

#endif