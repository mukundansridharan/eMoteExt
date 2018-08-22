using System;
using Microsoft.SPOT;
using ExampleLib;

namespace ExampleLibApp
{
    public class Program
    {
        public static void Main()
        {
            Debug.Print(Resources.GetString(Resources.StringResources.String1));
            Exe testExe = new Exe();
            Debug.Print("One plus two = " + testExe.OnePlusTwo());
            Debug.Print("Two plus four = " + testExe.TwoPlusFour());
        }
    }
}
