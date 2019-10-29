#ifdef MOTE_PROFILE
#define MOTE
#include <tinyhal.h>
#undef DBG
#endif

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#ifndef MOTE
#include <fstream>
#endif

#include "model_params.h"
#include "test_data.h"

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
inline void copyUIntVecToLL(uint* invec, ll* outvec, int vec_len)
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
inline void quantTanh(ll* vec, int vec_len, ll scale, ll* out_vec){
	for(int i=0; i<vec_len; i++)
		*(out_vec+i) = max(-scale, min(scale, *(vec+i)));
}

// quantSigm
inline void quantSigm(ll* vec, int vec_len, ll scale, ll* out_vec){
	for(int i=0; i<vec_len; i++)
		*(out_vec+i) = max(min((*(vec+i)+scale)/2, scale),0);
}

// Print utils
#ifdef DBG
void util_printVec(ll* vec, int vec_len){
	for(int i=0; i < vec_len; i++)
		cout << vec[i] << '\t';
	cout << "\n\n";
}

void util_printMatrix(uint* mat, int row_len, int col_len){
	for(int i=0; i < row_len; i++){
		for(int j=0; j < col_len; j++)
			cout << *(mat+i*col_len+j) << '\t';
		cout << endl;
	}
	cout << "\n\n";
}
#endif
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
#ifndef MOTE
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
#endif
void run_test(){
	int size = sizeof(qW1_transp_l) + sizeof(qFC_Bias_l) + sizeof(qW2_transp_l) + sizeof(qU2_transp_l) + sizeof(qFC_Weight_l) + sizeof(qU1_transp_l) + sizeof(qB_g_l) + sizeof(qB_h_l) + sizeof(q_l) + sizeof(I_l) + sizeof(mean_l) + sizeof(stdev_l) + sizeof(I_l_vec) + sizeof(q_times_I_l);
	
#ifdef DBG
	cout << "Model size: " << size/1000 << " KB" << endl << endl;
#endif
#ifdef MOTE_PROFILE
	CPU_GPIO_EnableOutputPin(0, false); //J11-3
	CPU_GPIO_EnableOutputPin(1, false); //J11-4
#endif
#ifndef MOTE
	// Initialize output file
	ofstream outfile;
	outfile.open("out_c++.csv");
#endif
	for(int d=0; d < numData; d ++)
	{
		uint test_input[timeSteps][inputDims] = {0};
		util_slice3D((uint*) test_inputs, (uint*) test_input, d, timeSteps, inputDims);

#ifdef MOTE_PROFILE
		// Profile latency per bag (second in V1)
		if(d%numInstances==0)
			CPU_GPIO_SetPinState(1, true);
#endif		

#ifdef DBG
		util_printMatrix((uint*) test_input, timeSteps, inputDims);
#endif

		ll h[hiddenDims] = {0};
	
		for(int t=0; t<timeSteps; t++){
#ifdef MOTE_PROFILE
			// Profile latency per timestep			
			//hal_printf("b");
			CPU_GPIO_SetPinState(0, true);
#endif
			uint x_int[inputDims] = {0};
			util_slice2D((uint*)test_input, x_int, t, inputDims);
	
			ll x[8] = {};
	
			copyUIntVecToLL(x_int, x, inputDims);
#ifdef DBG
			cout << "Current input array in ll" << endl;
			util_printVec(x, inputDims);
#endif	
			stdScaleInput(x, inputDims, x);
#ifdef DBG	
			cout << "Post-standardization input" << endl;
			util_printVec(x, inputDims);
#endif	
			// Precompute
			ll pre[hiddenDims] = {0};
			mulMatVec((ll*)qW1_transp_l, x, wRank, inputDims, out_wRank);
			mulMatVec((ll*)qW2_transp_l, out_wRank, hiddenDims, wRank, pre);
	
			mulMatVec((ll*)qU1_transp_l, h, uRank, hiddenDims, out_uRank);
			mulMatVec((ll*)qU2_transp_l, out_uRank, hiddenDims, uRank, out_hiddenDims);
	
			addVecs(pre, out_hiddenDims, hiddenDims, pre);
	
			divVecScal(pre, q_l, hiddenDims, pre);

#ifdef DBG
			cout << "Pre at t=" << t << endl;
			util_printVec(pre, hiddenDims);
#endif
	
			// Create h_, z
			ll h_[hiddenDims] = {0};
			ll z[hiddenDims] = {0};
	
			addVecs(pre, (ll*)qB_h_l, hiddenDims, h_);
			addVecs(pre, (ll*)qB_g_l, hiddenDims, z);
	
			UPDATE_NL(h_, hiddenDims, q_times_I_l, h_);
			divVecScal(h_, q_l, hiddenDims, h_);
	
			GATE_NL(z, hiddenDims, q_times_I_l, z);
			divVecScal(z, q_l, hiddenDims, z);
#ifdef DBG
			cout << "h_ at t=" << t << endl;
			util_printVec(h_, hiddenDims);
	
			cout << "z at t=" << t << endl;
			util_printVec(z, hiddenDims);
#endif
			// Create new h
			mulVecs(z, h, hiddenDims, h);
	
			subVecs((ll*)I_l_vec, z, hiddenDims, out_hiddenDims);
			mulVecs(out_hiddenDims, h_, hiddenDims, out_hiddenDims);
	
			addVecs(h, out_hiddenDims, hiddenDims, h);
			divVecScal(h, I_l, hiddenDims, h);
#ifdef MOTE_PROFILE
			//hal_printf("e");
			CPU_GPIO_SetPinState(0, false);
#endif
#ifdef DBG
			cout << "h at t=" << t << endl;
			util_printVec(h, hiddenDims);
#endif
		}
	
		// Classify
		mulMatVec((ll*)qFC_Weight_l, h, numClasses, hiddenDims, out_numClasses);
		addVecs(out_numClasses, (ll*)qFC_Bias_l, numClasses, out_numClasses);
#ifdef DBG
		cout << "Classification output:" << endl;
		util_printVec(out_numClasses, numClasses);
#endif
#ifdef MOTE_PROFILE
		if(d%numInstances==numInstances-1)
			CPU_GPIO_SetPinState(1, false);
#endif
#ifndef MOTE
		//Print decision to csv file
		string outstr;
		for(int c = 0; c < numClasses -1 ; c++)
			outstr += strBuild(out_numClasses[c], ',');
		outstr += strBuild(out_numClasses[numClasses -1], '\n');
		outfile << outstr;
#endif
	}
#ifndef MOTE
	outfile.close();
#else
	hal_printf("Test complete.");
#endif
}

/*int main(){
	run_test();
}*/