//-----------------------------------------------------------------------------
//
//                   ** WARNING! ** 
//    This file was generated automatically by a tool.
//    Re-running the tool will overwrite this file.
//    You should copy this file to a custom location
//    before adding any customization in the copy to
//    prevent loss of your changes when the tool is
//    re-run.
//
//-----------------------------------------------------------------------------


#ifndef _MATHFUNCS_CMSIS_VECTORFUNCS_H_
#define _MATHFUNCS_CMSIS_VECTORFUNCS_H_

namespace CMSIS
{
    struct VectorFuncs
    {
        // Helper Functions to access fields of managed object
        // Declaration of stubs. These functions are implemented by Interop code developers
        static void VectorAbs_Nat( CLR_RT_TypedArray_INT32 param0, CLR_RT_TypedArray_INT32 param1, HRESULT &hr );
        static void VectorNegate_Nat( CLR_RT_TypedArray_INT32 param0, CLR_RT_TypedArray_INT32 param1, HRESULT &hr );
        static void VectorOffset_Nat( CLR_RT_TypedArray_INT32 param0, INT32 param1, CLR_RT_TypedArray_INT32 param2, HRESULT &hr );
        static void VectorAdd_Nat( CLR_RT_TypedArray_INT32 param0, CLR_RT_TypedArray_INT32 param1, CLR_RT_TypedArray_INT32 param2, HRESULT &hr );
        static void VectorSub_Nat( CLR_RT_TypedArray_INT32 param0, CLR_RT_TypedArray_INT32 param1, CLR_RT_TypedArray_INT32 param2, HRESULT &hr );
        static INT32 VectorDot_Nat( CLR_RT_TypedArray_INT32 param0, CLR_RT_TypedArray_INT32 param1, HRESULT &hr );
        static void VectorHadamard_Nat( CLR_RT_TypedArray_INT32 param0, CLR_RT_TypedArray_INT32 param1, CLR_RT_TypedArray_INT32 param2, HRESULT &hr );
        static void VectorRecip_Nat( CLR_RT_TypedArray_INT32 param0, CLR_RT_TypedArray_INT32 param1, HRESULT &hr );
        static void VectorScale_Nat( CLR_RT_TypedArray_INT32 param0, INT32 param1, CLR_RT_TypedArray_INT32 param2, HRESULT &hr );
    };
}
#endif  //_MATHFUNCS_CMSIS_VECTORFUNCS_H_
