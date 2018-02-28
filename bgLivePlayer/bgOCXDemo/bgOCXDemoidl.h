

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Wed Feb 28 15:19:25 2018
 */
/* Compiler settings for .\bgOCXDemo.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: stub_data 
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


#ifndef __bgOCXDemoidl_h__
#define __bgOCXDemoidl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___DbgOCXDemo_FWD_DEFINED__
#define ___DbgOCXDemo_FWD_DEFINED__
typedef interface _DbgOCXDemo _DbgOCXDemo;
#endif 	/* ___DbgOCXDemo_FWD_DEFINED__ */


#ifndef ___DbgOCXDemoEvents_FWD_DEFINED__
#define ___DbgOCXDemoEvents_FWD_DEFINED__
typedef interface _DbgOCXDemoEvents _DbgOCXDemoEvents;
#endif 	/* ___DbgOCXDemoEvents_FWD_DEFINED__ */


#ifndef __bgOCXDemo_FWD_DEFINED__
#define __bgOCXDemo_FWD_DEFINED__

#ifdef __cplusplus
typedef class bgOCXDemo bgOCXDemo;
#else
typedef struct bgOCXDemo bgOCXDemo;
#endif /* __cplusplus */

#endif 	/* __bgOCXDemo_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __bgOCXDemoLib_LIBRARY_DEFINED__
#define __bgOCXDemoLib_LIBRARY_DEFINED__

/* library bgOCXDemoLib */
/* [control][helpstring][helpfile][version][uuid] */ 


EXTERN_C const IID LIBID_bgOCXDemoLib;

#ifndef ___DbgOCXDemo_DISPINTERFACE_DEFINED__
#define ___DbgOCXDemo_DISPINTERFACE_DEFINED__

/* dispinterface _DbgOCXDemo */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__DbgOCXDemo;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("2A4A9DFA-DEE6-438B-B2C6-D2D9574391C2")
    _DbgOCXDemo : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DbgOCXDemoVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DbgOCXDemo * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DbgOCXDemo * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DbgOCXDemo * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DbgOCXDemo * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DbgOCXDemo * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DbgOCXDemo * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DbgOCXDemo * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DbgOCXDemoVtbl;

    interface _DbgOCXDemo
    {
        CONST_VTBL struct _DbgOCXDemoVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DbgOCXDemo_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DbgOCXDemo_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DbgOCXDemo_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DbgOCXDemo_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DbgOCXDemo_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DbgOCXDemo_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DbgOCXDemo_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DbgOCXDemo_DISPINTERFACE_DEFINED__ */


#ifndef ___DbgOCXDemoEvents_DISPINTERFACE_DEFINED__
#define ___DbgOCXDemoEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DbgOCXDemoEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__DbgOCXDemoEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("CEE45F56-68A6-40CB-8605-B3FB43923933")
    _DbgOCXDemoEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DbgOCXDemoEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DbgOCXDemoEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DbgOCXDemoEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DbgOCXDemoEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DbgOCXDemoEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DbgOCXDemoEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DbgOCXDemoEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DbgOCXDemoEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DbgOCXDemoEventsVtbl;

    interface _DbgOCXDemoEvents
    {
        CONST_VTBL struct _DbgOCXDemoEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DbgOCXDemoEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DbgOCXDemoEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DbgOCXDemoEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DbgOCXDemoEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DbgOCXDemoEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DbgOCXDemoEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DbgOCXDemoEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DbgOCXDemoEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_bgOCXDemo;

#ifdef __cplusplus

class DECLSPEC_UUID("0074D9F2-F690-4F1F-BED6-D44288374196")
bgOCXDemo;
#endif
#endif /* __bgOCXDemoLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


