#ifndef __CLASSFACTORY_H_
#define __CLASSFACTORY_H_


void InitializeRegisterServer(void);
void UninitializeRegisterServer(void);
int MyCreateProcess(TCHAR *szPath);

STDAPI DllCallSquareOfANumber(int, int*);
STDAPI DllCallSquarerootOfANumber(int, int*);

#endif