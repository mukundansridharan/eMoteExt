#ifdef MOTE_PROFILE
#define MOTE
#include <tinyhal.h>
#endif

#ifdef MOTE
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

#include "utils.h"

// initialize outfile if running on PC
#ifndef MOTE
	// Initialize output file
	ofstream outfile;
#endif

//static ll h[hiddenDims_u] = {0};
static ll out_wRank_u[wRank_u] = {0};
static ll out_uRank_u[uRank_u] = {0};
static ll out_hiddenDims_u[hiddenDims_u] = {0};
static ll out_numClasses_u[numClasses_u] = {0};
static ll x[inputDims_u] = {0};
static ll pre[hiddenDims_u] = {0};
static ll h_[hiddenDims_u] = {0};
static ll z[hiddenDims_u] = {0};

inline int upper_fastgrnn(uint* test_input){
	ll h[hiddenDims_u] = {0};	
	for(int t=0; t<timeSteps_u; t++){
#ifdef MOTE_PROFILE
		// Profile latency per timestep			
		//hal_printf("b");
		CPU_GPIO_SetPinState(0, true);
#endif
		uint x_int[inputDims_u] = {0};
		util_slice2D(test_input, x_int, t, inputDims_u);

		copyUIntVecToLL(x_int, x, inputDims_u);
#ifdef DBG
		cout << "Current input array in ll" << endl;
		util_printVec(x, inputDims_u);
#endif	
		stdScaleInput(x, (ll*)mean_u, (ll*)stdev_u, I_u, inputDims_u, x);
#ifdef DBG	
		cout << "Post-standardization input" << endl;
		util_printVec(x, inputDims_u);
#endif	
		// Precompute
		mulMatVec((ll*)qW1_transp_u, x, wRank_u, inputDims_u, out_wRank_u);
		mulMatVec((ll*)qW2_transp_u, out_wRank_u, hiddenDims_u, wRank_u, pre);

		mulMatVec((ll*)qU1_transp_u, h, uRank_u, hiddenDims_u, out_uRank_u);
		mulMatVec((ll*)qU2_transp_u, out_uRank_u, hiddenDims_u, uRank_u, out_hiddenDims_u);

		addVecs(pre, out_hiddenDims_u, hiddenDims_u, pre);

		divVecScal(pre, q_u, hiddenDims_u, pre);

#ifdef DBG
		cout << "Pre at t=" << t << endl;
		util_printVec(pre, hiddenDims_u);
#endif

		// Create h_, z
		addVecs(pre, (ll*)qB_h_u, hiddenDims_u, h_);
		addVecs(pre, (ll*)qB_g_u, hiddenDims_u, z);

		UPDATE_NL(h_, hiddenDims_u, q_times_I_u, h_);
		divVecScal(h_, q_u, hiddenDims_u, h_);

		GATE_NL(z, hiddenDims_u, q_times_I_u, z);
		divVecScal(z, q_u, hiddenDims_u, z);
#ifdef DBG
		cout << "h_ at t=" << t << endl;
		util_printVec(h_, hiddenDims_u);

		cout << "z at t=" << t << endl;
		util_printVec(z, hiddenDims_u);
#endif
		// Create new h
		mulVecs(z, h, hiddenDims_u, h);

		subVecs((ll*)I_u_vec, z, hiddenDims_u, out_hiddenDims_u);
		mulVecScal((ll*)out_hiddenDims_u, I_times_zeta_u, hiddenDims_u, out_hiddenDims_u);
		addVecs(out_hiddenDims_u, (ll*)I_squared_times_nu_u_vec, hiddenDims_u, out_hiddenDims_u);
		mulVecs(out_hiddenDims_u, h_, hiddenDims_u, out_hiddenDims_u);
		divVecScal(out_hiddenDims_u, I_u, hiddenDims_u, out_hiddenDims_u);
		addVecs(h, out_hiddenDims_u, hiddenDims_u, h);
		divVecScal(h, I_u, hiddenDims_u, h);
#ifdef MOTE_PROFILE
		//hal_printf("e");
		CPU_GPIO_SetPinState(0, false);
#endif
#ifdef DBG
		cout << "h at t=" << t << endl;
		util_printVec(h, hiddenDims_u);
#endif
	}

	// Classify
	mulMatVec((ll*)qFC_Weight_u, h, numClasses_u, hiddenDims_u, out_numClasses_u);
	addVecs(out_numClasses_u, (ll*)qFC_Bias_u, numClasses_u, out_numClasses_u);
#ifdef DBG
	cout << "Classification output:" << endl;
	util_printVec(out_numClasses_u, numClasses_u);
#endif
#ifndef MOTE
	//Print decision to csv file
	string outstr;
	for(int c = 0; c < numClasses_u -1 ; c++)
		outstr += strBuild(out_numClasses_u[c], ',');
	outstr += strBuild(out_numClasses_u[numClasses_u -1], '\n');
	outfile << outstr;
#endif
	if(out_numClasses_u[0]>out_numClasses_u[1])
		return 0;
	else
		return 1;
}

static uint strided_data[timeSteps_u*inputDims_u];

bool fastgrnn_driver(uint* data){
	// Reshape data
	//int (&data2D)[orig_num_steps][inputDims_u] = *reinterpret_cast<int (*)[orig_num_steps][inputDims_u]>(&data);

	// Create strides
	for(int dest_index=0, src_index=0, i=0; i<timeSteps_u; dest_index += inputDims_u, src_index += stride_u, i++)
		copy(data + src_index, data + src_index + inputDims_u, strided_data + dest_index);

	// Call upper_fastgrnn
	int dec = upper_fastgrnn((uint*)strided_data);
	if(dec==0)
		return true;
	else
		return false;
}

static uint test_input[timeSteps_u][inputDims_u] = {0};

void run_test_upper(){
	int size = sizeof(qW1_transp_u) + sizeof(qFC_Bias_u) + sizeof(qW2_transp_u) + sizeof(qU2_transp_u) + sizeof(qFC_Weight_u) + sizeof(qU1_transp_u) + sizeof(qB_g_u) + sizeof(qB_h_u) + sizeof(q_u) + sizeof(I_u) + sizeof(mean_u) + sizeof(stdev_u) + sizeof(I_u_vec) + sizeof(q_times_I_u) + sizeof(I_squared_times_nu_u_vec) + sizeof(I_times_zeta_u);
	
#ifndef MOTE
	cout << "Model size: " << size/1000 << " KB" << endl << endl;
#endif
#ifdef MOTE_PROFILE
	CPU_GPIO_EnableOutputPin(0, false); //J11-3
	CPU_GPIO_EnableOutputPin(1, false); //J11-4
#endif
#ifndef MOTE
	// Initialize output file
	outfile.open("out_c++.csv");
#endif
	for(int d=0; d < numData_u; d ++)
	{
		util_slice3D((uint*) test_inputs_u, (uint*) test_input, d, timeSteps_u, inputDims_u);

#ifdef MOTE_PROFILE
		// Profile latency per bag (second in V1)
		CPU_GPIO_SetPinState(1, true);
#endif

#ifdef DBG
		util_printMatrix((uint*) test_input, timeSteps, inputDims_u);
#endif
		// Call upper_fastgrnn
		upper_fastgrnn((uint*)test_input);

#ifdef MOTE_PROFILE
		CPU_GPIO_SetPinState(1, false);
#endif
	}
#ifndef MOTE
	outfile.close();
#else
	hal_printf("Test complete (upper tier).\n");
#endif
}
#ifndef MOTE
int main(){
	run_test_upper();
}
#endif