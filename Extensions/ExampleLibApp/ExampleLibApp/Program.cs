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
            Debug.Print("Addition with args (internal non-stub function call): " + arg1 + " plus " + arg2 + " = " + testExe.Arg1PlusArg2_IntFunc(arg1, arg2));
            Debug.Print("Exponentiation with cmath library: " + arg1 + "^" + arg2 + " = " + testExe.IntExp_Lib(arg1, arg2));

            Exe.Arg struct_in = new Exe.Arg();
            struct_in.size = 5;
            struct_in.array = new UInt16[] {1, 2, 3, 4, 5};

            Exe.Arg struct_out = new Exe.Arg();
            struct_out.array = new UInt16[struct_in.size];

            testExe.CustomArgsIO(struct_in, out struct_out.size, struct_out.array);

            Debug.Print("Input struct: size = " + struct_in.size + "; array: ");
            foreach (var item in struct_in.array)
            {
                Debug.Print(item.ToString());
            }

            Debug.Print("Output struct: size = " + struct_out.size + "; array: ");
            foreach (var item in struct_out.array)
            {
                Debug.Print(item.ToString());
            }
        }
    }
}
