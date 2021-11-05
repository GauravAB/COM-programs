#ifndef __AUTOMATION_SERVER_
#define __AUTOMATION_SERVER_

#pragma once

class ICubeCuberoot : public IDispatch
{
public:
	//pure virtual
	virtual HRESULT __stdcall CubeOfNumber(int num , int* res) = 0;
	virtual HRESULT __stdcall CubeRootOfNumber(int num , int*res) = 0;
};

// {45BE5516-A88A-44E0-B90F-69F37E876F33}
const CLSID CLSID_CCubeCubeRoot = { 0x45be5516, 0xa88a, 0x44e0, 0xb9, 0xf, 0x69, 0xf3, 0x7e, 0x87, 0x6f, 0x33};
// {2BCA012D-29A0-4FE2-9FBC-0D71631F504B}
const IID IID_ICubeCuberoot = { 0x2bca012d, 0x29a0, 0x4fe2, 0x9f, 0xbc, 0xd, 0x71, 0x63, 0x1f, 0x50, 0x4b};

#endif