using System;
using System.Runtime.CompilerServices;
using Microsoft.SPOT;

namespace CMSIS
{
    public static class GlobalVar
    {
        // Q31 conversion factor
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

    public class Support
    {
        // Downscale any float; convert to Q31
        public static int ScaleConvertFloatToQ31(float inx, float scaleFactor)
        {
            return ConvertFloatToQ31(inx / scaleFactor);
        }

        // Downscale any float array; convert to Q31
        public static int[] ScaleConvertFloatArrToQ31(float[] inx, float scaleFactor)
        {
            float[] scaledx = new float[inx.Length];
            for (int i = 0; i < inx.Length; i++)
            {
                scaledx[i] = inx[i] / scaleFactor;
            }

            int[] outx = new int[scaledx.Length];
            ConvertFloatToQ31(scaledx, outx);
            return outx;
        }

        // Convert Q31 array to float; upscale
        public static float ScaleConvertQ31ToFloat(int inx, float scaleFactor)
        {
            return ConvertQ31ToFloat(inx) * scaleFactor;
        }

        // Convert Q31 array to float; upscale
        public static float[] ScaleConvertQ31ArrToFloat(int[] inx, float scaleFactor)
        {
            float[] outx = new float[inx.Length];
            ConvertQ31ToFloat(inx, outx);

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
            VectorCopy_Nat(ScaleConvertFloatArrToQ31(invec, GlobalVar.largeFactor), outvec);
            return ScaleConvertQ31ArrToFloat(outvec, GlobalVar.largeFactor);
        }

        // Fill vector with scalar
        public static float[] VectorFill(int len, float val)
        {
            int[] outvec = new int[len];
            VectorFill_Nat(ScaleConvertFloatToQ31(val, GlobalVar.largeFactor), outvec);
            return ScaleConvertQ31ArrToFloat(outvec, GlobalVar.largeFactor);
        }


        // Natives
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static int ConvertFloatToQ31(float x);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static float ConvertQ31ToFloat(int x);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void ConvertFloatToQ31(float[] inx, int[] outx);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void ConvertQ31ToFloat(int[] inx, float[] outx);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void VectorCopy_Nat(int[] inx, int[] outx);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void VectorFill_Nat(int val, int[] outx);
    }

    public class MatrixFuncs
    {

        // Multiply two matrices
        public static Matrix MatrixMult(Matrix m1, Matrix m2)
        {
            int[] m1dat = Support.ScaleConvertFloatArrToQ31(m1.data, GlobalVar.largeFactor);
            int[] m2dat = Support.ScaleConvertFloatArrToQ31(m2.data, GlobalVar.largeFactor);
            Matrix outmat = new Matrix(0, 0, new float[m1.rows * m2.cols]);

            int[] outdata = new int[m1.rows * m2.cols];
            MatrixMult_Nat(m1.rows, m1.cols, m1dat, m2.rows, m2.cols, m2dat, out outmat.rows, out outmat.cols, outdata);
            outmat.data = Support.ScaleConvertQ31ArrToFloat(outdata, GlobalVar.largeFactor * GlobalVar.largeFactor);
            return outmat;
        }

        // Add two matrices
        public static Matrix MatrixAdd(Matrix m1, Matrix m2)
        {
            int[] m1dat = Support.ScaleConvertFloatArrToQ31(m1.data, GlobalVar.largeFactor);
            int[] m2dat = Support.ScaleConvertFloatArrToQ31(m2.data, GlobalVar.largeFactor);
            Matrix outmat = new Matrix(0, 0, new float[m1.rows * m2.cols]);

            int[] outdata = new int[m1.rows * m2.cols];
            MatrixAdd_Nat(m1.rows, m1.cols, m1dat, m2.rows, m2.cols, m2dat, out outmat.rows, out outmat.cols, outdata);
            outmat.data = Support.ScaleConvertQ31ArrToFloat(outdata, GlobalVar.largeFactor);
            return outmat;
        }

        // Subtract one matrix from another
        public static Matrix MatrixSub(Matrix m1, Matrix m2)
        {
            int[] m1dat = Support.ScaleConvertFloatArrToQ31(m1.data, GlobalVar.largeFactor);
            int[] m2dat = Support.ScaleConvertFloatArrToQ31(m2.data, GlobalVar.largeFactor);
            Matrix outmat = new Matrix(0, 0, new float[m1.rows * m2.cols]);

            int[] outdata = new int[m1.rows * m2.cols];
            MatrixSub_Nat(m1.rows, m1.cols, m1dat, m2.rows, m2.cols, m2dat, out outmat.rows, out outmat.cols, outdata);
            outmat.data = Support.ScaleConvertQ31ArrToFloat(outdata, GlobalVar.largeFactor);
            return outmat;
        }

        // Transpose a matrix
        public static Matrix MatrixTrans(Matrix m1)
        {
            int[] m1dat = Support.ScaleConvertFloatArrToQ31(m1.data, GlobalVar.largeFactor);
            Matrix outmat = new Matrix(0, 0, new float[m1.rows * m1.cols]);

            int[] outdata = new int[m1.rows * m1.cols];
            MatrixTrans_Nat(m1.rows, m1.cols, m1dat, out outmat.rows, out outmat.cols, outdata);
            outmat.data = Support.ScaleConvertQ31ArrToFloat(outdata, GlobalVar.largeFactor);
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
            int[] indata = Support.ScaleConvertFloatArrToQ31(invec, GlobalVar.largeFactor);
            int[] outdata = new int[invec.Length];

            VectorAbs_Nat(indata, outdata);
            return Support.ScaleConvertQ31ArrToFloat(outdata, GlobalVar.largeFactor);
        }

        // Negate Vector
        public static float[] VectorNegate(float[] invec)
        {
            int[] indata = Support.ScaleConvertFloatArrToQ31(invec, GlobalVar.largeFactor);
            int[] outdata = new int[invec.Length];

            VectorNegate_Nat(indata, outdata);
            return Support.ScaleConvertQ31ArrToFloat(outdata, GlobalVar.largeFactor);
        }

        // Add an offset to vector
        public static float[] VectorOffset(float[] invec, float offset)
        {
            int[] indata = Support.ScaleConvertFloatArrToQ31(invec, GlobalVar.largeFactor);
            int[] outdata = new int[invec.Length];

            VectorOffset_Nat(indata, Support.ScaleConvertFloatToQ31(offset, GlobalVar.largeFactor), outdata);
            return Support.ScaleConvertQ31ArrToFloat(outdata, GlobalVar.largeFactor);
        }

        // Add two vectors
        public static float[] VectorAdd(float[] in1vec, float[] in2vec)
        {
            int[] in1data = Support.ScaleConvertFloatArrToQ31(in1vec, GlobalVar.largeFactor);
            int[] in2data = Support.ScaleConvertFloatArrToQ31(in2vec, GlobalVar.largeFactor);
            int[] outdata = new int[in1vec.Length];

            VectorAdd_Nat(in1data, in2data, outdata);
            return Support.ScaleConvertQ31ArrToFloat(outdata, GlobalVar.largeFactor);
        }

        // Subtract one vector from another
        public static float[] VectorSub(float[] in1vec, float[] in2vec)
        {
            int[] in1data = Support.ScaleConvertFloatArrToQ31(in1vec, GlobalVar.largeFactor);
            int[] in2data = Support.ScaleConvertFloatArrToQ31(in2vec, GlobalVar.largeFactor);
            int[] outdata = new int[in1vec.Length];

            VectorSub_Nat(in1data, in2data, outdata);
            return Support.ScaleConvertQ31ArrToFloat(outdata, GlobalVar.largeFactor);
        }

        // Dot product of two vectors
        public static float VectorDot(float[] in1vec, float[] in2vec)
        {
            int[] in1data = Support.ScaleConvertFloatArrToQ31(in1vec, GlobalVar.largeFactor);
            int[] in2data = Support.ScaleConvertFloatArrToQ31(in2vec, GlobalVar.largeFactor);
            
            int outdata = VectorDot_Nat(in1data, in2data);
            return Support.ScaleConvertQ31ToFloat(outdata, GlobalVar.largeFactor);
        }

        // Hadamard product of two vectors
        public static float[] VectorHadamad(float[] in1vec, float[] in2vec)
        {
            int[] in1data = Support.ScaleConvertFloatArrToQ31(in1vec, GlobalVar.largeFactor);
            int[] in2data = Support.ScaleConvertFloatArrToQ31(in2vec, GlobalVar.largeFactor);
            int[] outdata = new int[in1vec.Length];

            VectorHadamard_Nat(in1data, in2data, outdata);
            return Support.ScaleConvertQ31ArrToFloat(outdata, GlobalVar.largeFactor);
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
