using System;
using Microsoft.SPOT;
using InferenceEngine;

namespace SMOTest
{
    public class Program
    {
        public static void Main()
        {
            float[] f_classifier = new float[13] { 72f, 1.055556f, 0.916667f, 0.75f, 16f, 759f, 127f, 0.111111f, 39351f, 0.027778f, 103f, 0.333333f, 100f };
            float[] f_counter = new float[13] { 126f, 4.555556f, 136692f, 114f, 801.361862f, 73793f, 32230f, 127f, 0.166667f, 12467f, 0.055556f, 101f, 100f };

            SMO smotest = new SMO();

            long start1 = System.DateTime.Now.Ticks;
            double out_classifier = smotest.predictClass(f_classifier);
            long end1 = System.DateTime.Now.Ticks;

            long start2 = System.DateTime.Now.Ticks;
            double out_counter = smotest.predictCount(f_counter);
            long end2 = System.DateTime.Now.Ticks;


            Debug.Print("Classifier output: " + out_classifier + "; time (ms): " + ((end1-start1)/System.TimeSpan.TicksPerMillisecond));
            Debug.Print("Counter output: " + out_counter + "; time (ms): " + ((end2 - start2) / System.TimeSpan.TicksPerMillisecond));
        }
    }
}
