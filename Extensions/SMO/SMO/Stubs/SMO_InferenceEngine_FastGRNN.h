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


#ifndef _SMO_INFERENCEENGINE_FASTGRNN_H_
#define _SMO_INFERENCEENGINE_FASTGRNN_H_

namespace InferenceEngine
{
    struct FastGRNN
    {
        // Helper Functions to access fields of managed object
        // Declaration of stubs. These functions are implemented by Interop code developers
        static void runFastGRNNTest( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
        static INT8 FastGRNNDriver( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT32 param0, HRESULT &hr );
    };
}
#endif  //_SMO_INFERENCEENGINE_FASTGRNN_H_
