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
            Debug.Print("Connect Logic Analyzer. Sleeping for 5 seconds");
            Thread.Sleep(5000);
            // EMI test
            Debug.Print("Running lower tier test...");
            EMI emitest = new EMI();
            emitest.runEMITest();
            Thread.Sleep(500);
            // FastGRNN test
            Debug.Print("Running upper tier test...");
            FastGRNN fgrnntest = new FastGRNN();
            fgrnntest.runFastGRNNTest();
        }
    }
}
