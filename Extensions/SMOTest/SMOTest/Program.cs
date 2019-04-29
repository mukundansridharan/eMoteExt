using System;
using Microsoft.SPOT;
using InferenceEngine;

namespace SMOTest
{
    public class Program
    {
        public static void Main()
        {
            float[] f_classifier = new float[13] { 58f, 2f, 1.833333f, 1.5f, 7f, 48f, 47f, 0.083333f, 1599f, 0.083333f, -1f, 0f, -1f };
            float[] f_counter = new float[13] { 107f, 9.666667f, 22621f, 29f, 25.089744f, 6701f, 387f, 47f, 0.083333f, 93f, 0.083333f, -1f, -1f };

            SMO smotest = new SMO();
            
            double out_classifier = smotest.predictClass(f_classifier);

            double out_counter = smotest.predictCount(f_counter);

            Debug.Print("Classifier output: " + out_classifier);
            Debug.Print("Counter output: " + out_counter);
        }
    }
}
