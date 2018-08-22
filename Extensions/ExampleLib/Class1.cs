using System;
using System.Runtime.CompilerServices;
using Microsoft.SPOT;

namespace ExampleLib
{
    public class Exe
    {
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
    }
}
