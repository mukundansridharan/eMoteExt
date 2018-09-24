using System;
using CMSIS;
using Microsoft.SPOT.Hardware;
using Samraksh.eMote.DotNow;

namespace FastGRNN
{
    class fastgrnn_v1
    {
        private static Matrix weight_ih, weight_hh,  weight_ih_transpose, weight_hh_transpose;
        private static Vector hidden, bias_z, bias_h;
        private static int i_size, h_size;
        private static double zeta, nu;
        private static float Zeta, Nu;

        // Timing analysis variables
#if LOGIC
        private static OutputPort cell = new OutputPort(Pins.GPIO_J12_PIN1, false);
        private static OutputPort ih_mult_input = new OutputPort(Pins.GPIO_J12_PIN2, false);
        private static OutputPort hh_mult_hidden = new OutputPort(Pins.GPIO_J12_PIN3, false);
        private static OutputPort logit = new OutputPort(Pins.GPIO_J12_PIN4, false);
        private static OutputPort tanh = new OutputPort(Pins.GPIO_J12_PIN5, false);
        private static OutputPort hidden_update = new OutputPort(Pins.GPIO_J11_PIN3, false);
        private static OutputPort vec_add = new OutputPort(Pins.GPIO_J11_PIN4, false);
        private static OutputPort input_strides = new OutputPort(Pins.GPIO_J11_PIN5, false);
#endif
        private static void fastgrnncell(Vector input)
        {
            // Cell gpio pin toggle high
#if LOGIC
            cell.Write(true);
#elif DEBUG
            long start_2 = DateTime.Now.Ticks;
#endif
#if LOGIC
            ih_mult_input.Write(true);
#endif
            Vector temp1 = weight_ih_transpose * input;
#if LOGIC
            ih_mult_input.Write(false);
#elif DEBUG
            long end_2 = DateTime.Now.Ticks;

            long start_1 = DateTime.Now.Ticks;
#endif
#if LOGIC
            hh_mult_hidden.Write(true);
#endif
            Vector temp2 = weight_hh_transpose * hidden;
#if LOGIC
            hh_mult_hidden.Write(false);
#elif DEBUG
            long end_1 = DateTime.Now.Ticks;

            long start0 = DateTime.Now.Ticks;
#endif
#if LOGIC
            vec_add.Write(true);
#endif
            Vector temp = temp1 + temp2;
#if LOGIC
            vec_add.Write(false);
#elif DEBUG
            long end0 = DateTime.Now.Ticks;

            long start1 = DateTime.Now.Ticks;
#endif
            Vector z = (temp + bias_z);
#if DEBUG
            long end1 = DateTime.Now.Ticks;

            long start2 = DateTime.Now.Ticks;
#endif
#if LOGIC
            logit.Write(true);
#endif
            z = 1 / (1 + (-z).PointwiseExp());
#if LOGIC
            logit.Write(false);
#elif DEBUG
            long end2 = DateTime.Now.Ticks;

            long start3 = DateTime.Now.Ticks;
#endif
            Vector h = (temp + bias_h);
#if DEBUG
            long end3 = DateTime.Now.Ticks;

            long start4 = DateTime.Now.Ticks;
#endif
#if LOGIC
            tanh.Write(true);
#endif
            h = h.PointwiseTanh();
#if LOGIC
            tanh.Write(false);
#elif DEBUG
            long end4 = DateTime.Now.Ticks;

            long start5 = DateTime.Now.Ticks;
#endif
#if LOGIC
            hidden_update.Write(true);
#endif
            hidden = (Zeta * (1 - z) + Nu).PointwiseMultiply(h) + z.PointwiseMultiply(hidden);
#if LOGIC
            hidden_update.Write(false);
            // Cell gpio pin toggle low
            cell.Write(false);
#elif DEBUG
            long end5 = DateTime.Now.Ticks;

            Microsoft.SPOT.Debug.Print("-------------------------------------------------------------------------");
            Microsoft.SPOT.Debug.Print("Vector temp1 = weight_ih_transpose * input: " + (end_2 - start_2) / (float)TimeSpan.TicksPerMillisecond);
            Microsoft.SPOT.Debug.Print("Vector temp2 = weight_hh_transpose * hidden: " + (end_1 - start_1) / (float)TimeSpan.TicksPerMillisecond);
            Microsoft.SPOT.Debug.Print("Vector temp = temp1 + temp2: " + (end0 - start0) / (float)TimeSpan.TicksPerMillisecond);
            Microsoft.SPOT.Debug.Print("Vector z = (temp + bias_z): " + (end1 - start1) / (float)TimeSpan.TicksPerMillisecond);
            Microsoft.SPOT.Debug.Print("z = 1 / (1 + (-z).PointwiseExp()): " + (end2 - start2) / (float)TimeSpan.TicksPerMillisecond);
            Microsoft.SPOT.Debug.Print("Vector h = (temp + bias_h): " + (end3 - start3) / (float)TimeSpan.TicksPerMillisecond);
            Microsoft.SPOT.Debug.Print("h = h.PointwiseTanh(): " + (end4 - start4) / (float)TimeSpan.TicksPerMillisecond);
            Microsoft.SPOT.Debug.Print("hidden = (Zeta * (1 - z) + Nu).PointwiseMultiply(h) + z.PointwiseMultiply(hidden): " + (end5 - start5) / (float)TimeSpan.TicksPerMillisecond);
            Microsoft.SPOT.Debug.Print("-------------------------------------------------------------------------");
            Microsoft.SPOT.Debug.Print("Cell running time (ms): " + (end5 - start_2) / (float)TimeSpan.TicksPerMillisecond);
            Microsoft.SPOT.Debug.Print("-------------------------------------------------------------------------");
#endif
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
            weight_hh_transpose = weight_hh.Transpose();
            weight_ih_transpose = weight_ih.Transpose();
        }

        public Vector fastgrnn_calculations(Vector input, uint window, int stride)
        {
            hidden = Vector.BuildDense(h_size);
            int cut_length = input.vec.Length;
            int time_steps = (int)(Math.Ceiling((double)(cut_length - window) / stride));
            Microsoft.SPOT.Debug.Print("RNN time steps: " + time_steps);
            //int time_steps = (int)(Math.Floor((double)(cut_length - window + stride)/stride));

            // Pre-compute input slices
            Vector[] in_slices = new Vector[time_steps];
            for (int i = 0; i < time_steps; i++)
            {
#if LOGIC
                input_strides.Write(true);
#endif
                in_slices[i] = input.SubVector(i * stride, window);
#if LOGIC      
                input_strides.Write(false);
#endif
            }

            //long start = DateTime.Now.Ticks;
            for (int i = 0; i < time_steps; i++)
            {
                fastgrnncell(in_slices[i]);
            }
            //long end = DateTime.Now.Ticks;
            //Microsoft.SPOT.Debug.Print("FastGRNN execution time (s): " + (end - start) / (float)TimeSpan.TicksPerSecond);

            return hidden;
        }
    }
}
