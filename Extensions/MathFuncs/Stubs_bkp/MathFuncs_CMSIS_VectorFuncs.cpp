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
#include "arm_abs_q15.c"
#include "arm_negate_q15.c"
#include "arm_offset_q15.c"
#include "arm_add_q15.c"
#include "arm_sub_q15.c"
//#include "arm_dot_prod_q15.c" -- doesn't work
#include "arm_mult_q15.c"
#include "arm_scale_q15.c"
#include "arm_common_tables.c"

using namespace CMSIS;

void VectorFuncs::VectorAbs_Nat( CLR_RT_TypedArray_INT32 param0, CLR_RT_TypedArray_INT32 param1, HRESULT &hr )
{
	arm_abs_q15((q15_t*) param0.GetBuffer(), (q15_t*) param1.GetBuffer(), param1.GetSize());
}

void VectorFuncs::VectorNegate_Nat( CLR_RT_TypedArray_INT32 param0, CLR_RT_TypedArray_INT32 param1, HRESULT &hr )
{
	arm_negate_q15((q15_t*) param0.GetBuffer(), (q15_t*) param1.GetBuffer(), param1.GetSize());
}

void VectorFuncs::VectorOffset_Nat( CLR_RT_TypedArray_INT32 param0, INT32 param1, CLR_RT_TypedArray_INT32 param2, HRESULT &hr )
{
	arm_offset_q15((q15_t*) param0.GetBuffer(), param1, (q15_t*) param2.GetBuffer(), param2.GetSize());
}

void VectorFuncs::VectorAdd_Nat( CLR_RT_TypedArray_INT32 param0, CLR_RT_TypedArray_INT32 param1, CLR_RT_TypedArray_INT32 param2, HRESULT &hr )
{
	arm_add_q15((q15_t*) param0.GetBuffer(), (q15_t*) param1.GetBuffer(), (q15_t*) param2.GetBuffer(), param2.GetSize());
}

void VectorFuncs::VectorSub_Nat( CLR_RT_TypedArray_INT32 param0, CLR_RT_TypedArray_INT32 param1, CLR_RT_TypedArray_INT32 param2, HRESULT &hr )
{
	arm_sub_q15((q15_t*) param0.GetBuffer(), (q15_t*) param1.GetBuffer(), (q15_t*) param2.GetBuffer(), param2.GetSize());
}

// Doesn't work (Q16.48)
/*INT32 VectorFuncs::VectorDot_Nat( CLR_RT_TypedArray_INT32 param0, CLR_RT_TypedArray_INT32 param1, HRESULT &hr )
{
    q63_t retVal = 0;
    arm_dot_prod_q15((q15_t*) param0.GetBuffer(), (q15_t*) param1.GetBuffer(), param1.GetSize(), &retVal);
    return clip_q63_to_q15(retVal);
}*/

INT32 VectorFuncs::VectorDot_Nat( CLR_RT_TypedArray_INT32 param0, CLR_RT_TypedArray_INT32 param1, HRESULT &hr )
{
	q15_t multOutput[param1.GetSize()];
	arm_mult_q15((q15_t*) param0.GetBuffer(), (q15_t*) param1.GetBuffer(), multOutput, param1.GetSize());
	
	q15_t retVal=0;
	for(INT32 i=0; i< param1.GetSize(); i++)
	{
		arm_add_q15(&retVal, &multOutput[i], &retVal, 1);
	}
	return retVal;
}

void VectorFuncs::VectorHadamard_Nat( CLR_RT_TypedArray_INT32 param0, CLR_RT_TypedArray_INT32 param1, CLR_RT_TypedArray_INT32 param2, HRESULT &hr )
{
	arm_mult_q15((q15_t*) param0.GetBuffer(), (q15_t*) param1.GetBuffer(), (q15_t*) param2.GetBuffer(), param2.GetSize());
}


// Doesn't work: don't know how to interpret result (https://voltampmedia.com/2011/09/27/using-arms-cmsis-dsp-library-arm_recip_q15/)
/*void VectorFuncs::VectorRecip_Nat( CLR_RT_TypedArray_INT32 param0, CLR_RT_TypedArray_INT32 param1, HRESULT &hr )
{
	q15_t* inbuf = (q15_t*) param0.GetBuffer();
	q15_t* outbuf = (q15_t*) param1.GetBuffer();
	for(INT32 i=0; i < param1.GetSize(); i++)
	{
		uint32_t sign=arm_recip_q15(inbuf[i], &outbuf[i], (q15_t*) armRecipTableQ15);
		hal_printf("Reciprocal: sign: %u, out: %d", sign, outbuf[i]);
	}
}*/

void VectorFuncs::VectorRecip_Nat( CLR_RT_TypedArray_INT32 param0, CLR_RT_TypedArray_INT32 param1, HRESULT &hr )
{
}

void VectorFuncs::VectorScale_Nat( CLR_RT_TypedArray_INT32 param0, INT32 param1, CLR_RT_TypedArray_INT32 param2, HRESULT &hr )
{
	//int8_t shift set to 0, since scale factor is scale = scaleFract * 2^shift
	arm_scale_q15((q15_t*) param0.GetBuffer(), param1, 0, (q15_t*) param2.GetBuffer(), param0.GetSize());
}

