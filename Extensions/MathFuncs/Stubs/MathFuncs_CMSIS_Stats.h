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


#ifndef _MATHFUNCS_CMSIS_STATS_H_
#define _MATHFUNCS_CMSIS_STATS_H_

namespace CMSIS
{
    struct Stats
    {
        // Helper Functions to access fields of managed object
        // Declaration of stubs. These functions are implemented by Interop code developers
        static INT32 Max_Nat( CLR_RT_TypedArray_INT32 param0, UINT32 * param1, HRESULT &hr );
        static INT32 Min_Nat( CLR_RT_TypedArray_INT32 param0, UINT32 * param1, HRESULT &hr );
        static INT32 Mean_Nat( CLR_RT_TypedArray_INT32 param0, HRESULT &hr );
        static INT32 StD_Nat( CLR_RT_TypedArray_INT32 param0, HRESULT &hr );
        static INT32 Var_Nat( CLR_RT_TypedArray_INT32 param0, HRESULT &hr );
    };
}
#endif  //_MATHFUNCS_CMSIS_STATS_H_
