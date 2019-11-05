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


#include "SMO.h"
#include "SMO_InferenceEngine_SMO.h"
#include "emi_fastgrnn.cpp"

using namespace InferenceEngine;

void SMO::runEMITest( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
	run_test();
}

int num_disp = 0;

void SMO::EMIDriver( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT32 param0, HRESULT &hr )
{
	// Call EMI driver
	bool bag_dec = emi_driver(param0.GetBuffer());

	// Print aggregated bag-level decision
	if(bag_dec)
		hal_printf("\nDisplacement %d\n", num_disp++);
	//else
	//	hal_printf("\nN\n");
}

