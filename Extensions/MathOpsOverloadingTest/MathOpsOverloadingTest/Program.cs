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

            Matrix outmat = M1 * M2;

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

            outmat = M1 + M3;

            Debug.Print("##########################");
            Debug.Print("# MATRIX ADDITION OUTPUT #");
            Debug.Print("##########################");

            Debug.Print("Out rows: " + outmat.rows + "; cols: " + outmat.cols + "; data:");
            foreach (float item in outmat.data)
            {
                Debug.Print(item.ToString());
            }

            /*Matrix subtraction*/
            outmat = M1 - M3;

            Debug.Print("#############################");
            Debug.Print("# MATRIX SUBTRACTION OUTPUT #");
            Debug.Print("#############################");

            Debug.Print("Out rows: " + outmat.rows + "; cols: " + outmat.cols + "; data:");
            foreach (float item in outmat.data)
            {
                Debug.Print(item.ToString());
            }

            /*Vector definitions*/
            Vector vec1 = new Vector(new float[] { 1.0200f, 2.0400f, 6.0900f, -3.1200f, 9.3300f });
            Vector vec2 = new Vector(new float[] { -12.1100f, 2.4500f, 6.6000f, 4.5000f, -3.2200f });
            Vector vec3 = new Vector(new float[] { -12.1100f, 2.4500f, 6.6000f });

            float scal1 = 3.5f;

            /*Vector offset*/
            Debug.Print("############");
            Debug.Print("# VECTOR 1 #");
            Debug.Print("############");

            Debug.Print("Data:");
            foreach (var item in vec1.vec)
            {
                Debug.Print(item.ToString());
            }

            Vector vec1offset = vec1 + scal1;

            Debug.Print("###################");
            Debug.Print("# VECTOR 1 OFFSET #");
            Debug.Print("###################");
            Debug.Print("Scalar: " + scal1);

            Debug.Print("Data:");
            foreach (var item in vec1offset.vec)
            {
                Debug.Print(item.ToString());
            }

            Vector scal1minusvec1 = scal1-vec1;

            Debug.Print("##############################");
            Debug.Print("# SUBTRACT VECTOR 1 FROM 3.5 #");
            Debug.Print("##############################");
            Debug.Print("Scalar: " + scal1);

            Debug.Print("Data:");
            foreach (var item in scal1minusvec1.vec)
            {
                Debug.Print(item.ToString());
            }

            Debug.Print("############");
            Debug.Print("# VECTOR 2 #");
            Debug.Print("############");

            Debug.Print("Data:");
            foreach (var item in vec2.vec)
            {
                Debug.Print(item.ToString());
            }

            Vector oneminusvec2 = 1 - vec2;

            Debug.Print("###################################");
            Debug.Print("# SUBTRACT VECTOR 2 FROM SCALAR 1 #");
            Debug.Print("###################################");

            Debug.Print("Data:");
            foreach (var item in oneminusvec2.vec)
            {
                Debug.Print(item.ToString());
            }

            /*Vector addition*/
            Debug.Print("############");
            Debug.Print("# VECTOR 1 #");
            Debug.Print("############");

            Debug.Print("Data:");
            foreach (var item in vec1.vec)
            {
                Debug.Print(item.ToString());
            }

            Debug.Print("############");
            Debug.Print("# VECTOR 2 #");
            Debug.Print("############");

            Debug.Print("Data:");
            foreach (var item in vec2.vec)
            {
                Debug.Print(item.ToString());
            }

            Vector vecadd = vec1 + vec2;

            Debug.Print("##############");
            Debug.Print("# VECTOR ADD #");
            Debug.Print("##############");

            Debug.Print("Data:");
            foreach (var item in vecadd.vec)
            {
                Debug.Print(item.ToString());
            }

            /*Vector subtraction*/
            Vector vecsub = vec1 - vec2;

            Debug.Print("##############");
            Debug.Print("# VECTOR SUB #");
            Debug.Print("##############");

            Debug.Print("Data:");
            foreach (var item in vecsub.vec)
            {
                Debug.Print(item.ToString());
            }

            /*Vector reciprocal*/
            Vector vec1recip = 1/vec1;

            Debug.Print("#######################");
            Debug.Print("# VECTOR 1 RECIPROCAL #");
            Debug.Print("#######################");
            foreach (var item in vec1recip.vec)
            {
                Debug.Print(item.ToString());
            }

            Vector vec2recip = 1/vec2;

            Debug.Print("#######################");
            Debug.Print("# VECTOR 2 RECIPROCAL #");
            Debug.Print("#######################");
            foreach (var item in vec2recip.vec)
            {
                Debug.Print(item.ToString());
            }

            /*Vector scaling*/
            Vector vec1sc = scal1*vec1;

            Debug.Print("##########################");
            Debug.Print("# VECTOR 1 SCALED BY 3.5 #");
            Debug.Print("##########################");
            foreach (var item in vec1sc.vec)
            {
                Debug.Print(item.ToString());
            }

            Vector vec2sc = vec2*scal1;

            Debug.Print("##########################");
            Debug.Print("# VECTOR 2 SCALED BY 3.5 #");
            Debug.Print("##########################");
            foreach (var item in vec2sc.vec)
            {
                Debug.Print(item.ToString());
            }

            // Matrix * Vector
            Vector vmult = M1 * vec3;
            Debug.Print("############");
            Debug.Print("# MATRIX 1 #");
            Debug.Print("############");
            foreach (var item in M1.data)
            {
                Debug.Print(item.ToString());
            }

            Debug.Print("############");
            Debug.Print("# VECTOR 3 #");
            Debug.Print("############");
            foreach (var item in vec3.vec)
            {
                Debug.Print(item.ToString());
            }

            Debug.Print("#######################");
            Debug.Print("# MATRIX 1 * VECTOR 3 #");
            Debug.Print("#######################");
            foreach (var item in vmult.vec)
            {
                Debug.Print(item.ToString());
            }

            Debug.Print("\n\nEND UNIT TESTS");
        }
    }
}
