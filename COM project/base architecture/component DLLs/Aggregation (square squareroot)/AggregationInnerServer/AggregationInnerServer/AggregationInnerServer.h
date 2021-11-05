#ifndef __AGGREGATE_INNER_H_
#define __AGGREGATE_INNER_H_

interface ISquareroot : public IUnknown
{
	virtual HRESULT __stdcall SquarerootOfAnInteger(int, int*)=0;
};

// {E3BD43AE-49D6-44D5-A389-786435F3C77B}
CLSID CLSID_CSquareroot = {0xe3bd43ae, 0x49d6, 0x44d5, 0xa3, 0x89, 0x78, 0x64, 0x35, 0xf3, 0xc7, 0x7b};
// {020849FD-A5D4-4F7C-A628-1682AF41597F}
IID IID_ISquareroot = { 0x20849fd, 0xa5d4, 0x4f7c, 0xa6, 0x28, 0x16, 0x82, 0xaf, 0x41, 0x59, 0x7f};

#endif