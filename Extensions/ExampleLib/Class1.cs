using System;
using System.Runtime.CompilerServices;
using Microsoft.SPOT;

namespace ExampleLib
{
    public class Exe
    {
        public struct Arg
        {
            public UInt16 size;
            public UInt16[] array;
        };  

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public int OnePlusTwo();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public int TwoPlusFour();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public int Arg1PlusArg2(int arg1, int arg2);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public int Arg1PlusArg2_IntFunc(int arg1, int arg2);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public int IntExp_Lib(int arg1, int arg2);


        public void CustomArgsIO(Arg input, out UInt16 outsize, UInt16[] outarr)
        {
            CustomArgsIO_Int(input.size, input.array, out outsize, outarr);
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern int CustomArgsIO_Int(UInt16 input_size, UInt16[] input_array, out UInt16 output_size, UInt16[] output_array);
    }
}
