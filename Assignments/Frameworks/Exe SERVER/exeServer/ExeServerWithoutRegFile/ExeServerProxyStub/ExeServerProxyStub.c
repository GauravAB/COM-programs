

/* this ALWAYS GENERATED file contains the proxy stub code */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Tue Mar 20 05:10:59 2018
 */
/* Compiler settings for ProxyStubExeServer.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.00.0603 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#if !defined(_M_IA64) && !defined(_M_AMD64) && !defined(_ARM_)


#pragma warning( disable: 4049 )  /* more than 64k source lines */
#if _MSC_VER >= 1200
#pragma warning(push)
#endif

#pragma warning( disable: 4211 )  /* redefine extern to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#pragma warning( disable: 4024 )  /* array to pointer mapping*/
#pragma warning( disable: 4152 )  /* function/data pointer conversion in expression */
#pragma warning( disable: 4100 ) /* unreferenced arguments in x86 call */

#pragma optimize("", off ) 

#define USE_STUBLESS_PROXY


/* verify that the <rpcproxy.h> version is high enough to compile this file*/
#ifndef __REDQ_RPCPROXY_H_VERSION__
#define __REQUIRED_RPCPROXY_H_VERSION__ 475
#endif


#include "rpcproxy.h"
#ifndef __RPCPROXY_H_VERSION__
#error this stub requires an updated version of <rpcproxy.h>
#endif /* __RPCPROXY_H_VERSION__ */


#include "ExeServerProxyStubHeader.h"

#define TYPE_FORMAT_STRING_SIZE   7                                 
#define PROC_FORMAT_STRING_SIZE   127                               
#define EXPR_FORMAT_STRING_SIZE   1                                 
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   0            

typedef struct _ProxyStubExeServer_MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } ProxyStubExeServer_MIDL_TYPE_FORMAT_STRING;

typedef struct _ProxyStubExeServer_MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } ProxyStubExeServer_MIDL_PROC_FORMAT_STRING;

typedef struct _ProxyStubExeServer_MIDL_EXPR_FORMAT_STRING
    {
    long          Pad;
    unsigned char  Format[ EXPR_FORMAT_STRING_SIZE ];
    } ProxyStubExeServer_MIDL_EXPR_FORMAT_STRING;


static const RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};


extern const ProxyStubExeServer_MIDL_TYPE_FORMAT_STRING ProxyStubExeServer__MIDL_TypeFormatString;
extern const ProxyStubExeServer_MIDL_PROC_FORMAT_STRING ProxyStubExeServer__MIDL_ProcFormatString;
extern const ProxyStubExeServer_MIDL_EXPR_FORMAT_STRING ProxyStubExeServer__MIDL_ExprFormatString;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO ITrigoFunction_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO ITrigoFunction_ProxyInfo;



#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif

#if !(TARGET_IS_NT50_OR_LATER)
#error You need Windows 2000 or later to run this stub because it uses these features:
#error   /robust command line switch.
#error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
#error This app will fail with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const ProxyStubExeServer_MIDL_PROC_FORMAT_STRING ProxyStubExeServer__MIDL_ProcFormatString =
    {
        0,
        {

	/* Procedure getSine */

			0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x3 ),	/* 3 */
/*  8 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 10 */	NdrFcShort( 0x24 ),	/* 36 */
/* 12 */	NdrFcShort( 0x24 ),	/* 36 */
/* 14 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 16 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 18 */	NdrFcShort( 0x0 ),	/* 0 */
/* 20 */	NdrFcShort( 0x0 ),	/* 0 */
/* 22 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter __MIDL__ITrigoFunction0000 */

/* 24 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 26 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 28 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter __MIDL__ITrigoFunction0001 */

/* 30 */	NdrFcShort( 0x158 ),	/* Flags:  in, out, base type, simple ref, */
/* 32 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 34 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 36 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 38 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 40 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getCosine */

/* 42 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 44 */	NdrFcLong( 0x0 ),	/* 0 */
/* 48 */	NdrFcShort( 0x4 ),	/* 4 */
/* 50 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 52 */	NdrFcShort( 0x24 ),	/* 36 */
/* 54 */	NdrFcShort( 0x24 ),	/* 36 */
/* 56 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 58 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 60 */	NdrFcShort( 0x0 ),	/* 0 */
/* 62 */	NdrFcShort( 0x0 ),	/* 0 */
/* 64 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter __MIDL__ITrigoFunction0002 */

/* 66 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 68 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 70 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter __MIDL__ITrigoFunction0003 */

/* 72 */	NdrFcShort( 0x158 ),	/* Flags:  in, out, base type, simple ref, */
/* 74 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 76 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 78 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 80 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 82 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getTangent */

/* 84 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 86 */	NdrFcLong( 0x0 ),	/* 0 */
/* 90 */	NdrFcShort( 0x5 ),	/* 5 */
/* 92 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 94 */	NdrFcShort( 0x24 ),	/* 36 */
/* 96 */	NdrFcShort( 0x24 ),	/* 36 */
/* 98 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 100 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 102 */	NdrFcShort( 0x0 ),	/* 0 */
/* 104 */	NdrFcShort( 0x0 ),	/* 0 */
/* 106 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter __MIDL__ITrigoFunction0004 */

/* 108 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 110 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 112 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter __MIDL__ITrigoFunction0005 */

/* 114 */	NdrFcShort( 0x158 ),	/* Flags:  in, out, base type, simple ref, */
/* 116 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 118 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 120 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 122 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 124 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

			0x0
        }
    };

static const ProxyStubExeServer_MIDL_TYPE_FORMAT_STRING ProxyStubExeServer__MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */
/*  2 */	
			0x11, 0x8,	/* FC_RP [simple_pointer] */
/*  4 */	0xa,		/* FC_FLOAT */
			0x5c,		/* FC_PAD */

			0x0
        }
    };


/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: ITrigoFunction, ver. 0.0,
   GUID={0xB1FFB522,0x82D0,0x43FE,{0x95,0xA7,0xD9,0x88,0xFD,0x62,0x5E,0x86}} */

#pragma code_seg(".orpc")
static const unsigned short ITrigoFunction_FormatStringOffsetTable[] =
    {
    0,
    42,
    84
    };

static const MIDL_STUBLESS_PROXY_INFO ITrigoFunction_ProxyInfo =
    {
    &Object_StubDesc,
    ProxyStubExeServer__MIDL_ProcFormatString.Format,
    &ITrigoFunction_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO ITrigoFunction_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    ProxyStubExeServer__MIDL_ProcFormatString.Format,
    &ITrigoFunction_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(6) _ITrigoFunctionProxyVtbl = 
{
    &ITrigoFunction_ProxyInfo,
    &IID_ITrigoFunction,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *) (INT_PTR) -1 /* ITrigoFunction::getSine */ ,
    (void *) (INT_PTR) -1 /* ITrigoFunction::getCosine */ ,
    (void *) (INT_PTR) -1 /* ITrigoFunction::getTangent */
};

const CInterfaceStubVtbl _ITrigoFunctionStubVtbl =
{
    &IID_ITrigoFunction,
    &ITrigoFunction_ServerInfo,
    6,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
};

static const MIDL_STUB_DESC Object_StubDesc = 
    {
    0,
    NdrOleAllocate,
    NdrOleFree,
    0,
    0,
    0,
    0,
    0,
    ProxyStubExeServer__MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x50002, /* Ndr library version */
    0,
    0x800025b, /* MIDL Version 8.0.603 */
    0,
    0,
    0,  /* notify & notify_flag routine table */
    0x1, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0
    };

const CInterfaceProxyVtbl * const _ProxyStubExeServer_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_ITrigoFunctionProxyVtbl,
    0
};

const CInterfaceStubVtbl * const _ProxyStubExeServer_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_ITrigoFunctionStubVtbl,
    0
};

PCInterfaceName const _ProxyStubExeServer_InterfaceNamesList[] = 
{
    "ITrigoFunction",
    0
};


#define _ProxyStubExeServer_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _ProxyStubExeServer, pIID, n)

int __stdcall _ProxyStubExeServer_IID_Lookup( const IID * pIID, int * pIndex )
{
    
    if(!_ProxyStubExeServer_CHECK_IID(0))
        {
        *pIndex = 0;
        return 1;
        }

    return 0;
}

const ExtendedProxyFileInfo ProxyStubExeServer_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _ProxyStubExeServer_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _ProxyStubExeServer_StubVtblList,
    (const PCInterfaceName * ) & _ProxyStubExeServer_InterfaceNamesList,
    0, /* no delegation */
    & _ProxyStubExeServer_IID_Lookup, 
    1,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
#pragma optimize("", on )
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* !defined(_M_IA64) && !defined(_M_AMD64) && !defined(_ARM_) */

