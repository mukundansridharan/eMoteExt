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


#include "MathFuncs.h"
#include "MathFuncs_CMSIS_Stats.h"
#include "arm_max_q31.c"
#include "arm_min_q31.c"
#include "arm_mean_q31.c"
#include "arm_std_q31.c"
#include "arm_var_q31.c"

using namespace CMSIS;

INT32 Stats::Max_Nat( CLR_RT_TypedArray_INT32 param0, UINT32 * param1, HRESULT &hr )
{
    q31_t retVal = 0;
    arm_max_q31((q31_t*) param0.GetBuffer(), param0.GetSize(), &retVal, (uint32_t*) param1);
    return retVal;
}

INT32 Stats::Min_Nat( CLR_RT_TypedArray_INT32 param0, UINT32 * param1, HRESULT &hr )
{
    q31_t retVal = 0;
    arm_min_q31((q31_t*) param0.GetBuffer(), param0.GetSize(), &retVal, (uint32_t*) param1);
    return retVal;
}

INT32 Stats::Mean_Nat( CLR_RT_TypedArray_INT32 param0, HRESULT &hr )
{
    q31_t retVal = 0;
    arm_mean_q31((q31_t*) param0.GetBuffer(), param0.GetSize(), &retVal);
    return retVal;
}

INT32 Stats::StD_Nat( CLR_RT_TypedArray_INT32 param0, HRESULT &hr )
{
    q31_t retVal = 0;
    arm_std_q31((q31_t*) param0.GetBuffer(), param0.GetSize(), &retVal);
    return retVal;
}

INT32 Stats::Var_Nat( CLR_RT_TypedArray_INT32 param0, HRESULT &hr )
{
    q31_t retVal = 0;
    arm_var_q31((q31_t*) param0.GetBuffer(), param0.GetSize(), &retVal);
    return retVal;
}

