using System;
using System.Runtime.CompilerServices;
using Microsoft.SPOT;

namespace CMSIS
{
    public class Support
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static int ConvertFloatToInt(float x);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static float ConvertIntToFloat(int x);
    }
}
