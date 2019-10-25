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
            Debug.Print("Connect Logic Analyzer now");
            Thread.Sleep(5000);

            // Integer vector operations
            int[] vec1datq = new int[] { -200, -12, 6, 3, 99 };
            int[] vec2datq = new int[] { -5542, 22, 56, 124, -223 };

            Vector vec1q = new Vector(vec1datq);
            Vector vec2q = new Vector(vec2datq);

            Vector vecaddq = vec1q + vec2q;

            /*Vector addition*/
            Debug.Print("####################");
            Debug.Print("# INTEGER VECTOR 1 #");
            Debug.Print("####################");

            Debug.Print("Data:");
            foreach (var item in vec1q.GetData())
            {
                Debug.Print(item.ToString());
            }

            Debug.Print("####################");
            Debug.Print("# INTEGER VECTOR 2 #");
            Debug.Print("####################");

            Debug.Print("Data:");
            foreach (var item in vec2q.GetData())
            {
                Debug.Print(item.ToString());
            }


            Debug.Print("##########################");
            Debug.Print("# INTEGER VECTOR ADD 1+2 #");
            Debug.Print("##########################");

            Debug.Print("Data:");
            foreach (var item in vecaddq.GetData())
            {
                Debug.Print(item.ToString());
            }


            // Integer matrix operations
            /*Matrix initialization*/
            int[] intarr1 = new int[] { 73,33,86,14,20,16,0,64,14,35,98,26,97,48,32,41,55,86,50,20,0,24,33,39,64,4,17,63,78,41,20,4,4,10,18,42,84,82,2,59,28,88,43,11,12,94,73,30,65,8,83,67,56,66,60,20,72,6,91,31,1,26,72,51,43,51,71,98,43,77,83,83,5,18,42,4,55,48,17,75,35,6,34,8,27,97,58,18,67,17,41,95,68,18,85,12,38,35,6,98,60,28,84,73,50,15,44,93,74,97,3,82,46,26,60,71,82,42,72,36,52,55,63,48,76,8,41,74,72,71,55,53,29,95,64,78,39,22,25,74,32,41,10,23,97,62,5,15,27,90,55,52,55,58,71,33,70,3,76,94,10,7,76,95,9,29,63,65,6,12,53,26,72,37,35,32,51,11,60,57,89,60,76,54,55,34,13,27,33,49,60,26 };
            int[] intarr2 = new int[] { 36, 12, 82, 46, 69, 13, 21, 61, 32, 23, 25, 0, 57, 21, 19, 58, 28, 51, 68, 7, 21, 89, 50, 65, 88, 20, 55, 97, 75, 49, 94, 86 };
            int[] intarr3 = new int[] { 93, 30, 34, 66, 1, 43, 90, 51, 76, 60, 95, 51, 82, 80, 5, 82, 3, 29, 49, 50, 99, 78, 33, 22, 96, 20, 44, 48, 27, 56, 61, 39, 78, 88, 33, 12, 58, 15, 94, 66, 8, 23, 66, 8, 24, 84, 12, 76, 48, 58, 32, 40, 6, 0, 73, 27, 83, 84, 61, 97, 80, 91, 80, 5, 28, 53, 45, 30, 57, 3, 96, 14, 46, 42, 32, 76, 28, 15, 41, 13, 95, 37, 73, 85, 1, 79, 12, 48, 99, 53, 17, 1, 47, 41, 47, 0, 84, 15, 53, 64, 98, 63, 80, 76, 70, 80, 12, 90, 36, 57, 31, 7, 75, 51, 35, 82, 61, 15, 76, 45, 85, 48, 26, 61, 0, 37, 99, 35, 67, 73, 51, 9, 7, 43, 2, 49, 28, 64, 90, 64, 8, 88, 89, 93, 80, 42, 6, 56, 17, 75, 84, 87, 92, 73, 37, 80, 98, 96, 18, 90, 90, 46, 25, 79, 2, 78, 68, 33, 82, 11, 58, 94, 43, 8, 63, 43, 97, 34, 58, 74, 17, 44, 56, 18, 89, 6, 20, 18, 7, 37, 82, 70 };
            int[] intarr4 = new int[] { 69, 30, 60, 90, 91, 81, 86, 40, 66, 92, 89, 67, 62, 33, 95, 4, 24, 74, 51, 75, 51, 15, 20, 17, 95, 63, 15, 43, 11, 22, 78, 3, 28, 76, 4, 96, 92, 45, 49, 61, 70, 2, 12, 27, 32, 63, 44, 36, 33, 8, 75, 24, 17, 82, 3, 56, 27, 31, 68, 62, 63, 58, 64, 14, 81, 38, 39, 57, 42, 27, 6, 9, 76, 41, 41, 17, 78, 62, 18, 59, 47, 58, 97, 92, 20, 95, 61, 26, 25, 82, 7, 96, 10, 97, 92, 4, 26, 16, 74, 64, 53, 58, 40, 77, 77, 86, 17, 65, 59, 94, 53, 73, 90, 50, 66, 29, 52, 45, 27, 40, 67, 27, 67, 68, 68, 7, 66, 43, 91, 52, 52, 94, 26, 13, 91, 3, 24, 75, 32, 87, 41, 78, 74, 56, 42, 91, 39, 39, 34, 40, 79, 1, 18, 28, 84, 31, 82, 92, 17, 52, 91, 31, 38, 98, 98, 35, 29, 18, 78, 39, 63, 69, 66, 78, 60, 22, 4, 37, 90, 9, 24, 1, 81, 98, 57, 43, 30, 50, 88, 1, 42, 61, 21, 63, 38, 41, 50, 42, 78, 19, 32, 34, 14, 78, 84, 22, 18, 29, 15, 88, 19, 52, 70, 8, 94, 28, 26, 24, 91, 53, 18, 53, 98, 86, 67, 96, 44, 28, 98, 93, 81, 41, 87, 78, 86, 56, 67, 22, 0, 16, 28, 12, 14, 90, 20, 98, 97, 3, 32, 67, 50, 56, 61, 70, 60, 73, 14, 95, 69, 77, 53, 49, 47, 79, 70, 3, 84, 61, 3, 18, 58, 66, 26, 62, 87, 27, 13, 47, 12, 69, 27, 77, 74, 75, 71, 24, 50, 44, 85, 31, 8, 93, 96, 43, 31, 33, 47, 59, 72, 12, 12, 76, 59, 55, 80, 94, 47, 0, 38, 81, 8, 47, 29, 74, 63, 75, 42, 10, 49, 95, 58, 68, 52, 78, 26, 71, 53, 88, 67, 86, 31, 82, 31, 6, 22, 77, 59, 68, 95, 27, 37, 73, 44, 79, 22, 16, 22, 83, 4, 82, 42, 87, 32, 40, 32, 68, 96, 40, 67, 59, 42, 71, 53, 90, 51, 91, 29, 9, 35, 68, 79, 68, 29, 98, 24, 94, 86, 30, 6, 69, 89, 57, 35, 41, 47, 18, 49, 2, 78, 44, 36, 4, 44, 56, 65, 49, 7, 75, 64, 69, 62, 2, 0, 49, 75, 22, 4, 42, 3, 40, 45, 71, 76, 72, 4, 75, 72, 72, 54, 40, 12, 14, 47, 44, 5, 31, 40, 75, 52, 54, 97, 72, 1, 18, 89, 42, 85, 42, 56, 82, 76, 68, 50, 40, 11, 45, 63, 18, 92, 86, 44, 80, 83, 66, 21, 59, 71, 26, 55, 24, 77, 66, 49, 33, 90, 91, 95, 65, 48, 54, 29, 94, 49, 80, 1, 33, 99, 10, 39, 56, 43, 0, 49, 38, 63, 7, 36, 57, 52, 72, 56, 98, 36, 7, 60, 25, 10, 78, 83, 77, 85, 49, 65, 78, 54, 97, 24, 72, 95, 47, 31, 60, 67, 14, 91, 45, 97, 7, 50, 48, 48, 99, 31, 3, 2, 60, 32, 85, 37, 92, 95, 97, 11, 73, 40, 97, 77, 33, 45, 45, 77, 14, 17, 74, 24, 59, 7, 19, 95, 2, 22, 0, 54, 72, 56, 66, 84, 45, 54, 93, 47, 57, 79, 22, 90, 87, 21, 53, 30, 33, 41, 73, 53, 84, 93, 58, 3, 98, 0, 49, 34, 76, 0, 15, 46, 79, 97, 60, 30, 96, 98, 47, 35, 75, 64, 0, 95, 93, 65, 75, 20, 90, 0, 15, 19, 89, 18, 29, 23, 49, 57, 89, 21, 64, 65, 76, 4, 65, 24, 90, 40, 29, 7, 76, 64, 9, 8, 9, 26, 90, 84, 30, 88, 64, 10, 70, 42, 72, 58, 82, 19, 95, 73, 73, 1, 2, 60, 26, 95, 51, 79, 72, 89, 93, 7, 25, 7, 47, 21, 78, 24, 67, 68, 69, 73, 17, 96, 76, 75, 1, 91, 84, 40, 9, 54, 20, 15, 4, 38, 54, 15, 78, 22, 91, 35, 73, 69, 66, 79, 66, 14, 28, 87, 91, 62, 50, 8, 33, 82, 24, 76, 25, 25, 56, 58, 87, 33, 12, 46, 50, 64, 21, 86, 62, 70, 43, 40, 62, 69, 60, 88, 36, 93, 66, 22, 3, 41, 77, 98, 79, 9, 62, 18, 72, 63, 31, 56, 9, 71, 97, 89, 84, 52, 32, 44, 83, 43, 66, 93, 61, 13, 97, 32, 27, 6, 52, 73, 72, 5, 83, 20, 80, 33, 18, 61, 79, 93, 77, 77, 35, 77, 19, 93, 99, 32, 63, 47, 68, 27, 63, 26, 5, 35, 9, 96, 52, 9, 75, 78, 86, 19, 65, 44, 54, 7, 82, 90, 12, 61, 24, 22, 81, 27, 95, 58, 9, 71, 96, 67, 91, 20, 62, 54, 29, 55, 28, 0, 53, 94, 58, 38, 39, 97, 33, 37, 67, 62, 51, 64, 22, 75, 44, 92, 93, 30, 32, 56, 75, 97, 49, 23, 3, 77, 70, 95, 70, 42, 60, 15, 66, 81, 77, 32, 30, 76, 35, 40, 23, 28, 84, 26, 69, 38, 42, 77, 91, 70, 88, 35, 7, 91, 35, 82, 70, 91, 69, 39, 55, 37, 33, 77, 17, 28, 59, 45, 66, 86, 51, 90, 9, 2, 68, 20, 88, 82, 54, 83, 55, 74, 90, 41, 70, 19, 74, 99, 62, 44, 74, 67, 40, 98, 47, 88, 27, 30, 13, 73, 23, 54, 61, 64, 56, 99, 59, 35, 44, 54, 74, 3, 96, 29, 41, 99, 73, 59, 49, 53, 63, 97, 73, 29, 27, 96, 57, 89, 35, 14, 27, 64, 24, 63, 11, 96, 68, 15, 30, 97, 48, 11, 71, 99, 38, 94, 55, 74, 58, 13, 14, 32, 58, 99, 27, 85, 26, 43, 45, 65, 86, 36, 7, 14, 79, 29, 94, 25, 35, 73, 77, 26, 44, 68, 87, 41, 36, 30, 75, 55, 94, 76, 77, 99, 6, 51, 77, 93, 84, 93, 58, 58, 33, 13, 33, 73, 51, 59, 79, 79, 33, 52, 56, 62, 42, 79, 32 };

            Matrix M1 = new Matrix(32, 6, intarr1);
            Matrix M2 = new Matrix(6, 32, intarr2);
            Matrix M3 = new Matrix(32, 1, intarr3);
            Matrix M4 = new Matrix(32, 32, intarr4);

            /*Matrix multiplication*/
            /*Debug.Print("############");
            Debug.Print("# MATRIX 1 #");
            Debug.Print("############");

            Debug.Print("Rows: " + M1.GetRows() + "; cols: " + M1.GetCols() + "; data:");
            foreach (var item in M1.GetData())
            {
                Debug.Print(item.ToString());
            }

            Debug.Print("############");
            Debug.Print("# MATRIX 2 #");
            Debug.Print("############");

            Debug.Print("Rows: " + M2.GetRows() + "; cols: " + M2.GetCols() + "; data:");
            foreach (var item in M2.GetData())
            {
                Debug.Print(item.ToString());
            }*/

            Matrix outmat = MatrixFuncs.MatrixMult(M1, MatrixFuncs.MatrixMult(M2,M3));

            Matrix outmat2 = MatrixFuncs.MatrixMult(M4, M3);


            Debug.Print("########################################");
            Debug.Print("# INTEGER MATRIX MULTIPLICATION OUTPUT #");
            Debug.Print("########################################");

            Debug.Print("Out rows: " + outmat.GetRows() + "; cols: " + outmat.GetCols() + "; data:");
            foreach (int item in outmat.GetData())
            {
                Debug.Print(item.ToString());
            }

            return;

            /*Matrix addition*/
            Debug.Print("############");
            Debug.Print("# MATRIX 1 #");
            Debug.Print("############");

            Debug.Print("Rows: " + M1.GetRows() + "; cols: " + M1.GetCols() + "; data:");
            foreach (var item in M1.GetData())
            {
                Debug.Print(item.ToString());
            }

            Debug.Print("############");
            Debug.Print("# MATRIX 3 #");
            Debug.Print("############");

            Debug.Print("Rows: " + M3.GetRows() + "; cols: " + M3.GetCols() + "; data:");
            foreach (var item in M3.GetData())
            {
                Debug.Print(item.ToString());
            }

            outmat = MatrixFuncs.MatrixAdd(M3, M3);

            Debug.Print("##################################");
            Debug.Print("# INTEGER MATRIX ADDITION OUTPUT #");
            Debug.Print("##################################");

            Debug.Print("Out rows: " + outmat.GetRows() + "; cols: " + outmat.GetCols() + "; data:");
            foreach (int item in outmat.GetData())
            {
                Debug.Print(item.ToString());
            }

            return;

            /*Convert sample radar values (interpreted as fixed) to floats*/
            int val1 = 2063;
            int val2 = 1954;
            int val3 = 1949;

            Debug.Print("Q31 " + val1 + " in floating is: " + Support.ScaleConvertQ31ToFloat(val1, GlobalVar.largeFactor) + " and back to Q31 is " + Support.ScaleConvertFloatToQ31(Support.ScaleConvertQ31ToFloat(val1, GlobalVar.largeFactor), GlobalVar.largeFactor));
            Debug.Print("Q31 " + val2 + " in floating is: " + Support.ScaleConvertQ31ToFloat(val2, GlobalVar.largeFactor) + " and back to Q31 is " + Support.ScaleConvertFloatToQ31(Support.ScaleConvertQ31ToFloat(val2, GlobalVar.largeFactor), GlobalVar.largeFactor));
            Debug.Print("Q31 " + val3 + " in floating is: " + Support.ScaleConvertQ31ToFloat(val3, GlobalVar.largeFactor) + " and back to Q31 is " + Support.ScaleConvertFloatToQ31(Support.ScaleConvertQ31ToFloat(val3, GlobalVar.largeFactor), GlobalVar.largeFactor));

            /*Convert small floats to Q31*/
            float valf1 = 1.01f;
            float valf2 = 0.89f;
            float valf3 = 1.50f;

            Debug.Print("Float " + valf1 + " in Q31 is: " + Support.ScaleConvertFloatToQ31(valf1, GlobalVar.largeFactor) + " and back to float is " + Support.ScaleConvertQ31ToFloat(Support.ScaleConvertFloatToQ31(valf1, GlobalVar.largeFactor), GlobalVar.largeFactor));
            Debug.Print("Float " + valf2 + " in Q31 is: " + Support.ScaleConvertFloatToQ31(valf2, GlobalVar.largeFactor) + " and back to float is " + Support.ScaleConvertQ31ToFloat(Support.ScaleConvertFloatToQ31(valf2, GlobalVar.largeFactor), GlobalVar.largeFactor));
            Debug.Print("Float " + valf3 + " in Q31 is: " + Support.ScaleConvertFloatToQ31(valf3, GlobalVar.largeFactor) + " and back to float is " + Support.ScaleConvertQ31ToFloat(Support.ScaleConvertFloatToQ31(valf3, GlobalVar.largeFactor), GlobalVar.largeFactor));

            
           /*Matrix initialization*/
            float[] floatarr1 = new float[] { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f };
            float[] floatarr2 = new float[] { 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f };
            float[] floatarr3 = new float[] { 8.0f, 7.0f, 6.0f, 5.0f, 4.0f, 3.0f };

            M1 = new Matrix(2, 3, floatarr1);
            M2 = new Matrix(3, 2, floatarr2);
            M3 = new Matrix(2, 3, floatarr3);

            /*Matrix multiplication*/
            Debug.Print("############");
            Debug.Print("# MATRIX 1 #");
            Debug.Print("############");

            Debug.Print("Rows: " + M1.GetRows() + "; cols: " + M1.GetCols() + "; data:");
            foreach (var item in M1.GetData())
            {
                Debug.Print(item.ToString());
            }

            Debug.Print("############");
            Debug.Print("# MATRIX 2 #");
            Debug.Print("############");

            Debug.Print("Rows: " + M2.GetRows() + "; cols: " + M2.GetCols() + "; data:");
            foreach (var item in M2.GetData())
            {
                Debug.Print(item.ToString());
            }

            outmat = MatrixFuncs.MatrixMult(M1, M2);

            Debug.Print("################################");
            Debug.Print("# MATRIX MULTIPLICATION OUTPUT #");
            Debug.Print("################################");

            Debug.Print("Out rows: " + outmat.GetRows() + "; cols: " + outmat.GetCols() + "; data:");
            foreach (float item in outmat.GetData())
            {
                Debug.Print(item.ToString());
            }

            /*Matrix addition*/
            Debug.Print("############");
            Debug.Print("# MATRIX 1 #");
            Debug.Print("############");

            Debug.Print("Rows: " + M1.GetRows() + "; cols: " + M1.GetCols() + "; data:");
            foreach (var item in M1.GetData())
            {
                Debug.Print(item.ToString());
            }

            Debug.Print("############");
            Debug.Print("# MATRIX 3 #");
            Debug.Print("############");

            Debug.Print("Rows: " + M3.GetRows() + "; cols: " + M3.GetCols() + "; data:");
            foreach (var item in M3.GetData())
            {
                Debug.Print(item.ToString());
            }

            outmat = MatrixFuncs.MatrixAdd(M1, M3);

            Debug.Print("##########################");
            Debug.Print("# MATRIX ADDITION OUTPUT #");
            Debug.Print("##########################");

            Debug.Print("Out rows: " + outmat.GetRows() + "; cols: " + outmat.GetCols() + "; data:");
            foreach (float item in outmat.GetData())
            {
                Debug.Print(item.ToString());
            }

            /*Matrix subtraction*/
            outmat = MatrixFuncs.MatrixSub(M1, M3);

            Debug.Print("#############################");
            Debug.Print("# MATRIX SUBTRACTION OUTPUT #");
            Debug.Print("#############################");

            Debug.Print("Out rows: " + outmat.GetRows() + "; cols: " + outmat.GetCols() + "; data:");
            foreach (float item in outmat.GetData())
            {
                Debug.Print(item.ToString());
            }

            /*Matrix transpose*/
            Debug.Print("############");
            Debug.Print("# MATRIX 1 #");
            Debug.Print("############");

            Debug.Print("Rows: " + M1.GetRows() + "; cols: " + M1.GetCols() + "; data:");
            foreach (var item in M1.GetData())
            {
                Debug.Print(item.ToString());
            }

            Debug.Print("######################");
            Debug.Print("# MATRIX 1 TRANSPOSE #");
            Debug.Print("######################");

            outmat = MatrixFuncs.MatrixTrans(M1);

            Debug.Print("Out rows: " + outmat.GetRows() + "; cols: " + outmat.GetCols() + "; data:");
            foreach (float item in outmat.GetData())
            {
                Debug.Print(item.ToString());
            }

            Debug.Print("############");
            Debug.Print("# MATRIX 2 #");
            Debug.Print("############");

            Debug.Print("Rows: " + M2.GetRows() + "; cols: " + M2.GetCols() + "; data:");
            foreach (var item in M2.GetData())
            {
                Debug.Print(item.ToString());
            }

            Debug.Print("######################");
            Debug.Print("# MATRIX 2 TRANSPOSE #");
            Debug.Print("######################");

            outmat = MatrixFuncs.MatrixTrans(M2);

            Debug.Print("Out rows: " + outmat.GetRows() + "; cols: " + outmat.GetCols() + "; data:");
            foreach (float item in outmat.GetData())
            {
                Debug.Print(item.ToString());
            }

            Debug.Print("############");
            Debug.Print("# MATRIX 3 #");
            Debug.Print("############");

            Debug.Print("Rows: " + M3.GetRows() + "; cols: " + M3.GetCols() + "; data:");
            foreach (var item in M3.GetData())
            {
                Debug.Print(item.ToString());
            }

            Debug.Print("######################");
            Debug.Print("# MATRIX 3 TRANSPOSE #");
            Debug.Print("######################");

            outmat = MatrixFuncs.MatrixTrans(M3);

            Debug.Print("Out rows: " + outmat.GetRows() + "; cols: " + outmat.GetCols() + "; data:");
            foreach (float item in outmat.GetData())
            {
                Debug.Print(item.ToString());
            }

            /*Vector copy*/
            Debug.Print("############");
            Debug.Print("# VECTOR 0 #");
            Debug.Print("############");

            Debug.Print("Data:");
            foreach (var item in floatarr1)
            {
                Debug.Print(item.ToString());

            }

            float[] copyarr1 = Support.VectorCopy(floatarr1, 0, (uint)floatarr1.Length);

            Debug.Print("#################");
            Debug.Print("# VECTOR 0 COPY #");
            Debug.Print("#################");

            Debug.Print("Data:");
            foreach (var item in copyarr1)
            {
                Debug.Print(item.ToString());
            }

            float[] copyarr2 = Support.VectorCopy(floatarr1, 2, 3);

            Debug.Print("#########################");
            Debug.Print("# VECTOR 0 PARTIAL COPY #");
            Debug.Print("#########################");

            Debug.Print("Data:");
            foreach (var item in copyarr2)
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
            float[] vec1dat = new float[] { 1.0200f, 2.0400f, 6.0900f, -3.1200f, 9.3300f };
            float[] vec2dat = new float[] { -12.1100f, 2.4500f, 6.6000f, 4.5000f, -3.2200f };

            Vector vec1 = new Vector(vec1dat);
            Vector vec2 = new Vector(vec2dat);

            /*Vector absolute*/
            Debug.Print("############");
            Debug.Print("# VECTOR 1 #");
            Debug.Print("############");

            Debug.Print("Data:");
            foreach (var item in vec1.GetData())
            {
                Debug.Print(item.ToString());
            }

            Vector vec1abs = VectorFuncs.VectorAbs(vec1);

            Debug.Print("################");
            Debug.Print("# VECTOR 1 ABS #");
            Debug.Print("################");

            Debug.Print("Data:");
            foreach (var item in vec1abs.GetData())
            {
                Debug.Print(item.ToString());

            }

            /*Vector negative*/
            Vector vec1neg = VectorFuncs.VectorNegate(vec1);

            Debug.Print("################");
            Debug.Print("# VECTOR 1 NEG #");
            Debug.Print("################");

            Debug.Print("Data:");
            foreach (var item in vec1neg.GetData())
            {
                Debug.Print(item.ToString());

            }

            /*Vector offset*/
            Vector vec1offset = VectorFuncs.VectorOffset(vec1, scal1);

            Debug.Print("###################");
            Debug.Print("# VECTOR 1 OFFSET #");
            Debug.Print("###################");
            Debug.Print("Scalar: " + scal1);

            Debug.Print("Data:");
            foreach (var item in vec1offset.GetData())
            {
                Debug.Print(item.ToString());
            }

            /*Vector addition*/
            Debug.Print("############");
            Debug.Print("# VECTOR 1 #");
            Debug.Print("############");

            Debug.Print("Data:");
            foreach (var item in vec1.GetData())
            {
                Debug.Print(item.ToString());
            }

            Debug.Print("############");
            Debug.Print("# VECTOR 2 #");
            Debug.Print("############");

            Debug.Print("Data:");
            foreach (var item in vec2.GetData())
            {
                Debug.Print(item.ToString());
            }

            Vector vecadd = VectorFuncs.VectorAdd(vec1, vec2);

            Debug.Print("##############");
            Debug.Print("# VECTOR ADD #");
            Debug.Print("##############");

            Debug.Print("Data:");
            foreach (var item in vecadd.GetData())
            {
                Debug.Print(item.ToString());
            }

            /*Vector subtraction*/
            Vector vecsub = VectorFuncs.VectorSub(vec1, vec2);

            Debug.Print("##############");
            Debug.Print("# VECTOR SUB #");
            Debug.Print("##############");

            Debug.Print("Data:");
            foreach (var item in vecsub.GetData())
            {
                Debug.Print(item.ToString());
            }

            /*Vector Hadamard product*/
            Vector vecprod = VectorFuncs.VectorHadamard(vec1, vec2);

            Debug.Print("###########################");
            Debug.Print("# VECTOR HADAMARD PRODUCT #");
            Debug.Print("###########################");

            Debug.Print("Data:");
            foreach (var item in vecprod.GetData())
            {
                Debug.Print(item.ToString());
            }

            /*Vector dot product*/
            float vecdot = VectorFuncs.VectorDot(vec1, vec2);

            Debug.Print("######################");
            Debug.Print("# VECTOR DOT PRODUCT #");
            Debug.Print("######################");

            Debug.Print("Value:" + vecdot);

            /*Vector reciprocal*/
            Vector vec1recip = VectorFuncs.VectorRecip(vec1);

            Debug.Print("#######################");
            Debug.Print("# VECTOR 1 RECIPROCAL #");
            Debug.Print("#######################");
            foreach (var item in vec1recip.GetData())
            {
                Debug.Print(item.ToString());
            }

            Vector vec2recip = VectorFuncs.VectorRecip(vec2);

            Debug.Print("#######################");
            Debug.Print("# VECTOR 2 RECIPROCAL #");
            Debug.Print("#######################");
            foreach (var item in vec2recip.GetData())
            {
                Debug.Print(item.ToString());
            }

            /*Vector scaling*/
            Vector vec1sc = VectorFuncs.VectorScale(vec1, scal1);

            Debug.Print("##########################");
            Debug.Print("# VECTOR 1 SCALED BY 3.5 #");
            Debug.Print("##########################");
            foreach (var item in vec1sc.GetData())
            {
                Debug.Print(item.ToString());
            }

            Vector vec2sc = VectorFuncs.VectorScale(vec2, scal1);

            Debug.Print("##########################");
            Debug.Print("# VECTOR 2 SCALED BY 3.5 #");
            Debug.Print("##########################");
            foreach (var item in vec2sc.GetData())
            {
                Debug.Print(item.ToString());
            }

            /*Vector statistics*/
            Debug.Print("#######################");
            Debug.Print("# VECTOR 1 STATISTICS #");
            Debug.Print("#######################");

            Debug.Print("Data:");
            foreach (var item in vec1.GetData())
            {
                Debug.Print(item.ToString());
            }

            UInt32 maxIndex, minIndex;
            float max = Stats.Max(vec1.GetData(), out maxIndex);
            float min = Stats.Min(vec1.GetData(), out minIndex);
            Debug.Print("Max: " + max + " at index: " + maxIndex);
            Debug.Print("Min: " + min + " at index: " + minIndex);
            Debug.Print("Mean: " + Stats.Mean(vec1.GetData()));
            Debug.Print("Standard deviation: " + Stats.StD(vec1.GetData()));
            Debug.Print("Variance: " + Stats.Var(vec1.GetData()));
            
            Debug.Print("\n\nEND UNIT TESTS");
        }
    }
}
