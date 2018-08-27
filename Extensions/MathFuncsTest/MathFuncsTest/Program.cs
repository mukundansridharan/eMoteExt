using System;
using System.Threading;
using Microsoft.SPOT;
using CMSIS;

namespace MathFuncsTest
{
    public class Program
    {
        public static void Main()
        {
            Thread.Sleep(5000);
            
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

            /*Vector definitions*/
            float[] vec1 = new float[] { 1.0200f, 2.0400f, 6.0900f, -3.1200f, 9.3300f };
            float[] vec2 = new float[] { -12.1100f, 2.4500f, 6.6000f, 4.5000f, -3.2200f };

            /*Vector absolute*/
            Debug.Print("############");
            Debug.Print("# VECTOR 1 #");
            Debug.Print("############");

            Debug.Print("Data:");
            foreach (var item in vec1)
            {
                Debug.Print(item.ToString());
            }

            float[] vec1abs = VectorFuncs.VectorAbs(vec1);

            Debug.Print("################");
            Debug.Print("# VECTOR 1 ABS #");
            Debug.Print("################");

            Debug.Print("Data:");
            foreach (var item in vec1abs)
            {
                Debug.Print(item.ToString());

            }

            /*Vector negative*/
            float[] vec1neg = VectorFuncs.VectorNegate(vec1);

            Debug.Print("################");
            Debug.Print("# VECTOR 1 NEG #");
            Debug.Print("################");

            Debug.Print("Data:");
            foreach (var item in vec1neg)
            {
                Debug.Print(item.ToString());

            }

            /*Vector offset*/
            float[] vec1offset = VectorFuncs.VectorOffset(vec1, scal1);

            Debug.Print("###################");
            Debug.Print("# VECTOR 1 OFFSET #");
            Debug.Print("###################");
            Debug.Print("Scalar: " + scal1);

            Debug.Print("Data:");
            foreach (var item in vec1offset)
            {
                Debug.Print(item.ToString());
            }

            /*Vector addition*/
            Debug.Print("############");
            Debug.Print("# VECTOR 1 #");
            Debug.Print("############");

            Debug.Print("Data:");
            foreach (var item in vec1)
            {
                Debug.Print(item.ToString());
            }

            Debug.Print("############");
            Debug.Print("# VECTOR 2 #");
            Debug.Print("############");

            Debug.Print("Data:");
            foreach (var item in vec2)
            {
                Debug.Print(item.ToString());
            }

            float[] vecadd = VectorFuncs.VectorAdd(vec1, vec2);

            Debug.Print("##############");
            Debug.Print("# VECTOR ADD #");
            Debug.Print("##############");

            Debug.Print("Data:");
            foreach (var item in vecadd)
            {
                Debug.Print(item.ToString());
            }

            /*Vector subtraction*/
            float[] vecsub = VectorFuncs.VectorSub(vec1, vec2);

            Debug.Print("##############");
            Debug.Print("# VECTOR SUB #");
            Debug.Print("##############");

            Debug.Print("Data:");
            foreach (var item in vecsub)
            {
                Debug.Print(item.ToString());
            }

            /*Vector Hadamard product*/
            float[] vecprod = VectorFuncs.VectorHadamard(vec1, vec2);

            Debug.Print("###########################");
            Debug.Print("# VECTOR HADAMARD PRODUCT #");
            Debug.Print("###########################");

            Debug.Print("Data:");
            foreach (var item in vecprod)
            {
                Debug.Print(item.ToString());
            }

            /*Vector dot product*/
            float vecdot = VectorFuncs.VectorDot(vec1, vec2);

            Debug.Print("######################");
            Debug.Print("# VECTOR DOT PRODUCT #");
            Debug.Print("######################");

            Debug.Print("Value:" + vecdot);

            /*Vector statistics*/
            Debug.Print("#######################");
            Debug.Print("# VECTOR 1 STATISTICS #");
            Debug.Print("#######################");

            Debug.Print("Data:");
            foreach (var item in vec1)
            {
                Debug.Print(item.ToString());
            }

            UInt32 maxIndex, minIndex;
            float max = Stats.Max(vec1, out maxIndex);
            float min = Stats.Min(vec1, out minIndex);
            Debug.Print("Max: " + max + " at index: " + maxIndex);
            Debug.Print("Min: " + min + " at index: " + minIndex);
            Debug.Print("Mean: " + Stats.Mean(vec1));
            Debug.Print("Standard deviation: " + Stats.StD(vec1));
            Debug.Print("Variance: " + Stats.Var(vec1));
        }
    }
}
