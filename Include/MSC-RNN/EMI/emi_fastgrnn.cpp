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

inline int emi_rnn(uint* test_input){	
	ll h[hiddenDims_l] = {0};
	ll out_wRank_l[wRank_l] = {0};
	ll out_uRank_l[uRank_l] = {0};
	ll out_hiddenDims_l[hiddenDims_l] = {0};
	ll out_numClasses_l[numClasses_l] = {0};
	for(int t=0; t<timeSteps_l; t++){
#ifdef MOTE_PROFILE
		// Profile latency per timestep			
		//hal_printf("b");
		CPU_GPIO_SetPinState(0, true);
#endif
		uint x_int[inputDims_l] = {0};
		util_slice2D(test_input, x_int, t, inputDims_l);

		ll x[inputDims_l] = {};

		copyUIntVecToLL(x_int, x, inputDims_l);
#ifdef DBG
		cout << "Current input array in ll" << endl;
		util_printVec(x, inputDims_l);
#endif	
		stdScaleInput(x, (ll*)mean_l, (ll*)stdev_l, I_l, inputDims_l, x);
#ifdef DBG	
		cout << "Post-standardization input" << endl;
		util_printVec(x, inputDims_l);
#endif	
		// Precompute
		ll pre[hiddenDims_l] = {0};
		mulMatVec((ll*)qW1_transp_l, x, wRank_l, inputDims_l, out_wRank_l);
		mulMatVec((ll*)qW2_transp_l, out_wRank_l, hiddenDims_l, wRank_l, pre);

		mulMatVec((ll*)qU1_transp_l, h, uRank_l, hiddenDims_l, out_uRank_l);
		mulMatVec((ll*)qU2_transp_l, out_uRank_l, hiddenDims_l, uRank_l, out_hiddenDims_l);

		addVecs(pre, out_hiddenDims_l, hiddenDims_l, pre);

		divVecScal(pre, q_l, hiddenDims_l, pre);

#ifdef DBG
		cout << "Pre at t=" << t << endl;
		util_printVec(pre, hiddenDims_l);
#endif

		// Create h_, z
		ll h_[hiddenDims_l] = {0};
		ll z[hiddenDims_l] = {0};

		addVecs(pre, (ll*)qB_h_l, hiddenDims_l, h_);
		addVecs(pre, (ll*)qB_g_l, hiddenDims_l, z);

		UPDATE_NL(h_, hiddenDims_l, q_times_I_l, h_);
		divVecScal(h_, q_l, hiddenDims_l, h_);

		GATE_NL(z, hiddenDims_l, q_times_I_l, z);
		divVecScal(z, q_l, hiddenDims_l, z);
#ifdef DBG
		cout << "h_ at t=" << t << endl;
		util_printVec(h_, hiddenDims_l);

		cout << "z at t=" << t << endl;
		util_printVec(z, hiddenDims_l);
#endif
		// Create new h
		mulVecs(z, h, hiddenDims_l, h);

		subVecs((ll*)I_l_vec, z, hiddenDims_l, out_hiddenDims_l);
		mulVecs(out_hiddenDims_l, h_, hiddenDims_l, out_hiddenDims_l);

		addVecs(h, out_hiddenDims_l, hiddenDims_l, h);
		divVecScal(h, I_l, hiddenDims_l, h);
#ifdef MOTE_PROFILE
		//hal_printf("e");
		CPU_GPIO_SetPinState(0, false);
#endif
#ifdef DBG
		cout << "h at t=" << t << endl;
		util_printVec(h, hiddenDims_l);
#endif
	}

	// Classify
	mulMatVec((ll*)qFC_Weight_l, h, numClasses_l, hiddenDims_l, out_numClasses_l);
	addVecs(out_numClasses_l, (ll*)qFC_Bias_l, numClasses_l, out_numClasses_l);
#ifdef DBG
	cout << "Classification output:" << endl;
	util_printVec(out_numClasses_l, numClasses_l);
#endif
#ifndef MOTE
	//Print decision to csv file
	string outstr;
	for(int c = 0; c < numClasses_l -1 ; c++)
		outstr += strBuild(out_numClasses_l[c], ',');
	outstr += strBuild(out_numClasses_l[numClasses_l -1], '\n');
	outfile << outstr;
#endif
	if(out_numClasses_l[0]>out_numClasses_l[1])
		return 0;
	else
		return 1;
}

bool emi_driver(uint* data){
	// Reshape data
	//int (&data2D)[orig_num_steps][inputDims_l] = *reinterpret_cast<int (*)[orig_num_steps][inputDims_l]>(&data);
	int maxconsectargets = 0;
	bool detect_in_bag = false;
	// Create instances and run EMI
	for(int start = 0, i=0; i<numInstances; start += instStride, i++){
		int end;
		if(i==numInstances-1)
			// Correction for last iteration
			end = orig_num_steps;
		else
			end = start + timeSteps_l;

		uint next_inst[timeSteps_l][inputDims_l] = {0};
		extract_instance(data, (uint*)next_inst, start, end, inputDims_l);
		
		// Call emi_rnn
		int inst_dec = emi_rnn((uint*)next_inst);
		//hal_printf("%d", inst_dec);
		// Bag-level detection logic
		if(inst_dec==0)
			maxconsectargets = 0;
		else
			maxconsectargets++;
		if(maxconsectargets>=k)
			return true;
	}
	return false;
}

void run_test_lower(){
	int size = sizeof(qW1_transp_l) + sizeof(qFC_Bias_l) + sizeof(qW2_transp_l) + sizeof(qU2_transp_l) + sizeof(qFC_Weight_l) + sizeof(qU1_transp_l) + sizeof(qB_g_l) + sizeof(qB_h_l) + sizeof(q_l) + sizeof(I_l) + sizeof(mean_l) + sizeof(stdev_l) + sizeof(I_l_vec) + sizeof(q_times_I_l);
	
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
	for(int d=0; d < numData_l; d ++)
	{
		uint test_input[timeSteps_l][inputDims_l] = {0};
		util_slice3D((uint*) test_inputs_l, (uint*) test_input, d, timeSteps_l, inputDims_l);

#ifdef MOTE_PROFILE
		// Profile latency per bag (second in V1)
		if(d%numInstances==0)
			CPU_GPIO_SetPinState(1, true);
#endif

#ifdef DBG
		util_printMatrix((uint*) test_input, timeSteps_l, inputDims_l);
#endif
		// Call emi_rnn
		emi_rnn((uint*)test_input);

#ifdef MOTE_PROFILE
		if(d%numInstances==numInstances-1)
			CPU_GPIO_SetPinState(1, false);
#endif
	}
#ifndef MOTE
	outfile.close();
#else
	hal_printf("Test complete (lower).\n");
#endif
}
#ifndef MOTE
int main(){
	run_test_lower();
}
#endif