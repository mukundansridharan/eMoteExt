using System;
using Microsoft.SPOT;
using InferenceEngine;

namespace SMOTest
{
    public class Program
    {
        public static void Main()
        {
            SMO emitest = new SMO();
            emitest.runEMITest();
        }
    }
}
