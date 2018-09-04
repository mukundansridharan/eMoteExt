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
#include "arm_mat_init_q15.c"
#include "arm_mat_add_q15.c"
#include "arm_mat_sub_q15.c"
#include "arm_mat_mult_q15.c"
#include "arm_mat_trans_q15.c"

using namespace CMSIS;

void MatrixFuncs::MatrixMult_Nat( UINT16 param0, UINT16 param1, CLR_RT_TypedArray_INT32 param2, UINT16 param3, UINT16 param4, CLR_RT_TypedArray_INT32 param5, UINT16 * param6, UINT16 * param7, CLR_RT_TypedArray_INT32 param8, HRESULT &hr )
{
	arm_matrix_instance_q15 m1, m2, mout;

	/*Initialize matrices*/
	arm_mat_init_q15(&m1, param0, param1, (q15_t *) param2.GetBuffer());
	arm_mat_init_q15(&m2, param3, param4, (q15_t *) param5.GetBuffer());

	/*Initialize output matrix*/
	arm_mat_init_q15(&mout, param0, param4, (q15_t *) param8.GetBuffer());

	/*Matrix operation*/
	arm_mat_mult_q15(&m1, &m2, &mout, NULL);
	
	/*Populate output*/
	* param6 = mout.numRows;
	* param7 = mout.numCols;
	memcpy(param8.GetBuffer(), mout.pData, sizeof(INT16)*mout.numRows*mout.numCols);
}

void MatrixFuncs::MatrixAdd_Nat( UINT16 param0, UINT16 param1, CLR_RT_TypedArray_INT32 param2, UINT16 param3, UINT16 param4, CLR_RT_TypedArray_INT32 param5, UINT16 * param6, UINT16 * param7, CLR_RT_TypedArray_INT32 param8, HRESULT &hr )
{
	arm_matrix_instance_q15 m1, m2, mout;

	/*Initialize matrices*/
	arm_mat_init_q15(&m1, param0, param1, (q15_t *) param2.GetBuffer());
	arm_mat_init_q15(&m2, param3, param4, (q15_t *) param5.GetBuffer());

	/*Initialize output matrix*/
	arm_mat_init_q15(&mout, param0, param1, (q15_t *) param8.GetBuffer());

	/*Matrix operation*/
	arm_mat_add_q15(&m1, &m2, &mout);
	
	/*Populate output*/
	* param6 = mout.numRows;
	* param7 = mout.numCols;
	memcpy(param8.GetBuffer(), mout.pData, sizeof(INT16)*mout.numRows*mout.numCols);
}

void MatrixFuncs::MatrixSub_Nat( UINT16 param0, UINT16 param1, CLR_RT_TypedArray_INT32 param2, UINT16 param3, UINT16 param4, CLR_RT_TypedArray_INT32 param5, UINT16 * param6, UINT16 * param7, CLR_RT_TypedArray_INT32 param8, HRESULT &hr )
{
	arm_matrix_instance_q15 m1, m2, mout;

	/*Initialize matrices*/
	arm_mat_init_q15(&m1, param0, param1, (q15_t *) param2.GetBuffer());
	arm_mat_init_q15(&m2, param3, param4, (q15_t *) param5.GetBuffer());

	/*Initialize output matrix*/
	arm_mat_init_q15(&mout, param0, param1, (q15_t *) param8.GetBuffer());

	/*Matrix operation*/
	arm_mat_sub_q15(&m1, &m2, &mout);
	
	/*Populate output*/
	* param6 = mout.numRows;
	* param7 = mout.numCols;
	memcpy(param8.GetBuffer(), mout.pData, sizeof(INT16)*mout.numRows*mout.numCols);
}

void MatrixFuncs::MatrixTrans_Nat( UINT16 param0, UINT16 param1, CLR_RT_TypedArray_INT32 param2, UINT16 * param3, UINT16 * param4, CLR_RT_TypedArray_INT32 param5, HRESULT &hr )
{
	arm_matrix_instance_q15 m1, mout;

	/*Initialize input matrix*/
	arm_mat_init_q15(&m1, param0, param1, (q15_t *) param2.GetBuffer());

	/*Initialize output matrix*/
	arm_mat_init_q15(&mout, param1, param0, (q15_t *) param5.GetBuffer());

	/*Matrix operation*/
	arm_mat_trans_q15(&m1, &mout);

	/*Populate output*/
	* param3 = mout.numRows;
	* param4 = mout.numCols;
	memcpy(param5.GetBuffer(), mout.pData, sizeof(INT16)*mout.numRows*mout.numCols);
}

