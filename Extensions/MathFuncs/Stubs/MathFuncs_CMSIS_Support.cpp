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
#include "arm_math.h"
#include "utils.h"

using namespace CMSIS;

INT32 Support::ConvertFloatToInt( float param0, HRESULT &hr )
{
    q31_t retVal;
    arm_float_to_q31(&param0, &retVal, 1);
    return retVal;
}

float Support::ConvertIntToFloat( INT32 param0, HRESULT &hr )
{
    float retVal;
    arm_q31_to_float((q31_t*) &param0, &retVal, 1);
    return retVal;
}

