#include <iostream>
//#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <ctime>

#include "model_params.h"

using namespace std;

#define min(a,b) ((b)>(a))?a:b
#define max(a,b) ((a)>(b))?a:b

#define UPDATE_NL quantTanh
#define GATE_NL quantSigm

ll out_wRank[wRank] = {0};
ll out_uRank[uRank] = {0};
ll out_hiddenDims[hiddenDims] = {0};
ll out_numClasses[numClasses] = {0};

// Copy uint into ll array
void copyUIntVecToLL(uint* invec, ll* outvec, int vec_len)
{
	copy(invec,invec+vec_len, outvec);
}

//Vector-scalar multiplication
void mulVecScal(ll* vec, ll scal, int vec_len, ll* out){
	for(int i=0; i < vec_len; i++)
		out[i] = *(vec+i)*scal;
} 

//Vector-scalar division
void divVecScal(ll* vec, ll scal, int vec_len, ll* out){
	for(int i=0; i < vec_len; i++)
		out[i] = *(vec+i)/scal;
} 

//Matrix-vector multiplication
void mulMatVec(ll* mat, ll* vec, int mat_rows, int vec_len, ll* out){
	for(int i=0; i < mat_rows; i++){
		out[i] = 0;
		for(int j=0; j < vec_len; j++)
			out[i] += *((mat+i*vec_len)+j)*(*(vec+j));
	}
}

//Vector-vector addition
void addVecs(ll* vec1, ll* vec2, int vec_len, ll* out){
	for(int i=0; i < vec_len; i++)
		out[i] = *(vec1+i)+*(vec2+i);
}

//Vector-vector subtraction
void subVecs(ll* vec1, ll* vec2, int vec_len, ll* out){
	for(int i=0; i < vec_len; i++)
		out[i] = *(vec1+i)-*(vec2+i);
}

//Vector-vector multiplication (Hadamard)
void mulVecs(ll* vec1, ll* vec2, int vec_len, ll* out){
	for(int i=0; i < vec_len; i++)
		out[i] = *(vec1+i)*(*(vec2+i));
}

// Standardization with scaling
void stdScaleInput(ll* in_vec, int vec_len, ll* out_vec){
	for(int i=0; i<vec_len; i++)
		*(out_vec+i) = I_l*(*(in_vec+i)-mean_l[i])/stdev_l[i];
}

// quantTanh
void quantTanh(ll* vec, int vec_len, ll scale, ll* out_vec){
	for(int i=0; i<vec_len; i++)
		*(out_vec+i) = max(-scale, min(scale, *(vec+i)));
}

// quantSigm
void quantSigm(ll* vec, int vec_len, ll scale, ll* out_vec){
	for(int i=0; i<vec_len; i++)
		*(out_vec+i) = max(min((*(vec+i)+scale)/2, scale),0);
}

// Print utils
/*void util_printVec(ll* vec, int vec_len){
	for(int i=0; i < vec_len; i++)
		printf("%lli\t", vec[i]);
	printf("\n\n");
}

void util_printMatrix(uint* mat, int row_len, int col_len){
	for(int i=0; i < row_len; i++){
		for(int j=0; j < col_len; j++)
			printf("%u\t", *(mat+i*col_len+j));
		printf("\n");
	}
	printf("\n\n");
}*/

// Matrix slice utils
void util_slice2D(uint* src, uint*dst, int row_index, int vec_len){
	for(int j=0; j < vec_len; j++)
		*(dst+j) = *((src+row_index*vec_len)+j);
}

void util_slice3D(uint* src, uint*dst, int row_index, int col_len, int vec_len){
	uint *slice_beg = (src + row_index * col_len * vec_len);
	for(int k=0; k < col_len * vec_len; k++)
			*(dst+k) = *(slice_beg + k);
}

// String builder util
string strBuild(ll i, char delim)
{
    string s;
    stringstream out;
    out << i;
    out << delim;
    s = out.str();
    return s;
}

int main(){
	int size = sizeof(qW1_transp_l) + sizeof(qFC_Bias_l) + sizeof(qW2_transp_l) + sizeof(qU2_transp_l) + sizeof(qFC_Weight_l) + sizeof(qU1_transp_l) + sizeof(qB_g_l) + sizeof(qB_h_l) + sizeof(q_l) + sizeof(I_l) + sizeof(mean_l) + sizeof(stdev_l) + sizeof(I_l_vec) + sizeof(q_times_I_l);
	//printf("Model size: %d KB\n\n", size/1000);

	// Initialize output file
	//ofstream outfile;
	//outfile.open("out_c++.csv");

	// Two instances for testing - 12x8 each
	uint test_inputs[][12][8] = {{{2032,2041,2019,2014,2034,2021,2038,2053},{2017,2051,2017,2041,2039,2050,2013,2056},{2016,2035,2021,2040,2008,2059,2026,2046},{2049,2050,2026,2067,2025,2041,2039,2025},{2025,2056,1994,2039,2015,2037,2029,2049},{2018,2060,2014,2046,2036,2029,2018,2045},{2020,2031,2037,2023,2001,2056,2002,2029},{2031,2028,2041,2059,2024,2060,2052,2036},{2045,2034,2012,2030,2019,1965,2039,2023},{2014,2073,2015,2031,2054,2026,2028,2060},{2023,2020,2046,2020,2035,2037,2012,2029},{2050,2019,2055,2042,2019,2063,2020,2043}}, {{2045,2034,2012,2030,2019,1965,2039,2023},{2014,2073,2015,2031,2054,2026,2028,2060},{2023,2020,2046,2020,2035,2037,2012,2029},{2050,2019,2055,2042,2019,2063,2020,2043},{2039,2043,2013,2059,2001,2020,2044,2034},{2029,2058,2002,2046,2040,2017,2051,2050},{2026,2070,2034,2020,2042,2019,2009,2042},{2019,2031,2054,2046,2040,2058,2033,2048},{2045,2027,2037,2070,2009,2076,2026,2026},{2051,2047,2005,2055,2009,2030,2043,2057},{2017,2059,2020,2049,2041,2052,2007,2049},{2021,2016,2028,2036,2015,2052,2007,2043}}};

	int numData = 2;

	for(int d=0; d < numData; d ++)
	{
		uint test_input[timeSteps][inputDims] = {0};
		util_slice3D((uint*) test_inputs, (uint*) test_input, d, timeSteps, inputDims);
		//util_printMatrix((uint*) test_input, timeSteps, inputDims);

		ll h[hiddenDims] = {0};
	
		for(int t=0; t<timeSteps; t++){
			clock_t startTime = clock(); //Start timer
			//util_printVec(h, hiddenDims);
			uint x_int[inputDims] = {0};
			util_slice2D((uint*)test_input, x_int, t, inputDims);
	
			ll x[8] = {};
	
			copyUIntVecToLL(x_int, x, inputDims);
	
			//printf("Current input array in ll\n");
			//util_printVec(x, inputDims);
	
			stdScaleInput(x, inputDims, x);
	
			//printf("Post-standardization input\n");
			//util_printVec(x, inputDims);
	
			// Precompute
			ll pre[hiddenDims] = {0};
			mulMatVec((ll*)qW1_transp_l, x, wRank, inputDims, out_wRank);
			mulMatVec((ll*)qW2_transp_l, out_wRank, hiddenDims, wRank, pre);
	
			//util_printVec(pre, hiddenDims);
	
			mulMatVec((ll*)qU1_transp_l, h, uRank, hiddenDims, out_uRank);
			mulMatVec((ll*)qU2_transp_l, out_uRank, hiddenDims, uRank, out_hiddenDims);
	
			addVecs(pre, out_hiddenDims, hiddenDims, pre);
	
			//util_printVec(pre, hiddenDims);
	
			divVecScal(pre, q_l, hiddenDims, pre);
	
			//printf("Pre at t=%d:\n", t);
			//util_printVec(pre, hiddenDims);
	
			// Create h_, z
			ll h_[hiddenDims] = {0};
			ll z[hiddenDims] = {0};
	
			addVecs(pre, (ll*)qB_h_l, hiddenDims, h_);
			addVecs(pre, (ll*)qB_g_l, hiddenDims, z);
	
			UPDATE_NL(h_, hiddenDims, q_times_I_l, h_);
			divVecScal(h_, q_l, hiddenDims, h_);
	
			GATE_NL(z, hiddenDims, q_times_I_l, z);
			divVecScal(z, q_l, hiddenDims, z);
	
			//printf("h_ at t=%d:\n", t);
			//util_printVec(h_, hiddenDims);
	
			//printf("z at t=%d:\n", t);
			//util_printVec(z, hiddenDims);
	
			// Create new h
			mulVecs(z, h, hiddenDims, h);
	
			subVecs((ll*)I_l_vec, z, hiddenDims, out_hiddenDims);
			mulVecs(out_hiddenDims, h_, hiddenDims, out_hiddenDims);
	
			addVecs(h, out_hiddenDims, hiddenDims, h);
			divVecScal(h, I_l, hiddenDims, h);

			double secondsPassed = (clock() - startTime) * 1e3 / (double) CLOCKS_PER_SEC;
			cout << "Loop running time: " << secondsPassed << " ms" << endl;
			//printf("h at t=%d:\n", t);
			//util_printVec(h, hiddenDims);
		}
	
		// Classify
		mulMatVec((ll*)qFC_Weight_l, h, numClasses, hiddenDims, out_numClasses);
		addVecs(out_numClasses, (ll*)qFC_Bias_l, numClasses, out_numClasses);
	
		//printf("Classification output:\n");
		//util_printVec(out_numClasses, numClasses);

		//Print decision to csv file
		string outstr;
		for(int c = 0; c < numClasses -1 ; c++)
			outstr += strBuild(out_numClasses[c], ',');
		outstr += strBuild(out_numClasses[numClasses -1], '\n');
		//cout << outstr;
		//outfile << outstr;
	}
	//outfile.close();
}