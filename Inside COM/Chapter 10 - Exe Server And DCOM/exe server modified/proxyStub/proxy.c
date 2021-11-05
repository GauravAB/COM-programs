

/* this ALWAYS GENERATED file contains the proxy stub code */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Thu Feb 22 07:14:48 2018
 */
/* Compiler settings for ProxyStub.idl:
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


#include "iface.h"

#define TYPE_FORMAT_STRING_SIZE   7                                 
#define PROC_FORMAT_STRING_SIZE   49                                
#define EXPR_FORMAT_STRING_SIZE   1                                 
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   0            

typedef struct _ProxyStub_MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } ProxyStub_MIDL_TYPE_FORMAT_STRING;

typedef struct _ProxyStub_MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } ProxyStub_MIDL_PROC_FORMAT_STRING;

typedef struct _ProxyStub_MIDL_EXPR_FORMAT_STRING
    {
    long          Pad;
    unsigned char  Format[ EXPR_FORMAT_STRING_SIZE ];
    } ProxyStub_MIDL_EXPR_FORMAT_STRING;


static const RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};


extern const ProxyStub_MIDL_TYPE_FORMAT_STRING ProxyStub__MIDL_TypeFormatString;
extern const ProxyStub_MIDL_PROC_FORMAT_STRING ProxyStub__MIDL_ProcFormatString;
extern const ProxyStub_MIDL_EXPR_FORMAT_STRING ProxyStub__MIDL_ExprFormatString;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO ISum_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO ISum_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO ISubtract_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO ISubtract_ProxyInfo;



#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif

#if !(TARGET_IS_NT50_OR_LATER)
#error You need Windows 2000 or later to run this stub because it uses these features:
#error   /robust command line switch.
#error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
#error This app will fail with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const ProxyStub_MIDL_PROC_FORMAT_STRING ProxyStub__MIDL_ProcFormatString =
    {
        0,
        {

	/* Procedure SubtractionofTwoIntegers */


	/* Procedure SumofTwoIntegers */

			0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x3 ),	/* 3 */
/*  8 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 10 */	NdrFcShort( 0x2c ),	/* 44 */
/* 12 */	NdrFcShort( 0x24 ),	/* 36 */
/* 14 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x4,		/* 4 */
/* 16 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 18 */	NdrFcShort( 0x0 ),	/* 0 */
/* 20 */	NdrFcShort( 0x0 ),	/* 0 */
/* 22 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter __MIDL__ISubtract0000 */


	/* Parameter __MIDL__ISum0000 */

/* 24 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 26 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 28 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter __MIDL__ISubtract0001 */


	/* Parameter __MIDL__ISum0001 */

/* 30 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 32 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 34 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter __MIDL__ISubtract0002 */


	/* Parameter __MIDL__ISum0002 */

/* 36 */	NdrFcShort( 0x158 ),	/* Flags:  in, out, base type, simple ref, */
/* 38 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 40 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */

/* 42 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 44 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 46 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

			0x0
        }
    };

static const ProxyStub_MIDL_TYPE_FORMAT_STRING ProxyStub__MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */
/*  2 */	
			0x11, 0x8,	/* FC_RP [simple_pointer] */
/*  4 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */

			0x0
        }
    };


/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: ISum, ver. 0.0,
   GUID={0x727E87DA,0xD534,0x47F2,{0xB9,0x62,0x1F,0xC8,0x87,0xD4,0xAB,0x10}} */

#pragma code_seg(".orpc")
static const unsigned short ISum_FormatStringOffsetTable[] =
    {
    0
    };

static const MIDL_STUBLESS_PROXY_INFO ISum_ProxyInfo =
    {
    &Object_StubDesc,
    ProxyStub__MIDL_ProcFormatString.Format,
    &ISum_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO ISum_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    ProxyStub__MIDL_ProcFormatString.Format,
    &ISum_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(4) _ISumProxyVtbl = 
{
    &ISum_ProxyInfo,
    &IID_ISum,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *) (INT_PTR) -1 /* ISum::SumofTwoIntegers */
};

const CInterfaceStubVtbl _ISumStubVtbl =
{
    &IID_ISum,
    &ISum_ServerInfo,
    4,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
};


/* Object interface: ISubtract, ver. 0.0,
   GUID={0x0F448DFF,0xC4B8,0x4061,{0xA3,0x40,0x0D,0x0B,0x4F,0x94,0x6F,0x2A}} */

#pragma code_seg(".orpc")
static const unsigned short ISubtract_FormatStringOffsetTable[] =
    {
    0
    };

static const MIDL_STUBLESS_PROXY_INFO ISubtract_ProxyInfo =
    {
    &Object_StubDesc,
    ProxyStub__MIDL_ProcFormatString.Format,
    &ISubtract_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO ISubtract_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    ProxyStub__MIDL_ProcFormatString.Format,
    &ISubtract_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(4) _ISubtractProxyVtbl = 
{
    &ISubtract_ProxyInfo,
    &IID_ISubtract,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *) (INT_PTR) -1 /* ISubtract::SubtractionofTwoIntegers */
};

const CInterfaceStubVtbl _ISubtractStubVtbl =
{
    &IID_ISubtract,
    &ISubtract_ServerInfo,
    4,
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
    ProxyStub__MIDL_TypeFormatString.Format,
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

const CInterfaceProxyVtbl * const _ProxyStub_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_ISumProxyVtbl,
    ( CInterfaceProxyVtbl *) &_ISubtractProxyVtbl,
    0
};

const CInterfaceStubVtbl * const _ProxyStub_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_ISumStubVtbl,
    ( CInterfaceStubVtbl *) &_ISubtractStubVtbl,
    0
};

PCInterfaceName const _ProxyStub_InterfaceNamesList[] = 
{
    "ISum",
    "ISubtract",
    0
};


#define _ProxyStub_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _ProxyStub, pIID, n)

int __stdcall _ProxyStub_IID_Lookup( const IID * pIID, int * pIndex )
{
    IID_BS_LOOKUP_SETUP

    IID_BS_LOOKUP_INITIAL_TEST( _ProxyStub, 2, 1 )
    IID_BS_LOOKUP_RETURN_RESULT( _ProxyStub, 2, *pIndex )
    
}

const ExtendedProxyFileInfo ProxyStub_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _ProxyStub_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _ProxyStub_StubVtblList,
    (const PCInterfaceName * ) & _ProxyStub_InterfaceNamesList,
    0, /* no delegation */
    & _ProxyStub_IID_Lookup, 
    2,
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

