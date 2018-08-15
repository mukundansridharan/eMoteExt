/*
 * GlobalTime.h
 *
 *  Created on: April 30, 2015
 *      Author: Bora Karaoglu
 */


#ifndef GLOBALTIME_H_
#define GLOBALTIME_H_

#include <tinyhal.h>

#define MAX_SAMPLES 8
#define MAX_NBR 12
#define INVALID_TIMESTAMP 0x7FFFFFFFFFFFFFFF
#define INVALID_NBR_ID 0xFFFF
#define MAXRangeUINT64 0xFFFFFFFFFFFFFFFF
#define UnknownRelativeFreq 255

#define MIN_NUM_ELEMENTS_FOR_TIME_CALCULATION 2
#define TIME_WALK_STEP (double)(200) //BK: This is the search step in Ticks used in FAST_RECOVERY2

typedef UINT8 NeighborIndex_t;
static const NeighborIndex_t c_bad_nbrIndex = 255;

struct TSSamples {
	UINT16 nbrID;
	double recordedTime[MAX_SAMPLES];
	double offsetBtwNodes[MAX_SAMPLES];
	bool isused[MAX_SAMPLES];
	UINT8 lastTimeIndex;
	UINT8 numSamples;
	double relativeFreq; // y *f = x x is the local clock clcok
	double y_intercept; // y *f = x x is the local clock clcok
	double additional_y_intercept_offset;
	//INT64 Last_TS_localtime;
	//INT64 Last_Adjust_localtime;
		//INT64 First_TS_localtime;
	//INT64 AvgUpdate_TS_localtime;
	//INT64 SkewCompute_localtime;
};

class Regression {
private:
	UINT8 nbrCount;

	/*void Compute(UINT16 nbr){
		INT64 newLocalAverage, newOffsetAverage, localSum, localAverageRest;
		INT64 offsetSum, offsetAverageRest;
		INT64 latestLocalTime, earliestLocalTime;
		INT64 simpleOffsetChangeSum, simpleCurrentOffsetDiff , simpleCurrentLocalTimeDiff;

		UINT16 nbrIndex = FindNeighbor(nbr);
		if(nbrIndex==c_bad_nbrIndex){
			return;
		}
		UINT64 *nbrLocalTimes = samples[nbrIndex].recordedTime;
		INT64 *nbrOffset = samples[nbrIndex].offsetBtwNodes;
		UINT8 numSamples = samples[nbrIndex].numSamples;


		//We use a rough approximation first to avoid time overflow errors. The idea
		//is that all times in the table should be relatively close to each other.
		//i is the first nonempty entry in the table
	    UINT8 i,j;
	    for(i = 0; i < MAX_SAMPLES && nbrLocalTimes[i] == INVALID_TIMESTAMP; ++i){
	    	j = 0;
	    }


	    UINT8 firstIdx, lastIdx;
	    firstIdx = lastIdx = i;
	    //hal_printf("fIdx init to %u\n", firstIdx);
	    newLocalAverage = nbrLocalTimes[i];
		newOffsetAverage = nbrOffset[i];

		localSum = 0;
		localAverageRest = 0;
		offsetSum =0;
		offsetAverageRest =0;
		simpleOffsetChangeSum=0;
		while( ++i < MAX_SAMPLES ) {
			if( nbrLocalTimes[i] != INVALID_TIMESTAMP ) {
				//This only works because C ISO 1999 defines the sign for modulo the
				//same as for the Dividend! This approximates dividing using only integers.

				localSum += (INT64)(nbrLocalTimes[i] - newLocalAverage) / numSamples;
				localAverageRest += (nbrLocalTimes[i] - newLocalAverage) % numSamples;
				offsetSum += (INT64)(nbrOffset[i] - newOffsetAverage) / numSamples;
				offsetAverageRest += (nbrOffset[i] - newOffsetAverage) % numSamples;

			}
		}
		newLocalAverage += localSum + localAverageRest / numSamples;
		newOffsetAverage += offsetSum + offsetAverageRest / numSamples;

		//Find latestLocalTime and earliestLocalTime in the buffer
		if (numSamples >= 2) {
			latestLocalTime = nbrLocalTimes[lastIdx];
			earliestLocalTime = nbrLocalTimes[firstIdx];
		    for (i = 0; i < MAX_SAMPLES; i++) {
		    	if( nbrLocalTimes[i] != INVALID_TIMESTAMP ) {
		    		if (nbrLocalTimes[i] > latestLocalTime) {
		    			latestLocalTime = nbrLocalTimes[i];
		    			lastIdx = i;
		    		}
		    		if (nbrLocalTimes[i] < earliestLocalTime) {
		    			earliestLocalTime = nbrLocalTimes[i];
		    			firstIdx = i;
		    		}
		    	}
		    }
		}

		//samples[nbrIndex].avgSkew  = (float)(nbrOffset[lastIdx] - nbrOffset[firstIdx]) /
		//		(float)(nbrLocalTimes[lastIdx]- nbrLocalTimes[firstIdx]);

		simpleOffsetChangeSum=0;
		for (int i =firstIdx; i<=lastIdx ; i++){
			//simpleCurrentLocalTimeDiff =nbrLocalTimes[(i+1) % MAX_SAMPLES] - nbrLocalTimes[i]
			simpleCurrentOffsetDiff = nbrOffset[(i+1) % MAX_SAMPLES] - nbrOffset[i];
			simpleOffsetChangeSum += simpleCurrentOffsetDiff;
			//samples[nbrIndex].relativeFreq += nbrLocalTimes[i]/(nbrLocalTimes[i]-nbrOffset[(i+1) % MAX_SAMPLES])
		}
		samples[nbrIndex].avgSkew  = (float)(simpleOffsetChangeSum) /
						(float)(nbrLocalTimes[lastIdx]- nbrLocalTimes[firstIdx]);


		samples[nbrIndex].relativeFreq = 0;
		for (i = firstIdx; i != lastIdx ; i = ((i+1) % MAX_SAMPLES)){
			if( nbrLocalTimes[i] != INVALID_TIMESTAMP ) {
				for  (j = ((i+1) % MAX_SAMPLES); j != firstIdx  ; j = ((j+1) % MAX_SAMPLES)) {
					if( nbrLocalTimes[j] != INVALID_TIMESTAMP ) {
						if(nbrLocalTimes[i] < nbrLocalTimes[j] ) {
							simpleCurrentLocalTimeDiff = nbrLocalTimes[(i+1) % MAX_SAMPLES] - nbrLocalTimes[i];
							simpleCurrentOffsetDiff = nbrOffset[(i+1) % MAX_SAMPLES] - nbrOffset[i];
							samples[nbrIndex].relativeFreq += ((float) (simpleCurrentLocalTimeDiff - simpleCurrentOffsetDiff)) /(float) simpleCurrentLocalTimeDiff;
						}
						break;
					}
				}
			}
		}
		samples[nbrIndex].relativeFreq = samples[nbrIndex].relativeFreq / (numSamples-1);

		if(samples[nbrIndex].relativeFreq > 1.2 || samples[nbrIndex].relativeFreq < 0.8){
			ASSERT_SP(0);
		}

		samples[nbrIndex].recordedTimeAvg = newLocalAverage;
		samples[nbrIndex].offsetAvg = newOffsetAverage;
		//hal_printf("GlobalTime: Avg Drift: %lu, Avg Skew: %f \n",samples[nbrIndex].offsetAvg, samples[nbrIndex].avgSkew);
	}*/

	void Compute(UINT16 nbr){
		NeighborIndex_t nbrIndex = FindNeighbor(nbr);
		if(nbrIndex==c_bad_nbrIndex){
			return;
		}
		double *nbrLocalTimes = samples[nbrIndex].recordedTime;
		double *nbrOffset = samples[nbrIndex].offsetBtwNodes;
		UINT8 numSamples = samples[nbrIndex].numSamples;

		if (numSamples < 2) {
			return;
		}

	    NeighborIndex_t firstIdx, lastIdx, i;
	    firstIdx = lastIdx = i = 1;
	    double latestLocalTime, earliestLocalTime;
		//Find latestLocalTime and earliestLocalTime in the buffer
		latestLocalTime = nbrLocalTimes[lastIdx];
		earliestLocalTime = nbrLocalTimes[firstIdx];
		for (i = 0; i < MAX_SAMPLES; i++) {
			//if( nbrLocalTimes[i] != INVALID_TIMESTAMP ) {
			if( samples[nbrIndex].isused[i]){
				if (nbrLocalTimes[i] > latestLocalTime) {
					latestLocalTime = nbrLocalTimes[i];
					lastIdx = i;
				}
				if (nbrLocalTimes[i] < earliestLocalTime) {
					earliestLocalTime = nbrLocalTimes[i];
					firstIdx = i;
				}
			}
		}

		double SSxy = 0;
		double SSxx = 0;
		double sum_y = 0;
		double sum_x = 0;
		//samples[nbrIndex].relativeFreq = 0;
		for (i = 0; i < MAX_SAMPLES; i++) {
			//if( nbrLocalTimes[i] != INVALID_TIMESTAMP ) {
			if( samples[nbrIndex].isused[i]){
				sum_y += nbrLocalTimes[i];
				sum_x += nbrLocalTimes[i] - nbrOffset[i] ;
			}
		}
		for (i = 0; i < MAX_SAMPLES; i++) {
			//if( nbrLocalTimes[i] != INVALID_TIMESTAMP ) {
			if( samples[nbrIndex].isused[i]){
				SSxx += ((nbrLocalTimes[i] - nbrOffset[i])  - sum_x/((double)numSamples)) * ((nbrLocalTimes[i] - nbrOffset[i])  - sum_x/((double)numSamples));
				SSxy += ((nbrLocalTimes[i] - nbrOffset[i])  - sum_x/((double)numSamples)) * ((nbrLocalTimes[i])  - sum_y/((double)numSamples));
			}
		}
		samples[nbrIndex].relativeFreq = SSxy/SSxx;
		samples[nbrIndex].y_intercept = sum_y/((double)numSamples) - sum_x/((double)numSamples) * samples[nbrIndex].relativeFreq;

		samples[nbrIndex].additional_y_intercept_offset = 0;

		if(samples[nbrIndex].relativeFreq < 0.9 || samples[nbrIndex].relativeFreq > 1.1){ //Reject last received samples
			UINT8 previoustolastindex;
			if(samples[nbrIndex].lastTimeIndex == 0) 	previoustolastindex = MAX_SAMPLES - 1;
			else 										previoustolastindex = samples[nbrIndex].lastTimeIndex -1;
			samples[nbrIndex].recordedTime[samples[nbrIndex].lastTimeIndex] = samples[nbrIndex].recordedTime[previoustolastindex];
			samples[nbrIndex].offsetBtwNodes[samples[nbrIndex].lastTimeIndex] = samples[nbrIndex].offsetBtwNodes[previoustolastindex];
		}

	}



public:
	TSSamples samples[MAX_NBR];

	bool IsTableFull(){
		for(int i=0; i < MAX_NBR; i++){
			if (samples[i].nbrID == INVALID_NBR_ID){
				return false;
			}
		}
		return true;
	}

	Regression(){
		Init();
	};

	NeighborIndex_t FindNeighbor(UINT16 nbr){
		for(int i=0; i < MAX_NBR; i++){
			if (samples[i].nbrID == nbr){
				return i;
			}
		}
		return c_bad_nbrIndex;
	}

	NeighborIndex_t InsertNbrID(UINT16 nbr){
		for(int i=0; i < MAX_NBR; i++){
			if (samples[i].nbrID == INVALID_NBR_ID){
				samples[i].nbrID = nbr;
				++nbrCount;
				return i;
			}
		}
		return c_bad_nbrIndex;
	}

	UINT8 NumberOfRecordedElements(UINT16 nbr){
		NeighborIndex_t nbrIndex = FindNeighbor(nbr);
		if (nbrIndex >= MAX_NBR) return 0;
		return(samples[nbrIndex].numSamples);
	};
	void Insert(UINT16 nbr,UINT64 nbr_ltime, INT64 nbr_loffset){
		NeighborIndex_t nbrIndex = FindNeighbor(nbr);
		UINT64 timeDifference;
		UINT8 previndex;
		//Add new neighbor if not found
		if (nbrIndex == c_bad_nbrIndex){
			nbrIndex = InsertNbrID(nbr);
			//No space in regression table
			if (nbrIndex == c_bad_nbrIndex) {
				return;
			}
		}
		samples[nbrIndex].nbrID = nbr;
		previndex = samples[nbrIndex].lastTimeIndex;
		//if( samples[nbrIndex].recordedTime[samples[nbrIndex].lastTimeIndex] != INVALID_TIMESTAMP
		if( samples[nbrIndex].isused[samples[nbrIndex].lastTimeIndex]
		&&  samples[nbrIndex].recordedTime[samples[nbrIndex].lastTimeIndex] >= nbr_ltime
		){ // Discard out of orderly received time stamps // Consider adding it in between.
			CleanNbrwithIndex(nbrIndex);
		}
//		else{
//			if(nbr_loffset < 0){
//				timeDifference = Local2NeighborTime(nbr, nbr_ltime + (UINT64)(nbr_loffset*(-1)));
//			}
//			else{
//				timeDifference = Local2NeighborTime(nbr, nbr_ltime  - nbr_loffset);
//			}
//			if(timeDifference > nbr_ltime && timeDifference - nbr_ltime > 40000  ){
//				CleanNbrwithIndex(nbrIndex);
//			}
//			else if( nbr_ltime > timeDifference && nbr_ltime - timeDifference > 40000  ){
//				CleanNbrwithIndex(nbrIndex);
//			}
//		}
		samples[nbrIndex].lastTimeIndex++;
		samples[nbrIndex].lastTimeIndex = samples[nbrIndex].lastTimeIndex % MAX_SAMPLES;
		samples[nbrIndex].isused[samples[nbrIndex].lastTimeIndex] = true;
		samples[nbrIndex].recordedTime[samples[nbrIndex].lastTimeIndex] = nbr_ltime;
		samples[nbrIndex].offsetBtwNodes[samples[nbrIndex].lastTimeIndex] = nbr_loffset;
		if(samples[nbrIndex].numSamples < MAX_SAMPLES){
			samples[nbrIndex].numSamples++;
		}
		Compute(nbr);
	}
	void Clean(UINT16 nbr){
		NeighborIndex_t nbrIndex = FindNeighbor(nbr);
		CleanNbrwithIndex(nbrIndex);

	}
	void CleanNbrwithIndex(NeighborIndex_t nbrIndex){
		if(nbrIndex >= 0 && nbrIndex < MAX_NBR){
			samples[nbrIndex].nbrID = INVALID_NBR_ID;
			--nbrCount;
			samples[nbrIndex].lastTimeIndex = MAX_SAMPLES;
			samples[nbrIndex].numSamples = 0;
			samples[nbrIndex].relativeFreq = 0;
			samples[nbrIndex].y_intercept = 0;
			samples[nbrIndex].additional_y_intercept_offset = 0;
			for(int i=0; i< MAX_SAMPLES; i++){
				samples[nbrIndex].recordedTime[i] =INVALID_TIMESTAMP;
				samples[nbrIndex].offsetBtwNodes[i] = 0;
				samples[nbrIndex].isused[i] = false;
			}
		}
	}

	UINT64 Local2NeighborTime(UINT16 nbr, const UINT64& curtime);
	UINT64 Neighbor2LocalTime(UINT16 nbr, const UINT64& nbrtime);


	void Init(){
		nbrCount=0;
		for (NeighborIndex_t ii=0; ii< MAX_NBR; ii++){
			samples[ii].nbrID = INVALID_NBR_ID;
			CleanNbrwithIndex(ii);
		}
	}

	float FindRelativeFreq(UINT16 nbr) {
		float rv = UnknownRelativeFreq;
		NeighborIndex_t nbrIndex = FindNeighbor(nbr);
		if(nbrIndex==c_bad_nbrIndex){
			return rv;
		}
		return(samples[nbrIndex].relativeFreq);
	};

	UINT64 LastRecordedTime(UINT16 nbr){
		NeighborIndex_t nbrIndex = FindNeighbor(nbr);
		if (nbrIndex == c_bad_nbrIndex) return (0);
		return (samples[nbrIndex].recordedTime[samples[nbrIndex].lastTimeIndex]);
	};
};



class GlobalTime {
public:
	static INT64 offset;
	static float skew;
	Regression regressgt2;
	static BOOL synced;
	static UINT16 leader;

	GlobalTime(){};
	void Init();

	UINT64 Local2NeighborTime(UINT16 nbr, const UINT64& curtime);
	UINT64 Neighbor2LocalTime(UINT16 nbr, const UINT64& nbrtime);

	void UnsuccessfulTransmission(UINT16 nbr){
		NeighborIndex_t nbrIndex = regressgt2.FindNeighbor(nbr);
		if(nbrIndex == c_bad_nbrIndex) {
			ASSERT_SP(0);
			return;
		}
		if(regressgt2.samples[nbrIndex].additional_y_intercept_offset == 0){
			regressgt2.samples[nbrIndex].additional_y_intercept_offset = (double)(-1)*TIME_WALK_STEP;
		}
		else if(regressgt2.samples[nbrIndex].additional_y_intercept_offset == (double)(-1)*TIME_WALK_STEP){
			regressgt2.samples[nbrIndex].additional_y_intercept_offset =   TIME_WALK_STEP;
		}
		else if(regressgt2.samples[nbrIndex].additional_y_intercept_offset ==  TIME_WALK_STEP){
			regressgt2.samples[nbrIndex].additional_y_intercept_offset = double(-2) * TIME_WALK_STEP;
		}
		else if(regressgt2.samples[nbrIndex].additional_y_intercept_offset == double(-2) * TIME_WALK_STEP){
			regressgt2.samples[nbrIndex].additional_y_intercept_offset =  double(2.5) * TIME_WALK_STEP;
		}
		else{
			regressgt2.samples[nbrIndex].additional_y_intercept_offset = 0;
		}
		return;
	}

	bool IsNeighborTimeAvailable(UINT16 nbr){
		if(regressgt2.NumberOfRecordedElements(nbr) >= MIN_NUM_ELEMENTS_FOR_TIME_CALCULATION) return true;
		else return false;
	};
	// OBSOLETE AND UNTESTED
	UINT64 Read(){
		UINT64 rtn;
		rtn = HAL_Time_CurrentTime();
		if(leader!=0x0000 || leader!=0xFFFF){ //Bk: if(leader!=MF_NODE_ID || leader!=0xFFFF){
			return Local2NeighborTime(leader,rtn);
		}
		return  rtn;
	};
};


void GlobalTime::Init(){
	regressgt2.Init();
	leader = 0xFFFF;
	offset=0;
}


UINT64 GlobalTime::Neighbor2LocalTime(UINT16 nbr, const UINT64& nbrTime){
	return regressgt2.Neighbor2LocalTime(nbr, nbrTime);
}

UINT64 GlobalTime::Local2NeighborTime(UINT16 nbr, const UINT64& curtime){
	return regressgt2.Local2NeighborTime(nbr, curtime);
}

UINT64 Regression::Neighbor2LocalTime(UINT16 nbr, const UINT64& nbrTime){
	if (NumberOfRecordedElements(nbr) < 2) return(HAL_Time_CurrentTicks());
	NeighborIndex_t nbrIndex = FindNeighbor(nbr);
	if(nbrIndex == c_bad_nbrIndex) {
		ASSERT_SP(0);
		return 0;
	}
	/*UINT64 lastrecordedTime = regressgt2.samples[nbrIndex].recordedTime[regressgt2.samples[nbrIndex].lastTimeIndex];
	UINT64 periodlength;
	bool negativeperiod = FALSE;
	//Check roll over
	if(nbrTime < lastrecordedTime) {
		if( lastrecordedTime - nbrTime > ((MAXRangeUINT64)/2)) { //Roll Over
			periodlength = (MAXRangeUINT64 - lastrecordedTime) + nbrTime;
		}
		else{ // Negative Interval
			periodlength = lastrecordedTime - nbrTime;
			negativeperiod = TRUE;
		}
	}
	else { // No rollover no negative
		periodlength = nbrTime - lastrecordedTime;
	}

	// Calculate the time
	lastrecordedTime = lastrecordedTime - regressgt2.samples[nbrIndex].offsetBtwNodes[regressgt2.samples[nbrIndex].lastTimeIndex];
	if (negativeperiod) lastrecordedTime = lastrecordedTime - (((float) periodlength)  * regressgt2.samples[nbrIndex].relativeFreq);
	else lastrecordedTime = lastrecordedTime + (((float) periodlength)  * regressgt2.samples[nbrIndex].relativeFreq);
	return (lastrecordedTime);*/
	return ( ( (double)nbrTime - samples[nbrIndex].y_intercept - samples[nbrIndex].additional_y_intercept_offset)/samples[nbrIndex].relativeFreq );
}

UINT64 Regression::Local2NeighborTime(UINT16 nbr, const UINT64& curtime){
	if (NumberOfRecordedElements(nbr) < 2) return(0);
	NeighborIndex_t nbrIndex = FindNeighbor(nbr);
	if(nbrIndex == c_bad_nbrIndex) {
		ASSERT_SP(0);
		return 0;
	}
	/*//UINT64 curtime = HAL_Time_CurrentTime();
	UINT8 nbrIndex = regressgt2.FindNeighbor(nbr);
	UINT64 periodlength;
	bool negativeperiod = FALSE;
	UINT64 lastlocalTime;

	// Get last sample
	if (regressgt2.samples[nbrIndex].offsetBtwNodes[regressgt2.samples[nbrIndex].lastTimeIndex] > 0) {
		lastlocalTime = regressgt2.samples[nbrIndex].recordedTime[regressgt2.samples[nbrIndex].lastTimeIndex] - (UINT64) regressgt2.samples[nbrIndex].offsetBtwNodes[regressgt2.samples[nbrIndex].lastTimeIndex];
	}
	else {
		lastlocalTime = regressgt2.samples[nbrIndex].recordedTime[regressgt2.samples[nbrIndex].lastTimeIndex] + (UINT64)((regressgt2.samples[nbrIndex].offsetBtwNodes[regressgt2.samples[nbrIndex].lastTimeIndex])*-1);
	}

	//Check roll over
	if(curtime < lastlocalTime) {
		if( lastlocalTime - curtime > ((MAXRangeUINT64)/2)) { //Roll Over
			periodlength = (MAXRangeUINT64 - lastlocalTime) + curtime;
		}
		else{ // Negative Interval
			periodlength = lastlocalTime - curtime;
			negativeperiod = TRUE;
		}
	}
	else { // No rollover no negative
		periodlength = curtime - lastlocalTime;
	}

	// Calculate the time
	lastlocalTime = regressgt2.samples[nbrIndex].recordedTime[regressgt2.samples[nbrIndex].lastTimeIndex];
	if (negativeperiod) lastlocalTime = lastlocalTime - (((float) periodlength) / regressgt2.samples[nbrIndex].relativeFreq);
	else lastlocalTime = lastlocalTime + (((float) periodlength) / regressgt2.samples[nbrIndex].relativeFreq);
	return (lastlocalTime);*/
	return ( samples[nbrIndex].relativeFreq * (double)curtime + samples[nbrIndex].y_intercept + samples[nbrIndex].additional_y_intercept_offset);
}


#endif //GLOBALTIME_H_
