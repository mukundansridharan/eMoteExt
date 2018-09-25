using System;
using System.Runtime.CompilerServices;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using Samraksh.eMote.DotNow;

namespace CMSIS
{
    public static class GlobalVar
    {
        // Q15 conversion factor
        public const float largeFactor = 20.0f;
    }

    public class Matrix
    {
        internal UInt16 rows;
        internal UInt16 cols;
        internal float[] data;
        internal int[] qdata;

        // Test if matrix contains float or Q data
        public bool IsFloat()
        {
            return (this.qdata == null && this.data != null);
        }

        public bool IsQ()
        {
            return (this.data == null && this.qdata != null);
        }

        // Get #rows
        public UInt16 GetRows()
        {
            return rows;
        }

        // Get #cols
        public UInt16 GetCols()
        {
            return cols;
        }

        // Get data
        public float[] GetData()
        {
            if (IsFloat())
                return data;
            else
            {
                float[] outdat = new float[qdata.Length];
                for (int i = 0; i < outdat.Length; i++)
                    outdat[i] = qdata[i];
                return outdat;
            }
        }

        // Initialize matrix with float array
        public Matrix(UInt16 rows, UInt16 cols, float[] data)
        {
            this.rows = rows;
            this.cols = cols;
            this.data = data;
            this.qdata = null; // Set Q array to null
        }

        // Initialize matrix with Q array
        public Matrix(UInt16 rows, UInt16 cols, int[] data)
        {
            this.rows = rows;
            this.cols = cols;
            this.data = null; // Set float array to null
            this.qdata = data;
        }

        // Overload operator + (m1 + m2)
        public static Matrix operator +(Matrix m1, Matrix m2)
        {
            return MatrixFuncs.MatrixAdd(m1, m2);
        }

        // Overload operator - (m1 - m2)
        public static Matrix operator -(Matrix m1, Matrix m2)
        {
            return MatrixFuncs.MatrixSub(m1, m2);
        }

        // Overload operator * (m1 * m2)
        public static Matrix operator *(Matrix m1, Matrix m2)
        {
            return MatrixFuncs.MatrixMult(m1, m2);
        }

        // Overload operator * (m1 * v1)
        public static Vector operator *(Matrix m1, Vector v1)
        {
            if (m1.IsFloat() && v1.IsFloat())
            {
                Matrix m2 = new Matrix((ushort)v1.data.Length, 1, v1.data);
                return new Vector(MatrixFuncs.MatrixMult(m1, m2).data);
            }
            else if (m1.IsQ() && v1.IsQ())
            {
                Matrix m2 = new Matrix((ushort)v1.qdata.Length, 1, v1.qdata);
                return new Vector(MatrixFuncs.MatrixMult(m1, m2).qdata);
            }
            else throw new NotImplementedException();
        }

        // Transpose
        public Matrix Transpose()
        {
            return MatrixFuncs.MatrixTrans(this);
        }
    }

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

        // Copy one vector into another
        public static float[] VectorCopy(float[] invec, int index, uint length)
        {
            int[] outvec = new int[length];
            VectorCopy_Nat(ScaleConvertFloatArrToQ15(invec, GlobalVar.largeFactor), index, outvec, length);
            return ScaleConvertQ15ArrToFloat(outvec, GlobalVar.largeFactor);
        }

        public static int[] VectorCopy(int[] invec, int index, uint length)
        {
            int[] outvec = new int[length];
            VectorCopy_Nat(invec, index, outvec, length);
            return outvec;
        }

        // Fill vector with scalar
        public static float[] VectorFill(int len, float val)
        {
            int[] outvec = new int[len];
            VectorFill_Nat(ScaleConvertFloatToQ15(val, GlobalVar.largeFactor), outvec);
            return ScaleConvertQ15ArrToFloat(outvec, GlobalVar.largeFactor);
        }

        public static int[] VectorFill(int len, int val)
        {
            int[] outvec = new int[len];
            VectorFill_Nat(val, outvec);
            return outvec;
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
        extern static void VectorCopy_Nat(int[] inx, int index, int[] outx, uint length);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static void VectorFill_Nat(int val, int[] outx);
    }

    public class MatrixFuncs
    {
#if LOGIC
        private static OutputPort mat_mult = new OutputPort(Pins.GPIO_J12_PIN1, false);
        private static OutputPort mat_add = new OutputPort(Pins.GPIO_J12_PIN2, false);
        private static OutputPort mat_trans = new OutputPort(Pins.GPIO_J12_PIN3, false);
#endif

        // Multiply two matrices
        public static Matrix MatrixMult(Matrix m1, Matrix m2)
        {
            int[] m1dat, m2dat;
            Matrix outmat;

            if (m1.IsFloat() && m2.IsFloat())
            {
                m1dat = Support.ScaleConvertFloatArrToQ15(m1.data, GlobalVar.largeFactor);
                m2dat = Support.ScaleConvertFloatArrToQ15(m2.data, GlobalVar.largeFactor);
                outmat = new Matrix(0, 0, new float[m1.rows * m2.cols]);
            }
            else if (m1.IsQ() && m2.IsQ())
            {
                m1dat = m1.qdata;
                m2dat = m2.qdata;
                outmat = new Matrix(0, 0, new int[m1.rows * m2.cols]);
            }
            else throw new NotSupportedException();

            int[] outdata = new int[m1.rows * m2.cols];
#if LOGIC
            mat_mult.Write(true);
#endif
            MatrixMult_Nat(m1.rows, m1.cols, m1dat, m2.rows, m2.cols, m2dat, out outmat.rows, out outmat.cols, outdata);
#if LOGIC
            mat_mult.Write(false);
#endif
            if (m1.IsFloat() && m2.IsFloat())
                outmat.data = Support.ScaleConvertQ15ArrToFloat(outdata, GlobalVar.largeFactor * GlobalVar.largeFactor);
            else if (m1.IsQ() && m2.IsQ())
                outmat.qdata = outdata;

            return outmat;
        }

        // Add two matrices
        public static Matrix MatrixAdd(Matrix m1, Matrix m2)
        {
            int[] m1dat, m2dat;
            Matrix outmat;

            if (m1.IsFloat() && m2.IsFloat())
            {
                m1dat = Support.ScaleConvertFloatArrToQ15(m1.data, GlobalVar.largeFactor);
                m2dat = Support.ScaleConvertFloatArrToQ15(m2.data, GlobalVar.largeFactor);
                outmat = new Matrix(0, 0, new float[m1.rows * m2.cols]);
            }
            else if (m1.IsQ() && m2.IsQ())
            {
                m1dat = m1.qdata;
                m2dat = m2.qdata;
                outmat = new Matrix(0, 0, new int[m1.rows * m2.cols]);
            }
            else throw new NotSupportedException();

            int[] outdata = new int[m1.rows * m2.cols];
#if LOGIC
            mat_add.Write(true);
#endif
            MatrixAdd_Nat(m1.rows, m1.cols, m1dat, m2.rows, m2.cols, m2dat, out outmat.rows, out outmat.cols, outdata);
#if LOGIC
            mat_add.Write(false);
#endif

            if (m1.IsFloat() && m2.IsFloat())
                outmat.data = Support.ScaleConvertQ15ArrToFloat(outdata, GlobalVar.largeFactor);
            else if (m1.IsQ() && m2.IsQ())
                outmat.qdata = outdata;
            return outmat;
        }

        // Subtract one matrix from another
        public static Matrix MatrixSub(Matrix m1, Matrix m2)
        {
            int[] m1dat, m2dat;
            Matrix outmat;

            if (m1.IsFloat() && m2.IsFloat())
            {
                m1dat = Support.ScaleConvertFloatArrToQ15(m1.data, GlobalVar.largeFactor);
                m2dat = Support.ScaleConvertFloatArrToQ15(m2.data, GlobalVar.largeFactor);
                outmat = new Matrix(0, 0, new float[m1.rows * m2.cols]);
            }
            else if (m1.IsQ() && m2.IsQ())
            {
                m1dat = m1.qdata;
                m2dat = m2.qdata;
                outmat = new Matrix(0, 0, new int[m1.rows * m2.cols]);
            }
            else throw new NotSupportedException();

            int[] outdata = new int[m1.rows * m2.cols];

            MatrixSub_Nat(m1.rows, m1.cols, m1dat, m2.rows, m2.cols, m2dat, out outmat.rows, out outmat.cols, outdata);

            if (m1.IsFloat() && m2.IsFloat())
                outmat.data = Support.ScaleConvertQ15ArrToFloat(outdata, GlobalVar.largeFactor);
            else if (m1.IsQ() && m2.IsQ())
                outmat.qdata = outdata;
            return outmat;
        }

        // Transpose a matrix
        public static Matrix MatrixTrans(Matrix m1)
        {
            int[] m1dat;
            Matrix outmat;

            if (m1.IsFloat())
            { 
                m1dat = Support.ScaleConvertFloatArrToQ15(m1.data, GlobalVar.largeFactor);
                outmat = new Matrix(0, 0, new float[m1.rows * m1.cols]);
            }
            else if (m1.IsQ())
            {
                m1dat = m1.qdata;
                outmat = new Matrix(0, 0, new int[m1.rows * m1.cols]);
            }
            else throw new NotSupportedException();

            int[] outdata = new int[m1.rows * m1.cols];
#if LOGIC
            mat_trans.Write(true);
#endif
            MatrixTrans_Nat(m1.rows, m1.cols, m1dat, out outmat.rows, out outmat.cols, outdata);
#if LOGIC
            mat_trans.Write(false);
#endif
            if (m1.IsFloat())
                outmat.data = Support.ScaleConvertQ15ArrToFloat(outdata, GlobalVar.largeFactor);
            else if (m1.IsQ())
                outmat.qdata = outdata;
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

    public class Vector
    {
        internal float[] data;
        internal int[] qdata;

        // Test if vector is float or Q
        public bool IsFloat()
        {
            return (qdata == null && data != null);
        }

        public bool IsQ()
        {
            return (data == null && qdata != null);
        }

        // Get data
        public float[] GetData()
        {
            if (IsFloat())
                return data;
            else
            {
                float[] outdat = new float[qdata.Length];
                for (int i = 0; i < outdat.Length; i++)
                    outdat[i] = qdata[i];
                return outdat;
            }
        }

       // Initialize vector with float array
        public Vector(float[] v)
        {
            data = v;
            qdata = null; // Set Q array to null
        }

        // Initialize vector with Q array
        public Vector(int[] v)
        {
            qdata = v;
            data = null; // Set float array to null
        }

        // Overload operator + (v1 + v2)
        public static Vector operator +(Vector v1, Vector v2)
        {

            return VectorFuncs.VectorAdd(v1, v2);
        }

        // Overload operator + (v1 + s1) (Add offset)
        public static Vector operator +(Vector v1, float s1)
        {
            return VectorFuncs.VectorOffset(v1, s1);
        }

        // Overload operator + (s1 + v1) (Add offset)
        public static Vector operator +(float s1, Vector v1)
        {
            return VectorFuncs.VectorOffset(v1, s1);
        }

        // Overload operator - (-v1)
        public static Vector operator -(Vector v1)
        {
            return VectorFuncs.VectorNegate(v1);
        }

        // Overload operator - (v1 - v2)
        public static Vector operator -(Vector v1, Vector v2)
        {
            return VectorFuncs.VectorSub(v1, v2);
        }

        // Overload operator - (v1 - s1) (Subtract offset)
        public static Vector operator -(Vector v1, float s1)
        {
            return VectorFuncs.VectorOffset(v1, -s1);
        }

        // Overload operator - (s1 - v1) (Subtract from scalar)
        public static Vector operator -(float s1, Vector v1)
        {
            return VectorFuncs.VectorOffset((-v1), s1);
        }

        // Oveload operator * (s1 * v1)
        public static Vector operator *(float s1, Vector v1)
        {
            return VectorFuncs.VectorScale(v1, s1);
        }

        // Oveload operator * (v1 * s1)
        public static Vector operator *(Vector v1, float s1)
        {
            return VectorFuncs.VectorScale(v1, s1);
        }

        // Oveload operator / (s1 / v1)
        public static Vector operator /(float s1, Vector v1)
        {
            if (s1 == 1.0f)
                return VectorFuncs.VectorRecip(v1);
            else
                return VectorFuncs.VectorScale(VectorFuncs.VectorRecip(v1), s1);
        }

        // Oveload operator / (v1 / s1)
        public static Vector operator /(Vector v1, float s1)
        {
            if (s1 == 1.0f)
                return v1;
            else
                return VectorFuncs.VectorScale(VectorFuncs.VectorRecip(v1), 1.0f / s1);
        }

        // Build empty vector of given length
        public static Vector BuildDense(int size)
        {
            return new Vector(Support.VectorFill(size, 0.0f));
        }

        // Point-wise tanh of vector
        public Vector PointwiseTanh()
        {
            return VectorFuncs.VectorTanh(this);
        }

        // Point-wise exponentiation of vector
        public Vector PointwiseExp()
        {
            return VectorFuncs.VectorExp(this);
        }

        // Point-wise multiplication of one vector with another
        public Vector PointwiseMultiply(Vector v)
        {
            return VectorFuncs.VectorHadamard(this, v);
        }

        public Vector SubVector(int index, uint count)
        {
            return new Vector(Support.VectorCopy(this.data, index, count));
        }
    }

    public class VectorFuncs
    {
#if LOGIC
        private static OutputPort vec_offset = new OutputPort(Pins.GPIO_J12_PIN4, false);
        private static OutputPort vec_scale = new OutputPort(Pins.GPIO_J12_PIN5, false);
        private static OutputPort vec_add = new OutputPort(Pins.GPIO_J11_PIN3, false);
        private static OutputPort vec_recip = new OutputPort(Pins.GPIO_J11_PIN4, false);
        private static OutputPort vec_had = new OutputPort(Pins.GPIO_J11_PIN5, false);
#endif

        // Find absolute of vector
        public static Vector VectorAbs(Vector invec)
        {
            int[] indata;
            int[] outdata;

            if (invec.IsFloat())
            {
                indata = Support.ScaleConvertFloatArrToQ15(invec.data, GlobalVar.largeFactor);
                outdata = new int[invec.data.Length];
            }
            else if (invec.IsQ())
            {
                indata = invec.qdata;
                outdata = new int[invec.qdata.Length];
            }
            else throw new NotImplementedException();

            VectorAbs_Nat(indata, outdata);

            if (invec.IsFloat())
                return new Vector(Support.ScaleConvertQ15ArrToFloat(outdata, GlobalVar.largeFactor));
            else
                return new Vector(outdata);
        }

        // Negate vector
        public static Vector VectorNegate(Vector invec)
        {
            int[] indata;
            int[] outdata;

            if (invec.IsFloat())
            {
                indata = Support.ScaleConvertFloatArrToQ15(invec.data, GlobalVar.largeFactor);
                outdata = new int[invec.data.Length];
            }
            else if (invec.IsQ())
            {
                indata = invec.qdata;
                outdata = new int[invec.qdata.Length];
            }
            else throw new NotImplementedException();

            VectorNegate_Nat(indata, outdata);

            if (invec.IsFloat())
                return new Vector(Support.ScaleConvertQ15ArrToFloat(outdata, GlobalVar.largeFactor));
            else
                return new Vector(outdata);
        }

        // Add an scalar to vector
        public static Vector VectorOffset(Vector invec, float offset)
        {
            int[] indata;
            int[] outdata;
            int offset_loc;

            if (invec.IsFloat())
            {
                indata = Support.ScaleConvertFloatArrToQ15(invec.data, GlobalVar.largeFactor);
                offset_loc = Support.ScaleConvertFloatToQ15(offset, GlobalVar.largeFactor);
                outdata = new int[invec.data.Length];
            }
            else if (invec.IsQ() && (int)offset==offset)
            {
                indata = invec.qdata;
                offset_loc = (int)offset;
                outdata = new int[invec.qdata.Length];
            }
            else throw new NotImplementedException();

#if LOGIC
            vec_offset.Write(true);
#endif
            VectorOffset_Nat(indata, offset_loc, outdata);
#if LOGIC
            vec_offset.Write(false);
#endif
            if (invec.IsFloat())
                return new Vector(Support.ScaleConvertQ15ArrToFloat(outdata, GlobalVar.largeFactor));
            else
                return new Vector(outdata);
        }

        // Scale vector by scalar
        public static Vector VectorScale(Vector invec, float scale)
        {
            int[] indata;
            int[] outdata;
            int scale_loc;

            if (invec.IsFloat())
            {
                indata = Support.ScaleConvertFloatArrToQ15(invec.data, GlobalVar.largeFactor);
                scale_loc = Support.ScaleConvertFloatToQ15(scale, GlobalVar.largeFactor);
                outdata = new int[invec.data.Length];
            }
            else if (invec.IsQ() && (int)scale == scale)
            {
                indata = invec.qdata;
                scale_loc = (int)scale;
                outdata = new int[invec.qdata.Length];
            }
            else throw new NotImplementedException();
#if LOGIC
            vec_scale.Write(true);
#endif
            VectorScale_Nat(indata, scale_loc, outdata);
#if LOGIC
            vec_scale.Write(false);
#endif
            if (invec.IsFloat())
                return new Vector(Support.ScaleConvertQ15ArrToFloat(outdata, GlobalVar.largeFactor));
            else
                return new Vector(outdata);
        }

        // Add two vectors
        public static Vector VectorAdd(Vector in1vec, Vector in2vec)
        {
            int[] in1data, in2data, outdata;

            if (in1vec.IsFloat() && in2vec.IsFloat())
            {
                in1data = Support.ScaleConvertFloatArrToQ15(in1vec.data, GlobalVar.largeFactor);
                in2data = Support.ScaleConvertFloatArrToQ15(in2vec.data, GlobalVar.largeFactor);
                outdata = new int[in1vec.data.Length];
            }
            else if (in1vec.IsQ() && in2vec.IsQ())
            {
                in1data = in1vec.qdata;
                in2data = in2vec.qdata;
                outdata = new int[in1vec.qdata.Length];
            }
            else throw new NotImplementedException();
#if LOGIC
            vec_add.Write(true);
#endif
            VectorAdd_Nat(in1data, in2data, outdata);
#if LOGIC
            vec_add.Write(false);
#endif
            if (in1vec.IsFloat() && in2vec.IsFloat())
                return new Vector(Support.ScaleConvertQ15ArrToFloat(outdata, GlobalVar.largeFactor));
            else
                return new Vector(outdata);
        }

        // Subtract one vector from another
        public static Vector VectorSub(Vector in1vec, Vector in2vec)
        {
            int[] in1data, in2data, outdata;

            if (in1vec.IsFloat() && in2vec.IsFloat())
            {
                in1data = Support.ScaleConvertFloatArrToQ15(in1vec.data, GlobalVar.largeFactor);
                in2data = Support.ScaleConvertFloatArrToQ15(in2vec.data, GlobalVar.largeFactor);
                outdata = new int[in1vec.data.Length];
            }
            else if (in1vec.IsQ() && in2vec.IsQ())
            {
                in1data = in1vec.qdata;
                in2data = in2vec.qdata;
                outdata = new int[in1vec.qdata.Length];
            }
            else throw new NotImplementedException();

            VectorSub_Nat(in1data, in2data, outdata);

            if (in1vec.IsFloat() && in2vec.IsFloat())
                return new Vector(Support.ScaleConvertQ15ArrToFloat(outdata, GlobalVar.largeFactor));
            else
                return new Vector(outdata);
        }

        // Reciprocal of vector - float implementation
        public static Vector VectorRecip(Vector in1vec)
        {
            // VectorRecip_Nat doesn't work: don't know how to interpret result
            // (https://voltampmedia.com/2011/09/27/using-arms-cmsis-dsp-library-arm_recip_q15/)
            /*int[] in1data = Support.ScaleConvertFloatArrToQ15(in1vec, GlobalVar.largeFactor);
            int[] outdata = new int[in1vec.Length];

            VectorRecip_Nat(in1data, outdata);
            return Support.ScaleConvertQ15ArrToFloat(outdata, 1.0f/GlobalVar.largeFactor);*/
#if LOGIC
            vec_recip.Write(true);
#endif
            // Simply compute reciprocal. TODO: Change to faster implementation
            float[] outdata = new float[in1vec.data.Length];
            for (int i = 0; i < in1vec.data.Length; i++)
                outdata[i] = 1.0f / in1vec.data[i];
#if LOGIC
            vec_recip.Write(false);
#endif
            return new Vector(outdata);
        }

        // Dot product of two vectors
        public static float VectorDot(Vector in1vec, Vector in2vec)
        {
            int[] in1data, in2data;

            if (in1vec.IsFloat() && in2vec.IsFloat())
            {
                in1data = Support.ScaleConvertFloatArrToQ15(in1vec.data, GlobalVar.largeFactor);
                in2data = Support.ScaleConvertFloatArrToQ15(in2vec.data, GlobalVar.largeFactor);
            }
            else if (in1vec.IsQ() && in2vec.IsQ())
            {
                in1data = in1vec.qdata;
                in2data = in2vec.qdata;
            }
            else throw new NotImplementedException();

            int outdata = VectorDot_Nat(in1data, in2data);

            if (in1vec.IsFloat() && in2vec.IsFloat())
                return Support.ScaleConvertQ15ToFloat(outdata, GlobalVar.largeFactor * GlobalVar.largeFactor);
            else
                return outdata;
        }

        // Hadamard product of two vectors
        public static Vector VectorHadamard(Vector in1vec, Vector in2vec)
        {
            int[] in1data, in2data, outdata;

            if (in1vec.IsFloat() && in2vec.IsFloat())
            {
                in1data = Support.ScaleConvertFloatArrToQ15(in1vec.data, GlobalVar.largeFactor);
                in2data = Support.ScaleConvertFloatArrToQ15(in2vec.data, GlobalVar.largeFactor);
                outdata = new int[in1vec.data.Length];
            }
            else if (in1vec.IsQ() && in2vec.IsQ())
            {
                in1data = in1vec.qdata;
                in2data = in2vec.qdata;
                outdata = new int[in1vec.qdata.Length];
            }
            else throw new NotImplementedException();
#if LOGIC
            vec_had.Write(true);
#endif
            VectorHadamard_Nat(in1data, in2data, outdata);
#if LOGIC
            vec_had.Write(false);
#endif
            if (in1vec.IsFloat() && in2vec.IsFloat())
                return new Vector(Support.ScaleConvertQ15ArrToFloat(outdata, GlobalVar.largeFactor * GlobalVar.largeFactor));
            else
                return new Vector(outdata);
        }

        // Tanh of vector: double function
        /*
         TODO: Simplifying formula: tanh(x) = x if |x| < 1; sign(x) otherwise
         */
        public static Vector VectorTanh(Vector invec)
        {
            float[] outvec = new float[invec.data.Length];

            for (int i = 0; i < invec.data.Length; i++)
            {
                /*if (invec[i] < 1.0f && invec[i] > -1.0f)
                    outvec[i] = invec[i];
                else if (invec[i] > 1)
                    outvec[i] = 1;
                else if (invec[i] < -1)
                    outvec[i] = -1;*/
                outvec[i] = (float)System.Math.Tanh(invec.data[i]);
            }

            return new Vector(outvec);
        }

        // Exponentiation of vector: double function
        public static Vector VectorExp(Vector invec)
        {
            float[] outvec = new float[invec.data.Length];

            for (int i = 0; i < invec.data.Length; i++)
                outvec[i] = (float) System.Math.Exp(invec.data[i]);

            return new Vector(outvec);
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

        // Doesn't work: don't know how to interpret result (https://voltampmedia.com/2011/09/27/using-arms-cmsis-dsp-library-arm_recip_q15/)
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static void VectorRecip_Nat(int[] intx, int[] outx);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static void VectorScale_Nat(int[] intx, int scale, int[] outx);
    }
}
