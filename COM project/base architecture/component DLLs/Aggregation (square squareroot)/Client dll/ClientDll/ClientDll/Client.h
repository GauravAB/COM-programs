#ifndef __CLASSFACTORY_H_
#define __CLASSFACTORY_H_


void InitializeRegisterServer(void);
void UninitializeRegisterServer(void);
int MyCreateProcess(TCHAR *szPath);
STDAPI DllCallSumOfTwoIntegers(int, int, int*);
STDAPI DllCallSubtractionOfTwoIntegers(int, int, int*);
STDAPI DllCallMultiplicationOfTwoIntegers(int, int, int*);
STDAPI DllCallDivisionOfTwoIntegers(int, int, int*);
STDAPI DllCallSquareOfANumber(int, int*);
STDAPI DllCallSquarerootOfANumber(int, int*);

#endif