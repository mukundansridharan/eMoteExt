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
#include "SMO_InferenceEngine_EMI.h"
#include "emi_fastgrnn.cpp"

using namespace InferenceEngine;

void EMI::runEMITest( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
	run_test_lower();
}

INT8 EMI::EMIDriver( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT32 param0, HRESULT &hr )
{
    INT8 retVal = emi_driver(param0.GetBuffer()); 
    return retVal;
}

