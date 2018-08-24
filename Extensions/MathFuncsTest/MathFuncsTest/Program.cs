using System;
using Microsoft.SPOT;
using CMSIS;

namespace MathFuncsTest
{
    public class Program
    {
        public static void Main()
        {
            const float largeFactor = 20.0f; // Suitably large factor, for downscaling. The larger the better chance of not 
                                             // overflowing, at the cost of precision

            float fl1 = 0.18f;
            int int1 = 25;

            Debug.Print(fl1 + " to Q31 = " + Support.ConvertFloatToInt(fl1) + " and back to float = " + Support.ConvertIntToFloat(Support.ConvertFloatToInt(fl1)));
            Debug.Print(int1 + " to float = " + Support.ConvertIntToFloat(int1) + " and back to int = " + Support.ConvertFloatToInt(Support.ConvertIntToFloat(int1)));

            float[] floatarr1 = new float[] { 1.0f/largeFactor, 2.0f/largeFactor, 3.0f/largeFactor, 4.0f/largeFactor, 5.0f/largeFactor, 6.0f/largeFactor };
            int[] q31arr1 = new int[6];
            float[] floatbackarr1 = new float[6];

            Support.ConvertFloatToInt(floatarr1, q31arr1);

            Debug.Print("############");
            Debug.Print("# MATRIX 1 #");
            Debug.Print("############");
            
            Debug.Print("Scaled float array: ");
            foreach (var item in floatarr1)
            {
                Debug.Print(item.ToString());
            }

            Debug.Print("Converted Q31 array: ");
            foreach (var item in q31arr1)
            {
                Debug.Print(item.ToString());
            }

            Support.ConvertIntToFloat(q31arr1, floatbackarr1);

            Debug.Print("Converted back to float array: ");
            foreach (var item in floatbackarr1)
            {
                Debug.Print(item.ToString());
            }

            float[] floatarr2 = new float[] { 7.0f/largeFactor, 8.0f/largeFactor, 9.0f/largeFactor, 10.0f/largeFactor, 11.0f/largeFactor, 12.0f/largeFactor };
            int[] q31arr2 = new int[6];

            Support.ConvertFloatToInt(floatarr2, q31arr2);

            Debug.Print("############");
            Debug.Print("# MATRIX 2 #");
            Debug.Print("############");

            Debug.Print("Scaled float array: ");
            foreach (var item in floatarr2)
            {
                Debug.Print(item.ToString());
            }

            Debug.Print("Converted Q31 array: ");
            foreach (var item in q31arr2)
            {
                Debug.Print(item.ToString());
            }

            Matrix M1 = new Matrix(2, 3, q31arr1);
            Matrix M2 = new Matrix(3, 2, q31arr2);
            Matrix outmat;
            MatrixFuncs.MatrixMult(M1, M2, out outmat);

            float[] matmultout = new float[outmat.rows*outmat.cols];
            Support.ConvertIntToFloat(outmat.data, matmultout);

            Debug.Print("################################");
            Debug.Print("# MATRIX MULTIPLICATION OUTPUT #");
            Debug.Print("################################");

            Debug.Print("Out rows: " + outmat.rows + "; cols: " + outmat.cols + "; data:");
            foreach (float item in matmultout)
            {
                Debug.Print((item * largeFactor * largeFactor).ToString());
            }

        }
    }
}
