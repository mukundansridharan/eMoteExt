using System;
using System.Threading;
using Microsoft.SPOT;
using InferenceEngine;

namespace SMOTest
{
    public class Program
    {
        public static void Main()
        {
            //Debug.Print("Connect Logic Analyzer. Sleeping for 5 seconds");
            Thread.Sleep(5000);
            SMO emitest = new SMO();
            emitest.runEMITest();
            //ushort[] test_arr = new ushort[512];
            //emitest.EMIDriver(test_arr);
        }
    }
}
