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
#include "MathFuncs_CMSIS_VectorFuncs.h"
#include "arm_abs_q31.c"
#include "arm_negate_q31.c"
#include "arm_offset_q31.c"
#include "arm_add_q31.c"
#include "arm_sub_q31.c"
//#include "arm_dot_prod_q31.c" -- doesn't work
#include "arm_mult_q31.c"

using namespace CMSIS;

void VectorFuncs::VectorAbs_Nat( CLR_RT_TypedArray_INT32 param0, CLR_RT_TypedArray_INT32 param1, HRESULT &hr )
{
	arm_abs_q31((q31_t*) param0.GetBuffer(), (q31_t*) param1.GetBuffer(), param1.GetSize());
}

void VectorFuncs::VectorNegate_Nat( CLR_RT_TypedArray_INT32 param0, CLR_RT_TypedArray_INT32 param1, HRESULT &hr )
{
	arm_negate_q31((q31_t*) param0.GetBuffer(), (q31_t*) param1.GetBuffer(), param1.GetSize());
}

void VectorFuncs::VectorOffset_Nat( CLR_RT_TypedArray_INT32 param0, INT32 param1, CLR_RT_TypedArray_INT32 param2, HRESULT &hr )
{
	arm_offset_q31((q31_t*) param0.GetBuffer(), param1, (q31_t*) param2.GetBuffer(), param2.GetSize());
}

void VectorFuncs::VectorAdd_Nat( CLR_RT_TypedArray_INT32 param0, CLR_RT_TypedArray_INT32 param1, CLR_RT_TypedArray_INT32 param2, HRESULT &hr )
{
	arm_add_q31((q31_t*) param0.GetBuffer(), (q31_t*) param1.GetBuffer(), (q31_t*) param2.GetBuffer(), param2.GetSize());
}

void VectorFuncs::VectorSub_Nat( CLR_RT_TypedArray_INT32 param0, CLR_RT_TypedArray_INT32 param1, CLR_RT_TypedArray_INT32 param2, HRESULT &hr )
{
	arm_sub_q31((q31_t*) param0.GetBuffer(), (q31_t*) param1.GetBuffer(), (q31_t*) param2.GetBuffer(), param2.GetSize());
}

// Doesn't work (Q16.48)
/*INT32 VectorFuncs::VectorDot_Nat( CLR_RT_TypedArray_INT32 param0, CLR_RT_TypedArray_INT32 param1, HRESULT &hr )
{
    q63_t retVal = 0;
    arm_dot_prod_q31((q31_t*) param0.GetBuffer(), (q31_t*) param1.GetBuffer(), param1.GetSize(), &retVal);
    return clip_q63_to_q31(retVal);
}*/

INT32 VectorFuncs::VectorDot_Nat( CLR_RT_TypedArray_INT32 param0, CLR_RT_TypedArray_INT32 param1, HRESULT &hr )
{
	q31_t multOutput[param1.GetSize()];
	arm_mult_q31((q31_t*) param0.GetBuffer(), (q31_t*) param1.GetBuffer(), multOutput, param1.GetSize());
	
	q31_t retVal=0;
	for(INT32 i=0; i< param1.GetSize(); i++)
	{
		arm_add_q31(&retVal, &multOutput[i], &retVal, 1);
	}
	return retVal;
}

void VectorFuncs::VectorHadamard_Nat( CLR_RT_TypedArray_INT32 param0, CLR_RT_TypedArray_INT32 param1, CLR_RT_TypedArray_INT32 param2, HRESULT &hr )
{
	arm_mult_q31((q31_t*) param0.GetBuffer(), (q31_t*) param1.GetBuffer(), (q31_t*) param2.GetBuffer(), param2.GetSize());
}

