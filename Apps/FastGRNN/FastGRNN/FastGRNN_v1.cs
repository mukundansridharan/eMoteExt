using System;
using CMSIS;

namespace FastGRNN
{
    class fastgrnn_v1
    {
        private static Matrix weight_ih, weight_hh;
        private static Vector hidden, bias_z, bias_h;
        private static int i_size, h_size;
        private static double zeta, nu;
        private static float Zeta, Nu;

        private static void fastgrnncell(Vector input)
        {
            Vector z = (weight_ih.Transpose() * input + weight_hh.Transpose() * hidden + bias_z);
            z = 1 / (1 + (-z).PointwiseExp());
            Vector h = (weight_ih.Transpose() * input + weight_hh.Transpose() * hidden + bias_h);
            h = h.PointwiseTanh();
            hidden = (Zeta * (1 - z) + Nu).PointwiseMultiply(h) + z.PointwiseMultiply(hidden);
        }

        public fastgrnn_v1(Matrix wih, Matrix whh, Vector bz, Vector bh, float zt, float n)
        {
            weight_ih = wih; weight_hh = whh;
            i_size = weight_ih.rows; h_size = weight_ih.cols;
            bias_z = bz; bias_h = bh;
            zeta = zt; nu = n;

            // Precomputing
            Zeta = (float)(1 / (1 + Math.Exp(-zeta)));
            Nu = (float)(1 / (1 + Math.Exp(-nu)));
        }

        public Vector fastgrnn_calculations(Vector input, uint window, int stride)
        {
            hidden = Vector.BuildDense(h_size);
            int cut_length = input.vec.Length;
            int time_steps = (int)(Math.Ceiling((double)(cut_length - window) / stride));
            Microsoft.SPOT.Debug.Print("TNN time steps: " + time_steps);
            //int time_steps = (int)(Math.Floor((double)(cut_length - window + stride)/stride));
            for (int i = 0; i < time_steps; i++)
            {
                fastgrnncell(input.SubVector(i * stride, window));
            }

            return hidden;
        }
    }
}
