#ifndef __EXESERVER_CLIENT__
#define __EXESERVER_CLIENT__
#pragma once

void InitializeRegisterServer(void);
void InitializeUnregisterServer(void);
void InitializeComponent(void);
void UninitializeComponent(void);

STDAPI DllCallgetSine(float, float*);
STDAPI DllCallgetCoSine(float, float*);
STDAPI DllCallgetTangent(float, float*);










#endif
