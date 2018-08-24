﻿/***************************************************************
 * Author:  Pranshu Jain, Indian Institute of Technology, Delhi
 * Date:    07/05/2018
 ***************************************************************/

using System;
using System.Diagnostics;
using System.Threading;
using MathNet.Numerics;
using MathNet.Numerics.LinearAlgebra;

class fastgrnn
{
    private static Matrix<double> weight_ih, weight_hh;
    private static Vector<double> hidden, bias_z, bias_h;
    private static int i_size, h_size;
    private static double zeta, nu;

    private static void fastgrnncell(Vector<double> input)
    {
        Vector<Double> z = (weight_ih.Transpose() * input + weight_hh.Transpose() * hidden + bias_z);
        z = 1 / (1 + (-z).PointwiseExp());
        Vector<Double> h = (weight_ih.Transpose() * input + weight_hh.Transpose() * hidden + bias_h);
        h = h.PointwiseTanh();
        hidden = ((1 / (1 + Math.Exp(-zeta))) * (1 - z) + (1 / (1 + Math.Exp(-nu)))).PointwiseMultiply(h) + z.PointwiseMultiply(hidden);
    }

    public fastgrnn(Matrix<Double> wih, Matrix<Double> whh, Vector<Double> bz, Vector<Double> bh, double zt, double n)
    {
        weight_ih = wih; weight_hh = whh;
        i_size = weight_ih.RowCount; h_size = weight_ih.ColumnCount;
        bias_z = bz; bias_h = bh;
        zeta = zt; nu = n;
    }

    public Vector<Double> fastgrnn_calculations(Vector<Double> input, int window, int stride)
    {
        hidden = Vector<Double>.Build.Dense(h_size);
        int cut_length = input.Count;
        int time_steps = (int)(Math.Ceiling((double)(cut_length - window) / stride));
        //int time_steps = (int)(Math.Floor((double)(cut_length - window + stride)/stride));
        for (int i = 0; i < time_steps; i++)
        {
            fastgrnncell(input.SubVector(i * stride, window));

        }

        return hidden;
    }
}

class Program
{
    static void Main(string[] args)
    {
        double[,] weight_d = { { -0.807291, 0.054143 }, { -0.148830, 0.183069 }, { 0.404488, 0.707980 }, { 0.527930, 0.877890 }, { -0.259793, 0.845178 }, { -0.007380, 1.923690 }, { 0.268987, -1.189658 }, { -1.024323, -0.384940 }, { -1.356056, 0.632008 }, { 0.305551, 0.779431 }, { 1.759865, -1.213363 }, { 2.544312, 0.398587 }, { -2.167474, -0.412874 }, { 0.140635, 0.478601 }, { 0.970677, -1.772175 }, { 2.474926, -0.858927 } };
        double[,] weight_hh_d = { { 0.095296, -0.094757, -0.167785, -0.048441, -0.119560, 0.223966, -0.026868, -0.062044, 0.008684, 0.157768, -0.260443, 0.127656, 0.142844, -0.139087, -0.265670, -0.042686 }, { -0.140354, 0.132777, -0.012269, -0.054090, 0.004996, -0.215220, -0.337669, -0.274117, 0.032089, -0.152319, -0.008621, 0.119962, -0.421117, -0.112611, -0.286470, 0.350018 }, { -0.370852, 0.026803, 0.185052, -0.072138, -0.012923, -0.380921, -0.063776, -0.129190, -0.031414, -0.112181, 0.027522, -0.103328, -0.278160, -0.153880, 0.027692, -0.030082 }, { -0.143446, -0.029842, -0.024482, -0.078337, -0.065087, 0.452156, 0.068704, -0.065161, 0.020324, 0.132691, -0.140195, -0.082441, 0.003262, 0.008299, -0.375984, -0.427066 }, { -0.039452, 0.133458, 0.086650, 0.004027, -0.088567, -0.119544, -0.146666, -0.187629, 0.245142, -0.097486, 0.231028, -0.139706, -0.177773, -0.220417, -0.515569, -0.291572 }, { -0.119163, 0.035669, 0.026433, 0.239184, 0.027379, 0.532934, 0.061440, -0.092493, 0.034270, 0.221838, -0.182648, -0.201386, -0.145751, 0.068922, -0.690937, -0.605899 }, { 0.165179, -0.096901, -0.325785, 0.171979, -0.222776, 0.107114, 0.009311, -0.311943, -0.179049, 0.206143, 0.012336, -0.481572, -0.175070, 0.284959, 0.113791, -0.539374 }, { 0.096146, 0.081400, -0.193124, -0.139117, -0.203427, 0.034535, 0.240119, 0.034113, 0.032315, 0.193601, -0.176622, -0.058031, -0.034822, 0.091305, 0.165781, 0.009039 }, { -0.133916, 0.074230, 0.075313, -0.168746, 0.269984, -0.199086, -0.091068, -0.184775, -0.131761, 0.034491, -0.044894, -0.029969, -0.152212, -0.141207, -0.240130, -0.197781 }, { -0.013680, 0.025880, -0.073957, 0.189186, 0.139666, 0.490411, -0.055518, -0.231434, -0.346435, -0.061413, 0.009451, 0.100625, -0.372333, -0.012592, -0.398752, -0.223704 }, { -0.301359, 0.199858, 0.022153, -0.346225, 0.002330, -0.850531, -0.228147, -0.067157, 0.098913, -0.290422, 1.092212, 0.352541, -0.110814, 0.225351, -0.390548, 0.154644 }, { -0.115320, -0.062401, -0.197004, 0.011147, -0.233101, -0.423935, -0.050704, -0.110806, -0.204212, -0.225399, 0.263876, 0.062924, -0.233863, 0.017940, 0.373936, 0.051745 }, { 0.225193, 0.026116, 0.137963, 0.153754, -0.008488, -0.077943, -0.231065, -0.061954, 0.338705, -0.204226, -0.049731, -0.189403, -0.089194, 0.095336, -0.341446, -0.424124 }, { 0.198796, -0.179291, -0.252742, 0.198832, 0.084339, 0.280105, 0.045309, -0.012162, 0.197257, 0.089337, 0.515444, -0.051886, 0.221071, 0.012652, -0.586627, -0.576505 }, { 0.049291, -0.116907, -0.048729, -0.032367, -0.105319, -0.410434, 0.326631, -0.065737, -0.073069, -0.141285, -0.512469, 0.272834, -0.169589, 0.085407, 0.224846, 0.052239 }, { 0.185459, 0.036333, 0.015072, -0.174054, -0.004710, -0.418777, 0.291061, 0.137380, -0.114493, -0.029906, 0.130155, 0.131421, -0.326204, -0.157912, 0.145731, 0.163904 } };
        double[,] weight_ih_d = { { 0.222557, -0.262699, -0.024119, 0.064634, 0.458134, -0.080916, 0.032349, -0.091888, 0.098388, -0.512823, 0.163841, -0.260286, 0.557074, 0.161419, -0.247796, -0.252737 }, { 0.131642, -0.120850, -0.178273, 0.006016, 0.330613, -0.170522, -0.115710, 0.148491, 0.428042, -0.233770, 0.058651, 0.445961, -0.442697, 0.165113, -0.230008, 0.301324 }, { 0.274770, -0.267895, -0.009568, 0.347459, -0.085984, -0.201775, -0.020205, 0.052968, 0.514281, -0.387221, 0.095725, 0.421811, -0.114677, 0.266447, -0.389046, 0.217325 }, { 0.029996, 0.067386, -0.136256, -0.025923, 0.015652, -0.313656, -0.336820, 0.135331, 0.117917, 0.311904, -0.055468, 0.426640, -0.204970, 0.248137, -0.211688, 0.016199 }, { 0.068942, 0.013122, -0.139901, 0.380709, -0.466074, 0.142793, -0.008916, -0.051247, 0.139419, -0.220191, 0.093582, 0.322994, 0.013665, 0.121270, -0.058602, 0.169794 }, { 0.035120, 0.164825, 0.056314, -0.123586, -0.016355, 0.072114, -0.344923, -0.117403, -0.375697, 0.403167, 0.061898, -0.067054, 0.365363, 0.222105, 0.188450, 0.098553 }, { -0.418565, 0.255023, -0.108439, -0.004086, 0.126131, 0.146235, -0.378618, -0.088487, -0.229656, -0.140705, 0.099670, 0.335039, -0.205018, -0.053529, 0.019391, 0.092388 }, { 0.097618, 0.219178, 0.012538, -0.178338, 0.443639, 0.517251, -0.194892, -0.216401, -0.113795, 0.198402, 0.235259, -0.222819, 0.032255, 0.311890, -0.025779, 0.205664 }, { 0.107801, 0.112063, 0.067474, 0.135057, 0.168883, 0.026045, -0.332496, -0.078385, -0.047601, -0.002009, 0.256619, 0.083038, 0.090367, 0.187250, -0.220003, -0.092718 }, { 0.087916, -0.142956, -0.073055, -0.018606, -0.198186, 0.019623, -0.078038, -0.228456, -0.094193, 0.128757, 0.244585, -0.264860, -0.216267, 0.307181, -0.048640, 0.126476 }, { -0.031923, -0.246318, 0.122320, 0.037476, -0.079300, 0.046465, -0.092280, -0.075132, -0.042896, -0.215026, -0.055118, -0.137763, -0.124879, -0.056627, -0.058325, -0.060733 }, { 0.122898, -0.040686, -0.106160, 0.004890, -0.177303, -0.555728, 0.138595, -0.139106, -0.149647, -0.169599, -0.197045, -0.206442, -0.333872, 0.291776, -0.127201, -0.120739 }, { 0.149775, -0.064367, 0.174151, -0.016999, -0.088758, -0.007959, -0.155405, -0.059138, -0.289479, -0.172139, -0.248887, -0.403000, 0.039253, -0.105359, -0.089591, -0.316303 }, { 0.148136, -0.191341, -0.236983, -0.127529, 0.082342, -0.549013, 0.405665, 0.002050, -0.139286, 0.154416, -0.064317, -0.097090, -0.018535, 0.170147, 0.019907, 0.161577 }, { -0.011208, -0.059546, 0.286941, -0.164491, -0.262640, 0.231661, -0.062413, 0.008865, -0.340696, -0.016079, 0.012585, -0.219879, 0.002142, 0.029676, -0.129349, -0.141708 }, { -0.178216, 0.027033, 0.215674, -0.132476, 0.230466, -0.527433, 0.280467, -0.060711, 0.015775, 0.039859, 0.278775, 0.139421, 0.362601, 0.352947, 0.167975, 0.058636 }, { -0.078838, 0.270290, 0.170122, -0.096035, 0.018460, -0.250378, 0.237482, -0.062723, -0.174588, -0.178044, 0.153325, -0.140039, -0.139232, 0.210786, 0.252467, -0.038645 }, { 0.002238, 0.107751, 0.323442, -0.151654, -0.014218, 0.169822, 0.358606, -0.140607, 0.132073, 0.277321, 0.154462, 0.118437, 0.264012, 0.407914, 0.190472, 0.242032 }, { 0.050042, 0.059601, 0.071622, 0.088081, 0.005511, -0.284459, 0.363627, -0.046305, -0.058008, 0.185999, -0.079516, 0.001580, 0.073433, -0.072296, 0.125453, 0.055734 }, { -0.129019, 0.100474, 0.260067, -0.056810, -0.028657, 0.340164, 0.090432, -0.054359, 0.179681, 0.119666, 0.059927, 0.264582, -0.126748, 0.384354, 0.196117, -0.042495 }, { 0.016865, -0.155486, -0.416045, -0.223905, -0.007201, 0.198591, 0.339423, -0.055147, 0.000073, 0.087638, -0.203119, 0.014207, -0.091822, 0.028802, 0.165995, -0.087843 }, { -0.097230, 0.041265, -0.014200, 0.109580, -0.306454, 0.149150, -0.152856, 0.147646, 0.136794, -0.077594, 0.206056, 0.134403, -0.083920, 0.308213, -0.145628, -0.023940 }, { 0.262198, -0.285507, -0.277054, -0.520907, 0.066004, 0.093807, 0.330505, -0.498682, 0.239412, -0.180645, 0.164839, 0.271409, 0.218966, 0.209452, 0.110314, 0.051303 }, { -0.026650, -0.005358, -0.067826, -0.173242, -0.191550, -0.646819, -0.103870, 0.096304, 0.069439, 0.038006, -0.040097, 0.051597, 0.094015, 0.308455, -0.163417, 0.075581 }, { -0.097091, -0.282204, -0.024080, -0.337709, -0.146778, 0.194130, 0.303269, -0.101853, 0.310332, 0.052864, 0.047556, 0.091589, 0.278964, 0.026938, -0.164399, -0.037650 }, { 0.436878, 0.228830, -0.076019, -0.080531, 0.108732, -0.469716, 0.164302, -0.104122, 0.271689, 0.041580, 0.019797, 0.009506, 0.241732, 0.386104, -0.221120, 0.049273 }, { -0.022797, -0.202254, 0.042628, -0.069167, -0.114539, -0.079642, 0.201183, 0.165733, 0.416121, 0.149464, 0.087453, 0.027539, 0.219280, -0.029451, 0.018276, 0.212674 }, { -0.006005, 0.247552, -0.117738, 0.224642, 0.112231, -0.008201, 0.021954, -0.117260, -0.025519, -0.012528, 0.238178, -0.124273, 0.341450, 0.028981, 0.016481, 0.301496 }, { 0.150347, -0.014293, -0.048373, -0.135864, 0.027767, -0.533068, 0.224827, 0.006799, -0.008818, 0.192508, 0.257486, 0.048268, -0.064030, -0.004845, -0.032244, -0.001061 }, { -0.059212, 0.066691, 0.104350, 0.179880, -0.041952, 0.208344, -0.050791, -0.178796, -0.306042, -0.159161, 0.051805, -0.035198, 0.269782, 0.241361, 0.008905, -0.113223 }, { -0.397237, -0.062077, -0.052884, -0.383457, 0.002811, -0.182478, 0.159477, -0.128023, -0.354018, 0.429225, -0.154306, 0.186542, 0.004145, 0.116814, 0.062904, 0.091490 }, { -0.124406, -0.355966, 0.079270, -0.054845, -0.338529, 0.354785, -0.209201, -0.109550, -0.139628, -0.249953, 0.088102, -0.266389, -0.108667, 0.284838, 0.242842, 0.177848 } };
        double[] bias_d = { -1.382853, 0.377250 };
        double[] bias_z_d = { 1.080809, 1.143059, 1.067185, 1.071548, 0.994298, 0.968734, 1.201216, 0.995048, 1.069691, 1.020219, 1.580706, 0.989805, 0.918537, 1.045085, 0.694268, 0.981650 };
        double[] bias_h_d = { 0.859399, 0.814987, 0.749016, 0.930546, 0.915231, 0.757144, 0.912455, 0.865331, 0.851570, 0.934425, 0.689774, 0.848360, 0.958885, 1.081108, 0.989481, 0.902728 };
        double[] input_d = { 2063, 1954, 2128, 1983, 2153, 2058, 2129, 2111, 2081, 2123, 2025, 2108, 1987, 2065, 2001, 2020, 2028, 2017, 2044, 2036, 2042, 2031, 2042, 2004, 2064, 1996, 2106, 2035, 2118, 2091, 2098, 2132, 2047, 2133, 1983, 2092, 1977, 2020, 1999, 1988, 2028, 2002, 2025, 1996, 2042, 2015, 2067, 1997, 2110, 2024, 2142, 2085, 2118, 2145, 2050, 2145, 1982, 2090, 1957, 2009, 1995, 1951, 2069, 1974, 2103, 2027, 2088, 2061, 2073, 2054, 2085, 2045, 2090, 2085, 2067, 2106, 2017, 2080, 1967, 2021, 2014, 1992, 2053, 1990, 2089, 2026, 2094, 2057, 2077, 2049, 2075, 2042, 2098, 2070, 2092, 2111, 2031, 2122, 1956, 2072, 1950, 1994, 1991, 1944, 2060, 1956, 2107, 2006, 2127, 2049, 2131, 2070, 2129, 2112, 2114, 2168, 2020, 2151, 1945, 2086, 1937, 1987, 2005, 1924, 2080, 1949, 2142, 2009, 2149, 2077, 2125, 2122, 2087, 2148, 2049, 2142, 1970, 2101, 1917, 2023, 1949, 1949, 2029, 1919, 2111, 1953, 2164, 2022, 2172, 2099, 2134, 2132, 2075, 2167, 1992, 2126, 1943, 2053, 1943, 1975, 1988, 1935, 2064, 1951, 2136, 2003, 2167, 2074, 2120, 2132, 2057, 2162, 1993, 2125, 1940, 2062, 1947, 1973, 2001, 1917, 2082, 1929, 2159, 1998, 2172, 2094, 2129, 2164, 2058, 2192, 1990, 2158, 1921, 2091, 1923, 1986, 1977, 1930, 2037, 1930, 2129, 1977, 2169, 2040, 2141, 2119, 2106, 2153, 2035, 2151, 1973, 2103, 1940, 2026, 1959, 1952, 2019, 1921, 2103, 1958, 2172, 2032, 2166, 2120, 2112, 2159, 2035, 2151, 1961, 2097, 1947, 2017, 1998, 1963, 2084, 1954, 2139, 2012, 2150, 2090, 2096, 2124, 2014, 2134, 1962, 2097, 1935, 2001, 1980, 1930, 2056, 1939, 2127, 1981, 2163, 2077, 2143, 2149, 2094, 2157, 1990, 2121, 1939, 2059, 1946, 1978, 1994, 1931, 2078, 1952, 2166, 1939, 2170, 2073, 2199, 2146, 2082, 2173, 1971, 2116, 1930, 2018, 1961, 1955, 2089, 2007, 2039, 2023, 2142, 2096, 2168, 2126, 2111, 2203, 1983, 2155, 1923, 2031, 1933, 1957, 2003, 1915, 2088, 1942, 2186, 2032, 2182, 2106, 2126, 2178, 2018, 2174, 1918, 2095, 1911, 1968, 1978, 1892, 2103, 1917, 2177, 2019, 2199, 2150, 2112, 2202, 2001, 2170, 1909, 2070, 1903, 1956, 1971, 1888, 2092, 1905, 2179, 1988, 2212, 2128, 2149, 2206, 2034, 2199, 1920, 2123, 1881, 1998, 1945, 1910, 2056, 1915, 2170, 1988, 2181, 2108, 2091, 2186, 2004, 2163, 1946, 2058, 1951, 1962, 2032, 1931, 2116, 1975, 2161, 2080, 2130, 2148, 2055, 2153, 1990, 2099, 1943, 2007, 1988, 1940, 2082, 1941, 2156, 2022, 2156, 2105, 2086, 2147, 2000, 2131, 1942, 2058, 1967, 1981, 2027, 1940, 2106, 1960, 2167, 2049, 2146, 2132, 2089, 2173, 1958, 2117, 1907, 2012, 1958, 1918, 2041, 1924, 2138, 1985, 2194, 2101, 2135, 2183, 2030, 2184, 1922, 2114, 1897, 1975, 1973, 1895, 2094, 1923, 2217, 2024, 2209, 2133, 2103, 2210, 1958, 2164, 1865, 2027, 1900, 1910, 2025, 1863, 2184, 1937, 2250, 2094, 2205, 2231, 2049, 2249, 1913, 2135, 1853, 1976, 1941, 1858, 2098, 1873, 2235, 2007, 2247, 2180, 2121, 2243, 1946, 2226, 1846, 2082, 1855, 1914, 1993, 1840, 2141, 1916, 2212, 2057, 2173, 2158, 2066, 2192, 1960, 2117, 1929, 2002, 1969, 1927, 2074, 1912, 2183, 1995, 2208, 2114, 2115, 2200, 1994, 2176, 1901, 2052, 1918, 1923, 2015, 1865, 2157, 1960, 2218, 2110, 2151, 2220, 2017, 2197, 1894, 2067, 1914, 1940, 2023, 1882, 2156, 1940, 2227, 2105, 2201, 2192, 2050, 2236, 1895, 2146, 1835, 1973, 1915, 1853, 2086, 1828, 2248, 1956, 2287, 2148, 2167, 2287, 1999, 2270, 1856, 2117, 1822, 1939, 1940, 1814, 2124, 1849, 2266, 2019, 2258, 2209, 2096, 2260, 1941, 2204, 1838, 2042, 1874, 1984, 2006, 1840, 2148, 1892, 2251, 2027, 2216, 2183, 2088, 2232, 1965, 2162, 1888, 2021, 1936, 1922, 2047, 1908, 2156, 1986, 2182, 2116, 2112, 2206, 2013, 2181, 1914, 2063, 1903, 1928, 1996, 1866, 2120, 1892, 2235, 2016, 2233, 2170, 2122, 2242, 1982, 2194, 1883, 2067, 1894, 1943, 1994, 1858, 2103, 1906, 2205, 2011, 2199, 2137, 2103, 2196, 1997, 2173, 1920, 2060, 1933, 1943, 2010, 1921, 2114, 1945, 2185, 2060, 2156, 2159, 2068, 2186, 1963, 2113, 1919, 1999, 1968, 1935, 2061, 1942, 2138, 2005, 2157, 2088, 2123, 2133, 2035, 2128, 1986, 2099, 1961, 2037, 1966, 1981, 2013, 1955, 2046, 1869, 2128, 1985, 2174, 2116, 2123, 2185, 2032, 2189, 1934, 2119, 1877, 2006, 1923, 1902, 2024, 1848, 2158, 1906, 2252, 2056, 2203, 2210, 2099, 2261, 1937, 2193, 1845, 2039, 1897, 1895, 2023, 1836, 2171, 1908, 2242, 2068, 2191, 2214, 2029, 2236, 1887, 2132, 1857, 1980, 1944, 1856, 2092, 1857, 2219, 1985, 2262, 2134, 2167, 2249, 2010, 2251, 1856, 2126, 1829, 1968, 1918, 1848, 2073, 1841, 2216, 1946, 2280, 2125, 2202, 2246, 2042, 2253, 1890, 2155, 1843, 1991, 1909, 1864, 2065, 1854, 2204, 1956, 2254, 2113, 2158, 2238, 2026, 2230, 1898, 2123, 1865, 1994, 1939, 1884, 2077, 1868, 2200, 1941, 2255, 2094, 2195, 2213, 2065, 2245, 1938, 2177, 1839, 2066, 1865, 1914 };
        //double[] input_d = {2033,2041,2006,2001,2028,2008,1987,1963,2029,2015,1985,1963,1990,1978,1983,1981,1978,1963,1990,2013,1974,1990,1991,2005,1984,1994,2015,2003,1984,1988,1968,1975,1964,1989,1968,1992,1998,2012,1989,2003,2017,2025,1983,2005,1999,2023,1997,1963,1979,1937,1992,1972,1988,1997,2027,2012,1990,1963,2043,2010,1997,1966,2028,1993,1988,1972,2001,1956,1992,1994,2005,1993,2025,1997,1994,1983,2012,2002,2000,1962,2027,1991,2010,1954,1976,1976,1981,1969,1945,1982,1979,1999,1969,1975,1995,2012,2004,2010,2012,2001,2008,1974,1997,1976,2001,1991,1977,1966,2002,1981,1985,1978,1985,2019,1975,2003,2013,2012,1983,1963,2020,2008,1983,1979,1964,1977,1962,2003,1965,1995,1999,2034,1989,1986,1995,2012,2005,1966,2031,2002,2000,1970,1985,1973,1997,1983,1988,1961,1978,2000,1969,1975,1998,2045,1978,2015,2039,2028,1984,1991,2017,2023,2006,1980,1972,1968,2001,1989,1988,1947,1982,2011,1962,1973,2013,2015,1982,2001,2034,2015,2007,1961,2011,1960,2006,1980,1985,1975,1995,2013,1960,1993,2008,2033,1990,1999,2019,1989,2000,1971,2020,1968,1990,1988,1980,1942,1981,2015,1942,2005,1974,2033,1969,1973,1993,2018,1973,1987,2012,2028,1995,1976,2029,1956,2030,1978,1991,1962,1986,1989,1953,1988,1975,1992,1964,1978,2036,2014,1989,1961,2014,1984,2010,1987,1988,1964,2024,1980,1980,1955,1996,1987,1992,1983,2027,2033,1987,2001,2012,1979,1977,1966,2008,1973,2015,1997,1992,1963,1999,2005,1971,1967,2026,2020,2001,1996,2006,2040,1992,2000,2002,2000,1992,2000,1973,1988,2017,2005,1973,1962,1997,1985,1960,1987,1997,2013,1986,2022,2025,2022,1986,1978,1996,1972,2023,2001,2010,1992,1998,2003,1957,1985,2014,2043,1989,2029,2018,1988,1990,1989,1994,2014,1991,1980,1972,1955,1983,1989,1950,1987,2003,2043,1996,2019,2000,2020,1990,2005,2006,1981,1975,1973,1970,2002,1989,2009,1983,1984,1978,2018,1953,1989,2014,2001,1986,1982,1992,2018,1963,1972,1991,1976,2017,1985,1988,1972,1982,1995,1979,2006,2038,2006,2002,1968,2008,2024,1992,1980,2000,1973,2008,2009,1981,1953,1997,2007,1975,1995,1992,2025,2011,1970,2022,2016,2027,1980,2012,1975,1986,1971,1952,1964,1960,1982,1978,1984,1993,2011,2000,1995,2011,2030,1994,1970,1998,1964,1987,2005,1958,1984,1978,1990,1951,2004,1979,2010,1970,1993,1984,2012,1981,1993,1988,1989,1983,1994,1958,1941,1963,1979,1973,1989,1978,2018,1996,1995,2004,2031,1985,1958,2018,1965,2014,1978,1975,1946,2008,1999,1998,1984,2028,2026,1982,2003,2009,2029,1978,1977,2009,1940,2012,1980,1968,1974,1996,1997,1970,1955,2016,2019,2003,1951,2006,2005,1975,1971,2002,1978,2004,1994,1963,1971,2013,1986,1982,1977,1999,2001,1970,1996,2009,2017,1988,1982,1996,1963,1997,1989,1968,1987,2015,2005,1990,1978,2011,2021,1970,1999,2003,2020,1977,1993,2001,1982,2012,1946,2006,1952,2023,1998,2001,1970,1993,2004,1998,1994,2010,2023,1988,2009,1988,2017,2006,1975,1994,1958,2030,1999,1980,1955,2021,1969,1987,1952,2000,1976,1977,1974,2001,1994,1977,1961,2013,1981,2029,2013,1989,1962,1995,1989,1976,1977,2009,1998,1967,2000,2023,2017,2011,1991,2012,1997,2004,1994,1988,1974,2009,1980,1981,1969,2010,2027,1963,1985,1999,2013,1968,1959,2016,2018,1991,2004,1993,1952,2037,1998,2009,1985,2018,1990,1975,1976,1991,2010,1964,2006,2015,2051,1995,2046,1992,1987,2021,1996,1975,1974,2017,1996,1975,1972,2001,2006,1960,1941,2023,2005,2036,1980,2020,1999,2016,1982,1988,1948,2010,1981,2000,1973,1986,2016,1980,1987,1981,2013,1989,1989,1995,1993,1997,1978,2003,1976,1999,2002,1985,1967,1981,2000,1973,1979,2015,2011,1974,1980,1986,2032,1980,1975,1998,1982,2012,1990,1995,1976,2004,2010,1986,1976,1989,2012,1970,1987,1986,2000,1977,1986,2003,2032,2011,2001,2019,1958,2011,1986,1966,1964,1995,1990,1975,1992,1986,2018,1960,1992,2021,2020,2021,2008,1996,1997,2020,2013,1970,1981,2001,2009,1973,1969,1974,2008,1964,1980,2008,2034,2004,1982,1978,1975,1984,1988,1994,1971,2004,1995,1997,1963,2007,2014,1986,1989,1984,2010,1977,1992,1989,2008,1995,2004,2008,1981,2021,2003,1956,1975,1988,1984,1973,1991};
        double mean = 2039.426298; double std = 180.046503;
        double zeta = 6.016959; double nu = -5.950761;

        var weight = Matrix<double>.Build.DenseOfArray(weight_d);
        var weight_ih = Matrix<double>.Build.DenseOfArray(weight_ih_d);
        var weight_hh = Matrix<double>.Build.DenseOfArray(weight_hh_d);
        var bias = Vector<double>.Build.DenseOfArray(bias_d);
        var bias_z = Vector<double>.Build.DenseOfArray(bias_z_d);
        var bias_h = Vector<double>.Build.DenseOfArray(bias_h_d);
        var input = Vector<double>.Build.DenseOfArray(input_d);
        int window = 32, stride = 16;
        fastgrnn ins1;
        ins1 = new fastgrnn(weight_ih, weight_hh, bias_z, bias_h, zeta, nu);

        Stopwatch stopWatch = Stopwatch.StartNew();
        stopWatch.Start();
        Vector<double> logits = weight.Transpose() * ins1.fastgrnn_calculations((input - mean) / std, window, stride) + bias;

        stopWatch.Stop();
        TimeSpan ts = stopWatch.Elapsed;
        //string elapsedTime = String.Format("{0:00}:{1:00}:{2:00}.{3:00}",
        //ts.Hours, ts.Minutes, ts.Seconds,
        //ts.Milliseconds );
        Console.WriteLine("Elapsed time: " + stopWatch.ElapsedMilliseconds + " ms");
        Console.Write("Logits: " + logits);

    }
}