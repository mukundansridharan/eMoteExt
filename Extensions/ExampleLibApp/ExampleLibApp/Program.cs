using System;
using Microsoft.SPOT;
using ExampleLib;

namespace ExampleLibApp
{
    public class Program
    {
        public static void Main()
        {
            int arg1 = 5;
            int arg2 = 8;

            Debug.Print(Resources.GetString(Resources.StringResources.String1));
            Exe testExe = new Exe();
            Debug.Print("One plus two = " + testExe.OnePlusTwo());
            Debug.Print("Two plus four = " + testExe.TwoPlusFour());
            Debug.Print("Addition with args: " + arg1 + " plus " + arg2 + " = " + testExe.Arg1PlusArg2(arg1, arg2));
        }
    }
}
