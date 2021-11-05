#pragma once

void InitializeRegisterServer(void);
void InitializeUnRegisterServer(void);
int MyCreateProcess(TCHAR *szPath);
void getMonikerAndBinder(void);
STDAPI DllGetNextOperator(int seed, int *index);


