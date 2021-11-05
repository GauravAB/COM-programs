

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Fri Feb 16 00:45:52 2018
 */
/* Compiler settings for Server.IDL:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.00.0603 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_IX,0xAB577229,0x9BB9,0x4589,0xBD,0x83,0x02,0x47,0xE8,0x19,0x50,0x07);


MIDL_DEFINE_GUID(IID, IID_IY,0x70467C73,0xF547,0x42E3,0x97,0xE0,0xE5,0x20,0x07,0x99,0x4C,0x28);


MIDL_DEFINE_GUID(IID, IID_IZ,0x80905F1B,0xC703,0x4BDC,0xA1,0xBA,0x79,0x59,0x02,0xC1,0xF7,0xAB);


MIDL_DEFINE_GUID(IID, LIBID_ServerLib,0x2D40547C,0xD1FA,0x49C1,0x84,0x1B,0xE6,0x9D,0xF6,0xCA,0x66,0x71);


MIDL_DEFINE_GUID(CLSID, CLSID_Component1,0x37CF8074,0x8BC1,0x45EC,0xBB,0xF0,0x73,0xAD,0x30,0xEB,0x99,0xFB);


MIDL_DEFINE_GUID(CLSID, CLSID_Component2,0x4E76BCAA,0x0AFA,0x4AC0,0xB7,0xE8,0x00,0x77,0x70,0xE8,0xF8,0xE8);


MIDL_DEFINE_GUID(CLSID, CLSID_Component3,0x46549E71,0xCE86,0x4680,0xA0,0xDF,0x3A,0x9D,0xDE,0x50,0x3C,0x93);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



