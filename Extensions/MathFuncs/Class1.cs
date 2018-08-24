using System;
using System.Runtime.CompilerServices;
using Microsoft.SPOT;

namespace CMSIS
{
    public struct Matrix
    {
        public UInt16 rows;
        public UInt16 cols;
        public int[] data;

        public Matrix(UInt16 rows, UInt16 cols, int[] data)
        {
            this.rows = rows;
            this.cols = cols;
            this.data = data;
        }
    };

    public class Support
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static int ConvertFloatToInt(float x);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static float ConvertIntToFloat(int x);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void ConvertFloatToInt(float[] inx, int[] outx);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void ConvertIntToFloat(int[] inx, float[] outx);
    }

    public class MatrixFuncs
    {
        public static void MatrixMult(Matrix m1, Matrix m2, out Matrix outmat)
        {
            int[] outdata = new int[m1.rows * m2.cols];
            MatrixMult_Nat(m1.rows, m1.cols, m1.data, m2.rows, m2.cols, m2.data, out outmat.rows, out outmat.cols, outdata);
            outmat.data = outdata;
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static void MatrixMult_Nat(UInt16 m1_rows, UInt16 m1_cols, int[] m1_data,
            UInt16 m2_rows, UInt16 m2_cols, int[] m2_data,
            out UInt16 outrows, out UInt16 outcols, int[] outdata);
    }
}
