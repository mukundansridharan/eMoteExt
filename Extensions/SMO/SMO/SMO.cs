using System;
using System.Runtime.CompilerServices;
using Microsoft.SPOT;

namespace InferenceEngine
{
    public class SMO
    {
        //[MethodImplAttribute(MethodImplOptions.InternalCall)]
        //extern public float predictCount(float[] features);

        //[MethodImplAttribute(MethodImplOptions.InternalCall)]
        //extern public double predictClass(float[] features);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public void runEMITest();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public void EMIDriver(uint[] bagData);
    }
}
