using System;
using Microsoft.SPOT;
using CMSIS;

namespace MathFuncsTest
{
    public class Program
    {
        public static void Main()
        {
            const float largeFactor = 20.0f; // Suitably large factor, for downscaling. The larger this factor, the better the chance of
                                             // matrix multiplication not causing Q31 overflow, at the cost of precision

            float fl1 = 0.18f;
            int int1 = 25;

            Debug.Print(fl1 + " to Q31 = " + Support.ConvertFloatToQ31(fl1) + " and back to float = " + Support.ConvertQ31ToFloat(Support.ConvertFloatToQ31(fl1)));
            Debug.Print(int1 + " to float = " + Support.ConvertQ31ToFloat(int1) + " and back to int = " + Support.ConvertFloatToQ31(Support.ConvertQ31ToFloat(int1)));

            float[] floatarr1 = new float[] { 1.0f/largeFactor, 2.0f/largeFactor, 3.0f/largeFactor, 4.0f/largeFactor, 5.0f/largeFactor, 6.0f/largeFactor };
            int[] q31arr1 = new int[6];
            float[] floatbackarr1 = new float[6];

            Support.ConvertFloatToQ31(floatarr1, q31arr1);

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

            Support.ConvertQ31ToFloat(q31arr1, floatbackarr1);

            Debug.Print("Converted back to float array: ");
            foreach (var item in floatbackarr1)
            {
                Debug.Print(item.ToString());
            }

            float[] floatarr2 = new float[] { 7.0f/largeFactor, 8.0f/largeFactor, 9.0f/largeFactor, 10.0f/largeFactor, 11.0f/largeFactor, 12.0f/largeFactor };
            int[] q31arr2 = new int[6];

            Support.ConvertFloatToQ31(floatarr2, q31arr2);

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

            float[] outmatfloat = new float[outmat.rows*outmat.cols];
            Support.ConvertQ31ToFloat(outmat.data, outmatfloat);

            Debug.Print("################################");
            Debug.Print("# MATRIX MULTIPLICATION OUTPUT #");
            Debug.Print("################################");

            Debug.Print("Out rows: " + outmat.rows + "; cols: " + outmat.cols + "; data:");
            foreach (float item in outmatfloat)
            {
                Debug.Print((item * largeFactor * largeFactor).ToString());
            }

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

            float[] floatarr3 = new float[] { 8.0f / largeFactor, 7.0f / largeFactor, 6.0f / largeFactor, 5.0f / largeFactor, 4.0f / largeFactor, 3.0f / largeFactor };
            int[] q31arr3 = new int[6];

            Support.ConvertFloatToQ31(floatarr3, q31arr3);

            Debug.Print("############");
            Debug.Print("# MATRIX 3 #");
            Debug.Print("############");

            Debug.Print("Scaled float array: ");
            foreach (var item in floatarr3)
            {
                Debug.Print(item.ToString());
            }

            Debug.Print("Converted Q31 array: ");
            foreach (var item in q31arr3)
            {
                Debug.Print(item.ToString());
            }

            Matrix M3 = new Matrix(2, 3, q31arr3);

            MatrixFuncs.MatrixAdd(M1, M3, out outmat);
            outmatfloat = new float[outmat.rows * outmat.cols];
            Support.ConvertQ31ToFloat(outmat.data, outmatfloat);

            Debug.Print("##########################");
            Debug.Print("# MATRIX ADDITION OUTPUT #");
            Debug.Print("##########################");

            Debug.Print("Out rows: " + outmat.rows + "; cols: " + outmat.cols + "; data:");
            foreach (float item in outmatfloat)
            {
                Debug.Print((item * largeFactor).ToString());
            }

            MatrixFuncs.MatrixSub(M1, M3, out outmat);
            outmatfloat = new float[outmat.rows * outmat.cols];
            Support.ConvertQ31ToFloat(outmat.data, outmatfloat);

            Debug.Print("#############################");
            Debug.Print("# MATRIX SUBTRACTION OUTPUT #");
            Debug.Print("#############################");

            Debug.Print("Out rows: " + outmat.rows + "; cols: " + outmat.cols + "; data:");
            foreach (float item in outmatfloat)
            {
                Debug.Print((item * largeFactor).ToString());
            }

            Debug.Print("######################");
            Debug.Print("# MATRIX 1 TRANSPOSE #");
            Debug.Print("######################");

            MatrixFuncs.MatrixTrans(M1, out outmat);
            outmatfloat = new float[outmat.rows * outmat.cols];
            Support.ConvertQ31ToFloat(outmat.data, outmatfloat);

            Debug.Print("Out rows: " + outmat.rows + "; cols: " + outmat.cols + "; data:");
            foreach (float item in outmatfloat)
            {
                Debug.Print((item * largeFactor).ToString());
            }

            Debug.Print("######################");
            Debug.Print("# MATRIX 2 TRANSPOSE #");
            Debug.Print("######################");

            MatrixFuncs.MatrixTrans(M2, out outmat);
            outmatfloat = new float[outmat.rows * outmat.cols];
            Support.ConvertQ31ToFloat(outmat.data, outmatfloat);

            Debug.Print("Out rows: " + outmat.rows + "; cols: " + outmat.cols + "; data:");
            foreach (float item in outmatfloat)
            {
                Debug.Print((item * largeFactor).ToString());
            }

            Debug.Print("######################");
            Debug.Print("# MATRIX 3 TRANSPOSE #");
            Debug.Print("######################");

            MatrixFuncs.MatrixTrans(M3, out outmat);
            outmatfloat = new float[outmat.rows * outmat.cols];
            Support.ConvertQ31ToFloat(outmat.data, outmatfloat);

            Debug.Print("Out rows: " + outmat.rows + "; cols: " + outmat.cols + "; data:");
            foreach (float item in outmatfloat)
            {
                Debug.Print((item * largeFactor).ToString());
            }


            int[] q31arr1copy = new int[q31arr1.Length];
            Support.VectorCopy(q31arr1, q31arr1copy);
            Debug.Print("############");
            Debug.Print("# VECTOR 1 #");
            Debug.Print("############");

            Debug.Print("Scaled float array: ");
            foreach (var item in floatarr1)
            {
                Debug.Print((item * largeFactor).ToString());
            }

            float[] floatarr1copy = new float[q31arr1copy.Length];
            Support.ConvertQ31ToFloat(q31arr1copy, floatarr1copy);
            Debug.Print("Copy: ");
            foreach (var item in floatarr1copy)
            {
                Debug.Print((item*largeFactor).ToString());
            }

            float scalar = 3.5f;
            int[] fillVec = new int[6];
            Support.VectorFill(Support.ConvertFloatToQ31(scalar / largeFactor), fillVec);

            Debug.Print("##################");
            Debug.Print("# FILLING VECTOR #");
            Debug.Print("##################");
            Debug.Print("Scalar: " + scalar);

            float[] fillVecFloat = new float[fillVec.Length];
            Support.ConvertQ31ToFloat(fillVec, fillVecFloat);
            
            Debug.Print("Filled vector: ");
            foreach (var item in fillVecFloat)
            {
                Debug.Print((item * largeFactor).ToString());
            }
        }
    }
}
