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


#ifndef _MATHFUNCS_CMSIS_SUPPORT_H_
#define _MATHFUNCS_CMSIS_SUPPORT_H_

namespace CMSIS
{
    struct Support
    {
        // Helper Functions to access fields of managed object
        // Declaration of stubs. These functions are implemented by Interop code developers
        static INT32 ConvertFloatToQ15( float param0, HRESULT &hr );
        static float ConvertQ15ToFloat( INT32 param0, HRESULT &hr );
        static void ConvertFloatToQ15( CLR_RT_TypedArray_float param0, CLR_RT_TypedArray_INT32 param1, HRESULT &hr );
        static void ConvertQ15ToFloat( CLR_RT_TypedArray_INT32 param0, CLR_RT_TypedArray_float param1, HRESULT &hr );
        static void VectorCopy_Nat( CLR_RT_TypedArray_INT32 param0, INT32 param1, CLR_RT_TypedArray_INT32 param2, UINT32 param3, HRESULT &hr );
        static void VectorFill_Nat( INT32 param0, CLR_RT_TypedArray_INT32 param1, HRESULT &hr );
    };
}
#endif  //_MATHFUNCS_CMSIS_SUPPORT_H_
