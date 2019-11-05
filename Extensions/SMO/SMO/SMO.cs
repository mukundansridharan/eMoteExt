using System;
using System.Runtime.CompilerServices;
using Microsoft.SPOT;

namespace InferenceEngine
{
    public class EMI
    {
        //[MethodImplAttribute(MethodImplOptions.InternalCall)]
        //extern public float predictCount(float[] features);

        //[MethodImplAttribute(MethodImplOptions.InternalCall)]
        //extern public double predictClass(float[] features);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public void runEMITest();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public bool EMIDriver(uint[] bagData);
    }

    public class FastGRNN
    {
        //[MethodImplAttribute(MethodImplOptions.InternalCall)]
        //extern public float predictCount(float[] features);

        //[MethodImplAttribute(MethodImplOptions.InternalCall)]
        //extern public double predictClass(float[] features);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public void runFastGRNNTest();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public bool FastGRNNDriver(uint[] bagData);
    }
}
