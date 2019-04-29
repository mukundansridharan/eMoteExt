// SMONative.cpp : Defines the entry point for the console application.
//
#include <cstdlib>
#include "stdafx.h"
#include <cstdint>
#include <cmath>
#include <iostream>
#include "SMONative.h"
using namespace std;

void SMONative::fixedFeature(float floatFeature[], bool flag, uint32_t normFeature[])
{
	uint32_t rval[nFeatures];

	if (flag)
	{
		for (int i = 0; i < nFeatures; i++)
		{
			normFeature[i] = floatToFixed((floatFeature[i] - minClass[i]) * scaleClass[i], 1 << 20);
		}

	}
	else
	{
		for (int i = 0; i < nFeatures; i++)
		{
			normFeature[i] = floatToFixed((floatFeature[i] - minCount[i]) * scaleCount[i], 1 << 19);
		}
	}
}

int64_t SMONative::mult(int32_t a, int32_t b)
{
	return (((int64_t)a * (int64_t)b) >> 20);
}

float SMONative::pukKernel(int32_t supportVector[], uint32_t feature[])
{
	int32_t sum = 0;
	for (int i = 0; i < nFeatures; i++)
	{
		sum += mult(feature[i] - supportVector[i], feature[i] - supportVector[i]);
	}
	float temp = fixedToFloat(sum, 1 << 19);
	float returnVal = (float)pow(1 + 4.0 * temp * sqrt(pow(2.0, 1.0 / omega) - 1) / pow(sigma, 2.0), -omega);
	return returnVal;
}

float SMONative::rbfKernel(int32_t supportVector[], uint32_t feature[])
{
	int64_t sum = 0;
	for (int i = 0; i < nFeatures; i++)
	{
		sum += mult(feature[i] - supportVector[i], feature[i] - supportVector[i]);
	}
	float temp = fixedToFloat(sum, 1 << 20);
	float returnVal = (float)exp(-gamma * temp);
	return (returnVal);
}

int32_t SMONative::floatToFixed(float val, int convert)
{
	float intVal = val * convert;
	intVal += (intVal >= 0) ? 0.5f : -0.5f;
	return (int32_t)intVal;
}

float SMONative::fixedToFloat(int64_t val, int convert)
{
	return (float)val / convert;
}

float SMONative::predictCount(float features[])
{
	double decision = bias;
	uint32_t intFeat[nFeatures];
	fixedFeature(features, false, intFeat);
	int32_t SV[nFeatures];
	for (int i = 0; i < nSV; i++)
	{
		for (int j = i*nFeatures, k = 0; j < (i + 1)*nFeatures; j++, k++)
		{
			SV[k] = fixedCount[j];
		}
		decision += (W[i] * pukKernel(SV, intFeat));
	}
	return (float)round(decision);
}

double SMONative::predictClass(float features[])
{
	double decision = biasC;
	uint32_t intFeat[nFeatures];
	fixedFeature(features, true, intFeat);
	int32_t SV[nFeatures];
	for (int i = 0; i < nSVc; i++)
	{
		for (int j = i * nFeatures, k = 0; j < (i + 1) * nFeatures; j++, k++)
		{
			SV[k] = fixedClass[j];
		}
		
		decision += (Wc[i] * rbfKernel(SV, intFeat));
	}
	/*if (decision>0)
	{
	decision = 1;
	}
	else
	{
	decision = 0;
	}
	return (int32_t)decision;*/
	return decision;
}
