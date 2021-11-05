

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Mon Mar 19 19:16:17 2018
 */
/* Compiler settings for AutomationServerTypeLib.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.00.0603 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __AutomationProxyStubHeader_h__
#define __AutomationProxyStubHeader_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ICubeCuberoot_FWD_DEFINED__
#define __ICubeCuberoot_FWD_DEFINED__
typedef interface ICubeCuberoot ICubeCuberoot;

#endif 	/* __ICubeCuberoot_FWD_DEFINED__ */


#ifndef __CCubeCuberoot_FWD_DEFINED__
#define __CCubeCuberoot_FWD_DEFINED__

#ifdef __cplusplus
typedef class CCubeCuberoot CCubeCuberoot;
#else
typedef struct CCubeCuberoot CCubeCuberoot;
#endif /* __cplusplus */

#endif 	/* __CCubeCuberoot_FWD_DEFINED__ */


/* header files for imported files */
#include "unknwn.h"
#include "oaidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __ICubeCuberoot_INTERFACE_DEFINED__
#define __ICubeCuberoot_INTERFACE_DEFINED__

/* interface ICubeCuberoot */
/* [oleautomation][dual][unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_ICubeCuberoot;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("2BCA012D-29A0-4FE2-9FBC-0D71631F504B")
    ICubeCuberoot : public IDispatch
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE CubeOfNumber( 
            /* [in] */ int __MIDL__ICubeCuberoot0000,
            /* [retval][out] */ int *__MIDL__ICubeCuberoot0001) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CubeRootOfNumber( 
            /* [in] */ int __MIDL__ICubeCuberoot0002,
            /* [retval][out] */ int *__MIDL__ICubeCuberoot0003) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ICubeCuberootVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICubeCuberoot * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICubeCuberoot * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICubeCuberoot * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ICubeCuberoot * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ICubeCuberoot * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ICubeCuberoot * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ICubeCuberoot * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE *CubeOfNumber )( 
            ICubeCuberoot * This,
            /* [in] */ int __MIDL__ICubeCuberoot0000,
            /* [retval][out] */ int *__MIDL__ICubeCuberoot0001);
        
        HRESULT ( STDMETHODCALLTYPE *CubeRootOfNumber )( 
            ICubeCuberoot * This,
            /* [in] */ int __MIDL__ICubeCuberoot0002,
            /* [retval][out] */ int *__MIDL__ICubeCuberoot0003);
        
        END_INTERFACE
    } ICubeCuberootVtbl;

    interface ICubeCuberoot
    {
        CONST_VTBL struct ICubeCuberootVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICubeCuberoot_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICubeCuberoot_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICubeCuberoot_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICubeCuberoot_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ICubeCuberoot_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ICubeCuberoot_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ICubeCuberoot_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ICubeCuberoot_CubeOfNumber(This,__MIDL__ICubeCuberoot0000,__MIDL__ICubeCuberoot0001)	\
    ( (This)->lpVtbl -> CubeOfNumber(This,__MIDL__ICubeCuberoot0000,__MIDL__ICubeCuberoot0001) ) 

#define ICubeCuberoot_CubeRootOfNumber(This,__MIDL__ICubeCuberoot0002,__MIDL__ICubeCuberoot0003)	\
    ( (This)->lpVtbl -> CubeRootOfNumber(This,__MIDL__ICubeCuberoot0002,__MIDL__ICubeCuberoot0003) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICubeCuberoot_INTERFACE_DEFINED__ */



#ifndef __AutomationServerTypeLib_LIBRARY_DEFINED__
#define __AutomationServerTypeLib_LIBRARY_DEFINED__

/* library AutomationServerTypeLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_AutomationServerTypeLib;

EXTERN_C const CLSID CLSID_CCubeCuberoot;

#ifdef __cplusplus

class DECLSPEC_UUID("45BE5516-A88A-44E0-B90F-69F37E876F33")
CCubeCuberoot;
#endif
#endif /* __AutomationServerTypeLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


