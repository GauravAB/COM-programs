#pragma once

void InitializeRegisterServer(void);
void InitializeUnRegisterServer(void);
int MyCreateProcess(TCHAR *szPath);
void getMonikerAndBinder(void);
STDAPI DllGetNextOperator(int seed, int *index);

//global variable declarations
IRandomNumber *pIRandomNumber = NULL;
IRandomNumberFactory *pIRandomNumberFactory = NULL;
IBindCtx *pIBindCtx = NULL;
IMoniker *pIMoniker = NULL;

