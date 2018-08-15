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
    }
}
