#ifndef __CONTAINMENT_H_
#define __CONTAINMENT_H_


void InitializeRegisterServer(void);
void UninitializeRegisterServer(void);
int MyCreateProcess(TCHAR *szPath);
STDAPI DllCallSumOfTwoIntegers(int, int, int*);
STDAPI DllCallSubtractionOfTwoIntegers(int, int, int*);
STDAPI DllCallMultiplicationOfTwoIntegers(int, int, int*);
STDAPI DllCallDivisionOfTwoIntegers(int, int, int*);

#endif
