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


#ifndef _EXAMPLELIB_EXAMPLELIB_EXE_H_
#define _EXAMPLELIB_EXAMPLELIB_EXE_H_

namespace ExampleLib
{
    struct Exe
    {
        // Helper Functions to access fields of managed object
        // Declaration of stubs. These functions are implemented by Interop code developers
        static INT32 OnePlusTwo( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
        static INT32 TwoPlusFour( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
        static INT32 Arg1PlusArg2( CLR_RT_HeapBlock* pMngObj, INT32 param0, INT32 param1, HRESULT &hr );
        static INT32 Arg1PlusArg2_IntFunc( CLR_RT_HeapBlock* pMngObj, INT32 param0, INT32 param1, HRESULT &hr );
	   static INT32 IntFunc( INT32 param0, INT32 param1 );
    };
}
#endif  //_EXAMPLELIB_EXAMPLELIB_EXE_H_
