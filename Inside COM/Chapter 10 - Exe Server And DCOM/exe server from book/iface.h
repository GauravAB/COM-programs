

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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

#ifndef __iface_h__
#define __iface_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IX_FWD_DEFINED__
#define __IX_FWD_DEFINED__
typedef interface IX IX;

#endif 	/* __IX_FWD_DEFINED__ */


#ifndef __IY_FWD_DEFINED__
#define __IY_FWD_DEFINED__
typedef interface IY IY;

#endif 	/* __IY_FWD_DEFINED__ */


#ifndef __IZ_FWD_DEFINED__
#define __IZ_FWD_DEFINED__
typedef interface IZ IZ;

#endif 	/* __IZ_FWD_DEFINED__ */


#ifndef __Component1_FWD_DEFINED__
#define __Component1_FWD_DEFINED__

#ifdef __cplusplus
typedef class Component1 Component1;
#else
typedef struct Component1 Component1;
#endif /* __cplusplus */

#endif 	/* __Component1_FWD_DEFINED__ */


#ifndef __Component2_FWD_DEFINED__
#define __Component2_FWD_DEFINED__

#ifdef __cplusplus
typedef class Component2 Component2;
#else
typedef struct Component2 Component2;
#endif /* __cplusplus */

#endif 	/* __Component2_FWD_DEFINED__ */


#ifndef __Component3_FWD_DEFINED__
#define __Component3_FWD_DEFINED__

#ifdef __cplusplus
typedef class Component3 Component3;
#else
typedef struct Component3 Component3;
#endif /* __cplusplus */

#endif 	/* __Component3_FWD_DEFINED__ */


/* header files for imported files */
#include "unknwn.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IX_INTERFACE_DEFINED__
#define __IX_INTERFACE_DEFINED__

/* interface IX */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IX;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("AB577229-9BB9-4589-BD83-0247E8195007")
    IX : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE FxStringIn( 
            /* [string][in] */ wchar_t *szIn) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE FxStringOut( 
            /* [string][out] */ wchar_t **szOut) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IXVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IX * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IX * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IX * This);
        
        HRESULT ( STDMETHODCALLTYPE *FxStringIn )( 
            IX * This,
            /* [string][in] */ wchar_t *szIn);
        
        HRESULT ( STDMETHODCALLTYPE *FxStringOut )( 
            IX * This,
            /* [string][out] */ wchar_t **szOut);
        
        END_INTERFACE
    } IXVtbl;

    interface IX
    {
        CONST_VTBL struct IXVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IX_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IX_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IX_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IX_FxStringIn(This,szIn)	\
    ( (This)->lpVtbl -> FxStringIn(This,szIn) ) 

#define IX_FxStringOut(This,szOut)	\
    ( (This)->lpVtbl -> FxStringOut(This,szOut) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IX_INTERFACE_DEFINED__ */


#ifndef __IY_INTERFACE_DEFINED__
#define __IY_INTERFACE_DEFINED__

/* interface IY */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IY;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("70467C73-F547-42E3-97E0-E52007994C28")
    IY : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE FyCount( 
            /* [out] */ long *sizeArray) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE FyArrayIn( 
            /* [in] */ long sizeIn,
            /* [size_is][in] */ long arrayIn[  ]) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE FyArrayOut( 
            /* [in][out] */ long *psizeInOut,
            /* [size_is][out] */ long arrayOut[  ]) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IYVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IY * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IY * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IY * This);
        
        HRESULT ( STDMETHODCALLTYPE *FyCount )( 
            IY * This,
            /* [out] */ long *sizeArray);
        
        HRESULT ( STDMETHODCALLTYPE *FyArrayIn )( 
            IY * This,
            /* [in] */ long sizeIn,
            /* [size_is][in] */ long arrayIn[  ]);
        
        HRESULT ( STDMETHODCALLTYPE *FyArrayOut )( 
            IY * This,
            /* [in][out] */ long *psizeInOut,
            /* [size_is][out] */ long arrayOut[  ]);
        
        END_INTERFACE
    } IYVtbl;

    interface IY
    {
        CONST_VTBL struct IYVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IY_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IY_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IY_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IY_FyCount(This,sizeArray)	\
    ( (This)->lpVtbl -> FyCount(This,sizeArray) ) 

#define IY_FyArrayIn(This,sizeIn,arrayIn)	\
    ( (This)->lpVtbl -> FyArrayIn(This,sizeIn,arrayIn) ) 

#define IY_FyArrayOut(This,psizeInOut,arrayOut)	\
    ( (This)->lpVtbl -> FyArrayOut(This,psizeInOut,arrayOut) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IY_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_Server_0000_0002 */
/* [local] */ 

typedef /* [public][public][public] */ struct __MIDL___MIDL_itf_Server_0000_0002_0001
    {
    double x;
    double y;
    double z;
    } 	Point3d;



extern RPC_IF_HANDLE __MIDL_itf_Server_0000_0002_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_Server_0000_0002_v0_0_s_ifspec;

#ifndef __IZ_INTERFACE_DEFINED__
#define __IZ_INTERFACE_DEFINED__

/* interface IZ */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IZ;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("80905F1B-C703-4BDC-A1BA-795902C1F7AB")
    IZ : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE FzStructIn( 
            /* [in] */ Point3d pt) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE FzStructOut( 
            /* [out] */ Point3d *pt) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IZVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IZ * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IZ * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IZ * This);
        
        HRESULT ( STDMETHODCALLTYPE *FzStructIn )( 
            IZ * This,
            /* [in] */ Point3d pt);
        
        HRESULT ( STDMETHODCALLTYPE *FzStructOut )( 
            IZ * This,
            /* [out] */ Point3d *pt);
        
        END_INTERFACE
    } IZVtbl;

    interface IZ
    {
        CONST_VTBL struct IZVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IZ_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IZ_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IZ_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IZ_FzStructIn(This,pt)	\
    ( (This)->lpVtbl -> FzStructIn(This,pt) ) 

#define IZ_FzStructOut(This,pt)	\
    ( (This)->lpVtbl -> FzStructOut(This,pt) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IZ_INTERFACE_DEFINED__ */



#ifndef __ServerLib_LIBRARY_DEFINED__
#define __ServerLib_LIBRARY_DEFINED__

/* library ServerLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_ServerLib;

EXTERN_C const CLSID CLSID_Component1;

#ifdef __cplusplus

class DECLSPEC_UUID("37CF8074-8BC1-45EC-BBF0-73AD30EB99FB")
Component1;
#endif

EXTERN_C const CLSID CLSID_Component2;

#ifdef __cplusplus

class DECLSPEC_UUID("4E76BCAA-0AFA-4AC0-B7E8-007770E8F8E8")
Component2;
#endif

EXTERN_C const CLSID CLSID_Component3;

#ifdef __cplusplus

class DECLSPEC_UUID("46549E71-CE86-4680-A0DF-3A9DDE503C93")
Component3;
#endif
#endif /* __ServerLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


