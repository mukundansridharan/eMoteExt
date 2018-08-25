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


#include <tinyhal.h>
#include "MathFuncs.h"
#include "MathFuncs_CMSIS_MatrixFuncs.h"
#include "arm_math.h"
#include "arm_mat_init_q31.c"
#include "arm_mat_add_q31.c"
#include "arm_mat_sub_q31.c"
#include "arm_mat_mult_fast_q31.c"

using namespace CMSIS;

void MatrixFuncs::MatrixMult_Nat( UINT16 param0, UINT16 param1, CLR_RT_TypedArray_INT32 param2, UINT16 param3, UINT16 param4, CLR_RT_TypedArray_INT32 param5, UINT16 * param6, UINT16 * param7, CLR_RT_TypedArray_INT32 param8, HRESULT &hr )
{
	arm_matrix_instance_q31 m1, m2, mout;

	/*Initialize matrices*/
	arm_mat_init_q31(&m1, param0, param1, (q31_t *) param2.GetBuffer());
	arm_mat_init_q31(&m2, param3, param4, (q31_t *) param5.GetBuffer());

	/*Initialize output matrix*/
	arm_mat_init_q31(&mout, param0, param4, (q31_t *) param8.GetBuffer());

	/*Fast matrix multiplication*/
	arm_mat_mult_fast_q31(&m1, &m2, &mout);
	
	/*Populate output*/
	* param6 = mout.numRows;
	* param7 = mout.numCols;
	memcpy(param8.GetBuffer(), mout.pData, sizeof(INT32)*mout.numRows*mout.numCols);
}

void MatrixFuncs::MatrixAdd_Nat( UINT16 param0, UINT16 param1, CLR_RT_TypedArray_INT32 param2, UINT16 param3, UINT16 param4, CLR_RT_TypedArray_INT32 param5, UINT16 * param6, UINT16 * param7, CLR_RT_TypedArray_INT32 param8, HRESULT &hr )
{
	arm_matrix_instance_q31 m1, m2, mout;

	/*Initialize matrices*/
	arm_mat_init_q31(&m1, param0, param1, (q31_t *) param2.GetBuffer());
	arm_mat_init_q31(&m2, param3, param4, (q31_t *) param5.GetBuffer());

	/*Initialize output matrix*/
	arm_mat_init_q31(&mout, param0, param1, (q31_t *) param8.GetBuffer());

	/*Fast matrix multiplication*/
	arm_mat_add_q31(&m1, &m2, &mout);
	
	/*Populate output*/
	* param6 = mout.numRows;
	* param7 = mout.numCols;
	memcpy(param8.GetBuffer(), mout.pData, sizeof(INT32)*mout.numRows*mout.numCols);
}

void MatrixFuncs::MatrixSub_Nat( UINT16 param0, UINT16 param1, CLR_RT_TypedArray_INT32 param2, UINT16 param3, UINT16 param4, CLR_RT_TypedArray_INT32 param5, UINT16 * param6, UINT16 * param7, CLR_RT_TypedArray_INT32 param8, HRESULT &hr )
{
	arm_matrix_instance_q31 m1, m2, mout;

	/*Initialize matrices*/
	arm_mat_init_q31(&m1, param0, param1, (q31_t *) param2.GetBuffer());
	arm_mat_init_q31(&m2, param3, param4, (q31_t *) param5.GetBuffer());

	/*Initialize output matrix*/
	arm_mat_init_q31(&mout, param0, param1, (q31_t *) param8.GetBuffer());

	/*Fast matrix multiplication*/
	arm_mat_sub_q31(&m1, &m2, &mout);
	
	/*Populate output*/
	* param6 = mout.numRows;
	* param7 = mout.numCols;
	memcpy(param8.GetBuffer(), mout.pData, sizeof(INT32)*mout.numRows*mout.numCols);
}

