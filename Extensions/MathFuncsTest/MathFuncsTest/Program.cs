using System;
using Microsoft.SPOT;
using CMSIS;

namespace MathFuncsTest
{
    public class Program
    {
        public static void Main()
        {
            float fl1 = 0.18f;
            int int1 = 25;

            Debug.Print(fl1 + " to Q31 = " + Support.ConvertFloatToInt(fl1) + " and back to float = " + Support.ConvertIntToFloat(Support.ConvertFloatToInt(fl1)));
            Debug.Print(int1 + " to float = " + Support.ConvertIntToFloat(int1) + " and back to int = " + Support.ConvertFloatToInt(Support.ConvertIntToFloat(int1)));
        }
    }
}
