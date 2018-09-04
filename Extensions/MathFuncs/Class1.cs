using System;
using System.Runtime.CompilerServices;
using Microsoft.SPOT;

namespace CMSIS
{
    public static class GlobalVar
    {
        // Q15 conversion factor
        public const float largeFactor = 20.0f;
    }

    public struct Matrix
    {
        public UInt16 rows;
        public UInt16 cols;
        public float[] data;

        public Matrix(UInt16 rows, UInt16 cols, float[] data)
        {
            this.rows = rows;
            this.cols = cols;
            this.data = data;
        }
    };

    public class Stats
    {
        // Max of vector
        public static float Max(float[] invec, out uint outIndex)
        {
            int[] indata = Support.ScaleConvertFloatArrToQ15(invec, GlobalVar.largeFactor);

            int outVal = Max_Nat(indata, out outIndex);
            return Support.ScaleConvertQ15ToFloat(outVal, GlobalVar.largeFactor);
        }

        // Min of vector
        public static float Min(float[] invec, out uint outIndex)
        {
            int[] indata = Support.ScaleConvertFloatArrToQ15(invec, GlobalVar.largeFactor);

            int outVal = Min_Nat(indata, out outIndex);
            return Support.ScaleConvertQ15ToFloat(outVal, GlobalVar.largeFactor);
        }

        // Mean of vector
        public static float Mean(float[] invec)
        {
            int[] indata = Support.ScaleConvertFloatArrToQ15(invec, GlobalVar.largeFactor);

            int outVal = Mean_Nat(indata);
            return Support.ScaleConvertQ15ToFloat(outVal, GlobalVar.largeFactor);
        }

        // Standard deviation of vector
        public static float StD(float[] invec)
        {
            int[] indata = Support.ScaleConvertFloatArrToQ15(invec, GlobalVar.largeFactor);

            int outVal = StD_Nat(indata);
            return Support.ScaleConvertQ15ToFloat(outVal, GlobalVar.largeFactor);
        }

        // Variance of vector
        public static float Var(float[] invec)
        {
            int[] indata = Support.ScaleConvertFloatArrToQ15(invec, GlobalVar.largeFactor);

            int outVal = Var_Nat(indata);
            return Support.ScaleConvertQ15ToFloat(outVal, GlobalVar.largeFactor * GlobalVar.largeFactor);
        }


        // Natives
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static int Max_Nat(int[] vec, out uint maxIndex);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static int Min_Nat(int[] vec, out uint minIndex);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static int Mean_Nat(int[] vec);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static int StD_Nat(int[] vec);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static int Var_Nat(int[] vec);
    }

    public class Support
    {
        // Downscale any float; convert to Q15
        public static int ScaleConvertFloatToQ15(float inx, float scaleFactor)
        {
            return ConvertFloatToQ15(inx / scaleFactor);
        }

        // Downscale any float array; convert to Q15
        public static int[] ScaleConvertFloatArrToQ15(float[] inx, float scaleFactor)
        {
            float[] scaledx = new float[inx.Length];
            for (int i = 0; i < inx.Length; i++)
            {
                scaledx[i] = inx[i] / scaleFactor;
            }

            int[] outx = new int[scaledx.Length];
            ConvertFloatToQ15(scaledx, outx);
            return outx;
        }

        // Convert Q15 to float; upscale
        public static float ScaleConvertQ15ToFloat(int inx, float scaleFactor)
        {
            return ConvertQ15ToFloat(inx) * scaleFactor;
        }

        // Convert Q15 array to float; upscale
        public static float[] ScaleConvertQ15ArrToFloat(int[] inx, float scaleFactor)
        {
            float[] outx = new float[inx.Length];
            ConvertQ15ToFloat(inx, outx);

            for (int i = 0; i < outx.Length; i++)
            {
                outx[i] = outx[i] * scaleFactor;
            }
            
            return outx;
        }

        // Deep-copy one vector into another
        public static float[] VectorCopy(float[] invec)
        {
            int[] outvec = new int[invec.Length];
            VectorCopy_Nat(ScaleConvertFloatArrToQ15(invec, GlobalVar.largeFactor), outvec);
            return ScaleConvertQ15ArrToFloat(outvec, GlobalVar.largeFactor);
        }

        // Fill vector with scalar
        public static float[] VectorFill(int len, float val)
        {
            int[] outvec = new int[len];
            VectorFill_Nat(ScaleConvertFloatToQ15(val, GlobalVar.largeFactor), outvec);
            return ScaleConvertQ15ArrToFloat(outvec, GlobalVar.largeFactor);
        }


        // Natives
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static int ConvertFloatToQ15(float x);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static float ConvertQ15ToFloat(int x);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static void ConvertFloatToQ15(float[] inx, int[] outx);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static void ConvertQ15ToFloat(int[] inx, float[] outx);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static void VectorCopy_Nat(int[] inx, int[] outx);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static void VectorFill_Nat(int val, int[] outx);
    }

    public class MatrixFuncs
    {

        // Multiply two matrices
        public static Matrix MatrixMult(Matrix m1, Matrix m2)
        {
            int[] m1dat = Support.ScaleConvertFloatArrToQ15(m1.data, GlobalVar.largeFactor);
            int[] m2dat = Support.ScaleConvertFloatArrToQ15(m2.data, GlobalVar.largeFactor);
            Matrix outmat = new Matrix(0, 0, new float[m1.rows * m2.cols]);

            int[] outdata = new int[m1.rows * m2.cols];
            MatrixMult_Nat(m1.rows, m1.cols, m1dat, m2.rows, m2.cols, m2dat, out outmat.rows, out outmat.cols, outdata);
            outmat.data = Support.ScaleConvertQ15ArrToFloat(outdata, GlobalVar.largeFactor * GlobalVar.largeFactor);
            return outmat;
        }

        // Add two matrices
        public static Matrix MatrixAdd(Matrix m1, Matrix m2)
        {
            int[] m1dat = Support.ScaleConvertFloatArrToQ15(m1.data, GlobalVar.largeFactor);
            int[] m2dat = Support.ScaleConvertFloatArrToQ15(m2.data, GlobalVar.largeFactor);
            Matrix outmat = new Matrix(0, 0, new float[m1.rows * m2.cols]);

            int[] outdata = new int[m1.rows * m2.cols];
            MatrixAdd_Nat(m1.rows, m1.cols, m1dat, m2.rows, m2.cols, m2dat, out outmat.rows, out outmat.cols, outdata);
            outmat.data = Support.ScaleConvertQ15ArrToFloat(outdata, GlobalVar.largeFactor);
            return outmat;
        }

        // Subtract one matrix from another
        public static Matrix MatrixSub(Matrix m1, Matrix m2)
        {
            int[] m1dat = Support.ScaleConvertFloatArrToQ15(m1.data, GlobalVar.largeFactor);
            int[] m2dat = Support.ScaleConvertFloatArrToQ15(m2.data, GlobalVar.largeFactor);
            Matrix outmat = new Matrix(0, 0, new float[m1.rows * m2.cols]);

            int[] outdata = new int[m1.rows * m2.cols];
            MatrixSub_Nat(m1.rows, m1.cols, m1dat, m2.rows, m2.cols, m2dat, out outmat.rows, out outmat.cols, outdata);
            outmat.data = Support.ScaleConvertQ15ArrToFloat(outdata, GlobalVar.largeFactor);
            return outmat;
        }

        // Transpose a matrix
        public static Matrix MatrixTrans(Matrix m1)
        {
            int[] m1dat = Support.ScaleConvertFloatArrToQ15(m1.data, GlobalVar.largeFactor);
            Matrix outmat = new Matrix(0, 0, new float[m1.rows * m1.cols]);

            int[] outdata = new int[m1.rows * m1.cols];
            MatrixTrans_Nat(m1.rows, m1.cols, m1dat, out outmat.rows, out outmat.cols, outdata);
            outmat.data = Support.ScaleConvertQ15ArrToFloat(outdata, GlobalVar.largeFactor);
            return outmat;
        }


        // Natives
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static void MatrixSub_Nat(UInt16 m1_rows, UInt16 m1_cols, int[] m1_data,
            UInt16 m2_rows, UInt16 m2_cols, int[] m2_data,
            out UInt16 outrows, out UInt16 outcols, int[] outdata);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static void MatrixAdd_Nat(UInt16 m1_rows, UInt16 m1_cols, int[] m1_data,
            UInt16 m2_rows, UInt16 m2_cols, int[] m2_data,
            out UInt16 outrows, out UInt16 outcols, int[] outdata);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static void MatrixMult_Nat(UInt16 m1_rows, UInt16 m1_cols, int[] m1_data,
            UInt16 m2_rows, UInt16 m2_cols, int[] m2_data,
            out UInt16 outrows, out UInt16 outcols, int[] outdata);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static void MatrixTrans_Nat(UInt16 m1_rows, UInt16 m1_cols, int[] m1_data,
            out UInt16 outrows, out UInt16 outcols, int[] outdata);
    }

    public class VectorFuncs
    {
        // Find absolute of vector
        public static float[] VectorAbs(float[] invec)
        {
            int[] indata = Support.ScaleConvertFloatArrToQ15(invec, GlobalVar.largeFactor);
            int[] outdata = new int[invec.Length];

            VectorAbs_Nat(indata, outdata);
            return Support.ScaleConvertQ15ArrToFloat(outdata, GlobalVar.largeFactor);
        }

        // Negate Vector
        public static float[] VectorNegate(float[] invec)
        {
            int[] indata = Support.ScaleConvertFloatArrToQ15(invec, GlobalVar.largeFactor);
            int[] outdata = new int[invec.Length];

            VectorNegate_Nat(indata, outdata);
            return Support.ScaleConvertQ15ArrToFloat(outdata, GlobalVar.largeFactor);
        }

        // Add an offset to vector
        public static float[] VectorOffset(float[] invec, float offset)
        {
            int[] indata = Support.ScaleConvertFloatArrToQ15(invec, GlobalVar.largeFactor);
            int[] outdata = new int[invec.Length];

            VectorOffset_Nat(indata, Support.ScaleConvertFloatToQ15(offset, GlobalVar.largeFactor), outdata);
            return Support.ScaleConvertQ15ArrToFloat(outdata, GlobalVar.largeFactor);
        }

        // Add two vectors
        public static float[] VectorAdd(float[] in1vec, float[] in2vec)
        {
            int[] in1data = Support.ScaleConvertFloatArrToQ15(in1vec, GlobalVar.largeFactor);
            int[] in2data = Support.ScaleConvertFloatArrToQ15(in2vec, GlobalVar.largeFactor);
            int[] outdata = new int[in1vec.Length];

            VectorAdd_Nat(in1data, in2data, outdata);
            return Support.ScaleConvertQ15ArrToFloat(outdata, GlobalVar.largeFactor);
        }

        // Subtract one vector from another
        public static float[] VectorSub(float[] in1vec, float[] in2vec)
        {
            int[] in1data = Support.ScaleConvertFloatArrToQ15(in1vec, GlobalVar.largeFactor);
            int[] in2data = Support.ScaleConvertFloatArrToQ15(in2vec, GlobalVar.largeFactor);
            int[] outdata = new int[in1vec.Length];

            VectorSub_Nat(in1data, in2data, outdata);
            return Support.ScaleConvertQ15ArrToFloat(outdata, GlobalVar.largeFactor);
        }

        // Dot product of two vectors
        public static float VectorDot(float[] in1vec, float[] in2vec)
        {
            int[] in1data = Support.ScaleConvertFloatArrToQ15(in1vec, GlobalVar.largeFactor);
            int[] in2data = Support.ScaleConvertFloatArrToQ15(in2vec, GlobalVar.largeFactor);
            
            int outdata = VectorDot_Nat(in1data, in2data);
            return Support.ScaleConvertQ15ToFloat(outdata, GlobalVar.largeFactor * GlobalVar.largeFactor);
        }

        // Hadamard product of two vectors
        public static float[] VectorHadamard(float[] in1vec, float[] in2vec)
        {
            int[] in1data = Support.ScaleConvertFloatArrToQ15(in1vec, GlobalVar.largeFactor);
            int[] in2data = Support.ScaleConvertFloatArrToQ15(in2vec, GlobalVar.largeFactor);
            int[] outdata = new int[in1vec.Length];

            VectorHadamard_Nat(in1data, in2data, outdata);
            return Support.ScaleConvertQ15ArrToFloat(outdata, GlobalVar.largeFactor * GlobalVar.largeFactor);
        }


        // Natives
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static void VectorAbs_Nat(int[] intx, int[] outx);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static void VectorNegate_Nat(int[] intx, int[] outx);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static void VectorOffset_Nat(int[] intx, int offset, int[] outx);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static void VectorAdd_Nat(int[] intx, int[] inty, int[] outx);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static void VectorSub_Nat(int[] intx, int[] inty, int[] outx);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static int VectorDot_Nat(int[] intx, int[] inty);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static void VectorHadamard_Nat(int[] intx, int[] inty, int[] outx);
    }
}
