using System;
using Microsoft.SPOT;
using CMSIS;

namespace MathFuncsTest
{
    public class Program
    {
        public static void Main()
        {

            /*Conversion between float and Q31*/
            float fl1 = 0.18f;
            int int1 = 25;

            Debug.Print(fl1 + " to Q31 = " + Support.ConvertFloatToQ31(fl1) + " and back to float = " + Support.ConvertQ31ToFloat(Support.ConvertFloatToQ31(fl1)));
            Debug.Print(int1 + " to float = " + Support.ConvertQ31ToFloat(int1) + " and back to int = " + Support.ConvertFloatToQ31(Support.ConvertQ31ToFloat(int1)));


            /*Matrix initialization*/
            float[] floatarr1 = new float[] { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f };
            float[] floatarr2 = new float[] { 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f };
            float[] floatarr3 = new float[] { 8.0f, 7.0f, 6.0f, 5.0f, 4.0f, 3.0f };

            Matrix M1 = new Matrix(2, 3, floatarr1);
            Matrix M2 = new Matrix(3, 2, floatarr2);
            Matrix M3 = new Matrix(2, 3, floatarr3);

            /*Matrix multiplication*/
            Debug.Print("############");
            Debug.Print("# MATRIX 1 #");
            Debug.Print("############");

            Debug.Print("Rows: " + M1.rows + "; cols: " + M1.cols + "; data:");
            foreach (var item in M1.data)
            {
                Debug.Print(item.ToString());
            }

            Debug.Print("############");
            Debug.Print("# MATRIX 2 #");
            Debug.Print("############");

            Debug.Print("Rows: " + M2.rows + "; cols: " + M2.cols + "; data:");
            foreach (var item in M2.data)
            {
                Debug.Print(item.ToString());
            }

            Matrix outmat = MatrixFuncs.MatrixMult(M1, M2);

            Debug.Print("################################");
            Debug.Print("# MATRIX MULTIPLICATION OUTPUT #");
            Debug.Print("################################");

            Debug.Print("Out rows: " + outmat.rows + "; cols: " + outmat.cols + "; data:");
            foreach (float item in outmat.data)
            {
                Debug.Print(item.ToString());
            }

            /*Matrix addition*/
            Debug.Print("############");
            Debug.Print("# MATRIX 1 #");
            Debug.Print("############");

            Debug.Print("Rows: " + M1.rows + "; cols: " + M1.cols + "; data:");
            foreach (var item in M1.data)
            {
                Debug.Print(item.ToString());
            }

            Debug.Print("############");
            Debug.Print("# MATRIX 3 #");
            Debug.Print("############");

            Debug.Print("Rows: " + M3.rows + "; cols: " + M3.cols + "; data:");
            foreach (var item in M3.data)
            {
                Debug.Print(item.ToString());
            }

            outmat = MatrixFuncs.MatrixAdd(M1, M3);

            Debug.Print("##########################");
            Debug.Print("# MATRIX ADDITION OUTPUT #");
            Debug.Print("##########################");

            Debug.Print("Out rows: " + outmat.rows + "; cols: " + outmat.cols + "; data:");
            foreach (float item in outmat.data)
            {
                Debug.Print(item.ToString());
            }

            /*Matrix subtraction*/
            outmat = MatrixFuncs.MatrixSub(M1, M3);

            Debug.Print("#############################");
            Debug.Print("# MATRIX SUBTRACTION OUTPUT #");
            Debug.Print("#############################");

            Debug.Print("Out rows: " + outmat.rows + "; cols: " + outmat.cols + "; data:");
            foreach (float item in outmat.data)
            {
                Debug.Print(item.ToString());
            }

            /*Matrix transpose*/
            Debug.Print("############");
            Debug.Print("# MATRIX 1 #");
            Debug.Print("############");

            Debug.Print("Rows: " + M1.rows + "; cols: " + M1.cols + "; data:");
            foreach (var item in M1.data)
            {
                Debug.Print(item.ToString());
            }

            Debug.Print("######################");
            Debug.Print("# MATRIX 1 TRANSPOSE #");
            Debug.Print("######################");

            outmat = MatrixFuncs.MatrixTrans(M1);

            Debug.Print("Out rows: " + outmat.rows + "; cols: " + outmat.cols + "; data:");
            foreach (float item in outmat.data)
            {
                Debug.Print(item.ToString());
            }

            Debug.Print("############");
            Debug.Print("# MATRIX 2 #");
            Debug.Print("############");

            Debug.Print("Rows: " + M2.rows + "; cols: " + M2.cols + "; data:");
            foreach (var item in M2.data)
            {
                Debug.Print(item.ToString());
            }

            Debug.Print("######################");
            Debug.Print("# MATRIX 2 TRANSPOSE #");
            Debug.Print("######################");

            outmat = MatrixFuncs.MatrixTrans(M2);

            Debug.Print("Out rows: " + outmat.rows + "; cols: " + outmat.cols + "; data:");
            foreach (float item in outmat.data)
            {
                Debug.Print(item.ToString());
            }

            Debug.Print("############");
            Debug.Print("# MATRIX 3 #");
            Debug.Print("############");

            Debug.Print("Rows: " + M3.rows + "; cols: " + M3.cols + "; data:");
            foreach (var item in M3.data)
            {
                Debug.Print(item.ToString());
            }

            Debug.Print("######################");
            Debug.Print("# MATRIX 1 TRANSPOSE #");
            Debug.Print("######################");

            outmat = MatrixFuncs.MatrixTrans(M3);

            Debug.Print("Out rows: " + outmat.rows + "; cols: " + outmat.cols + "; data:");
            foreach (float item in outmat.data)
            {
                Debug.Print(item.ToString());
            }

            /*Vector copy*/
            Debug.Print("############");
            Debug.Print("# VECTOR 1 #");
            Debug.Print("############");

            Debug.Print("Data:");
            foreach (var item in floatarr1)
            {
                Debug.Print(item.ToString());

            }

            float[] copyarr1 = Support.VectorCopy(floatarr1);

            Debug.Print("#################");
            Debug.Print("# VECTOR 1 COPY #");
            Debug.Print("#################");

            Debug.Print("Data:");
            foreach (var item in copyarr1)
            {
                Debug.Print(item.ToString());

            }

            float scal1 = 3.5f;

            /*Vector fill with scalar */
            Debug.Print("############");
            Debug.Print("# SCALAR 1 #");
            Debug.Print("############");

            Debug.Print("Value:" +scal1 );

            float[] fillarr1 = Support.VectorFill(6, scal1);
            Debug.Print("#############################");
            Debug.Print("# VECTOR FILL WITH SCALAR 1 #");
            Debug.Print("#############################");

            Debug.Print("Vector length: " + fillarr1.Length + "; data:");
            foreach (var item in fillarr1)
            {
                Debug.Print(item.ToString());

            }
        }
    }
}
