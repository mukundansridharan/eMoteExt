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
#include "MathFuncs_CMSIS_Support.h"
#include "utils.h"

using namespace CMSIS;

INT32 Support::ConvertFloatToQ31( float param0, HRESULT &hr )
{
    q31_t retVal;
    arm_float_to_q31(&param0, &retVal, 1);
    return retVal;
}

float Support::ConvertQ31ToFloat( INT32 param0, HRESULT &hr )
{
    float retVal;
    arm_q31_to_float((q31_t*) &param0, &retVal, 1);
    return retVal;
}

void Support::ConvertFloatToQ31( CLR_RT_TypedArray_float param0, CLR_RT_TypedArray_INT32 param1, HRESULT &hr )
{
	arm_float_to_q31(param0.GetBuffer(), (q31_t*) param1.GetBuffer(), param0.GetSize());
}

void Support::ConvertQ31ToFloat( CLR_RT_TypedArray_INT32 param0, CLR_RT_TypedArray_float param1, HRESULT &hr )
{
	arm_q31_to_float((q31_t*) param0.GetBuffer(), param1.GetBuffer(), param0.GetSize());
}

