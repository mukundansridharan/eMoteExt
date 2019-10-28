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
#include "SMONative.cpp"
#include "emi_fastgrnn.cpp"

using namespace InferenceEngine;

float SMO::predictCount( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_float param0, HRESULT &hr )
{
    return SMONative::predictCount(param0.GetBuffer());
}

double SMO::predictClass( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_float param0, HRESULT &hr )
{
    return SMONative::predictClass(param0.GetBuffer());
}

