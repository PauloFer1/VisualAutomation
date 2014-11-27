

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Thu Nov 20 19:50:32 2014
 */
/* Compiler settings for VisualAutomation.idl:
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


#ifndef __VisualAutomation_h_h__
#define __VisualAutomation_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IVisualAutomation_FWD_DEFINED__
#define __IVisualAutomation_FWD_DEFINED__
typedef interface IVisualAutomation IVisualAutomation;

#endif 	/* __IVisualAutomation_FWD_DEFINED__ */


#ifndef __VisualAutomation_FWD_DEFINED__
#define __VisualAutomation_FWD_DEFINED__

#ifdef __cplusplus
typedef class VisualAutomation VisualAutomation;
#else
typedef struct VisualAutomation VisualAutomation;
#endif /* __cplusplus */

#endif 	/* __VisualAutomation_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __VisualAutomation_LIBRARY_DEFINED__
#define __VisualAutomation_LIBRARY_DEFINED__

/* library VisualAutomation */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_VisualAutomation;

#ifndef __IVisualAutomation_DISPINTERFACE_DEFINED__
#define __IVisualAutomation_DISPINTERFACE_DEFINED__

/* dispinterface IVisualAutomation */
/* [uuid] */ 


EXTERN_C const IID DIID_IVisualAutomation;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("00706EDC-9BBD-4FB7-BFAB-5AF49DE21980")
    IVisualAutomation : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IVisualAutomationVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IVisualAutomation * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IVisualAutomation * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IVisualAutomation * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IVisualAutomation * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IVisualAutomation * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IVisualAutomation * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IVisualAutomation * This,
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
        
        END_INTERFACE
    } IVisualAutomationVtbl;

    interface IVisualAutomation
    {
        CONST_VTBL struct IVisualAutomationVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IVisualAutomation_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IVisualAutomation_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IVisualAutomation_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IVisualAutomation_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IVisualAutomation_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IVisualAutomation_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IVisualAutomation_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IVisualAutomation_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_VisualAutomation;

#ifdef __cplusplus

class DECLSPEC_UUID("1B6079FA-92EF-4377-9BDA-7888529679DD")
VisualAutomation;
#endif
#endif /* __VisualAutomation_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


