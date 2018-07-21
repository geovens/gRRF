
#include "Node.h"
#include "RandomTree.h"
#include "Data.h"
#include "Linker.h"
#include "Function.h"
#include "stdafx.h"
//#include "flowgraph/flowgraph.h"

Node::Node()
{
	N = 0;
	ABC = NULL;
	ABC_Fit = NULL;
	FuncRandomABC = NULL;
	Left = NULL;
	Right = NULL;
	Trained = 0;
}

int Node::Split(featuretype* abc, featuretype* feature_temp_store, bool setsplitflags)
{
	ThisDataPointers->ChildrenN[0] = 0;
	ThisDataPointers->ChildrenN[1] = 0;
	
	//featuretype* feature = new featuretype[ThisData->D];
	valuetype value;
	if (setsplitflags)
	{
		ThisDataPointers->NewSplitFlags();
		ThisDataPointers->FastInit();
		for (int i = 0; i < ThisDataPointers->N; i++)
		{
			ThisDataPointers->GetFeatureValueNext(abc, feature_temp_store, &value);
			int flag = Tree->Function->TestFeature(feature_temp_store, abc);
			ThisDataPointers->ChildrenN[flag]++;
			ThisDataPointers->SetSplitFlagNext(flag);
		}
		ThisDataPointers->FastClose();
	}
	else
	{
		ThisDataPointers->FastInit();
		for (int i = 0; i < ThisDataPointers->N; i++)
		{
			ThisDataPointers->GetFeatureValueNext(abc, feature_temp_store, &value);
			int flag = Tree->Function->TestFeature(feature_temp_store, abc);
			ThisDataPointers->ChildrenN[flag]++;
		}
		ThisDataPointers->FastClose();
	}

	if (ThisDataPointers->ChildrenN[0] == 0 || ThisDataPointers->ChildrenN[1] == 0)
		return -1;
	else
		return 0;
}

int Node::SplitManyTimes(int times)
{
	ABC = new featuretype[Tree->Function->ABCNum];
	for (int d = 0; d < Tree->Function->ABCNum; d++)
		ABC[d] = 0;
	double mindiff = 10000000.0;
	featuretype* feature_temp_store = new featuretype[ThisData->D];
	featuretype* abc = new featuretype[Tree->Function->ABCNum];
	double* eout = new double[4];
	//srand(101 + Tree->ID);

	for (int t = 0; t < times; t++)
	{
		//Tree->Function->Data = ThisDataPointers;
		if (FuncRandomABC == NULL)
			Tree->Function->RandomABC(this, abc);
		else
			FuncRandomABC(this, ThisData, abc);
		int rec = Split(abc, feature_temp_store, true);

		if (rec == 0)
		{
			CalEntropy(eout);
			if (eout[0] < mindiff)
			{
				memcpy(ABC, abc, Tree->Function->ABCNum * sizeof(featuretype));
				mindiff = eout[0];
			}
		}
	}

	//ThisDataPointers->FastClose();
	delete feature_temp_store;
	delete abc;
	delete eout;

	if (mindiff == 10000000.0)
	{
		return -1;
	}
	else
	{
		//ThisDataPointers->NewSplitFlags();
		featuretype* feature_temp_store = new featuretype[ThisData->D];
		Split(ABC, feature_temp_store, true);
		delete feature_temp_store;
		return 0;
	}
}

int Node::CalEntropy(double* eout)
{
	valuetype sum[2] = { 0, 0 };
	int n[2] = { 0, 0 };

	ThisDataPointers->FastInit();
	for (int i = 0; i < ThisDataPointers->N; i++)
	{
		float v = ThisDataPointers->GetValueNext();
		int flag = ThisDataPointers->GetSplitFlagNext();
		sum[flag] += v;
		n[flag]++;
	}
	ThisDataPointers->FastClose();

	float average[2];
	if (n[0] > 0)
		average[0] = sum[0] / n[0];
	if (n[1] > 0)
		average[1] = sum[1] / n[1];
	float sum2[2] = { 0, 0 };

	ThisDataPointers->FastInit();
	for (int i = 0; i < ThisDataPointers->N; i++)
	{
		float v = ThisDataPointers->GetValueNext();
		int flag = ThisDataPointers->GetSplitFlagNext();
		sum2[flag] += pow(v - average[flag], 2);
	}
	ThisDataPointers->FastClose();

	eout[0] = (float)n[0] / (n[0] + n[1]) * sum2[0] + (float)n[1] / (n[0] + n[1]) * sum2[1];
	return 0;
}

double Node::FindCorrAndFit(featuretype* abc, featuretype* feature_temp_store)
{
	valuetype value;
	valuetype avefeature = 0;
	valuetype avevalue = 0;
	double stddfeature;
	double stddvalue;
	double sqdcorr;

	ThisDataPointers->FastInit();
	for (int i = 0; i < ThisDataPointers->N; i++)
	{
		ThisDataPointers->GetFeatureValueNext(abc, feature_temp_store, &value);
		avefeature += feature_temp_store[0];
		avevalue += value;
	}
	ThisDataPointers->FastClose();
	avefeature /= ThisDataPointers->N;
	avevalue /= ThisDataPointers->N;

	double sumsqdfeature = 0;
	double sumsqdvalue = 0;
	double sumsqdcorr = 0;
	ThisDataPointers->FastInit();
	for (int i = 0; i < ThisDataPointers->N; i++)
	{
		ThisDataPointers->GetFeatureValueNext(abc, feature_temp_store, &value);
		sumsqdfeature += pow((feature_temp_store[0] - avefeature), 2);
		sumsqdvalue += pow((value - avevalue), 2);
		sumsqdcorr += (feature_temp_store[0] - avefeature) * (value - avevalue);
	}
	ThisDataPointers->FastClose();
	stddfeature = sqrt(sumsqdfeature / ThisDataPointers->N);
	stddvalue = sqrt(sumsqdvalue / ThisDataPointers->N);

	if (stddfeature > 0 && stddvalue > 0)
	{
		sqdcorr = sumsqdcorr / stddfeature / stddvalue;
		FitCoefB = sumsqdcorr / sumsqdfeature;
		FitCoefA = avevalue - FitCoefB * avefeature;
	}
	else
	{
		sqdcorr = 0;
		FitCoefA = avevalue;
		FitCoefB = 0;
	}
	return abs(sqdcorr);
}

int Node::FindCorrManyTimes(int times)
{
	if (ABC_Fit == NULL)
		ABC_Fit = new featuretype[Tree->Function->ABCNum];
	double maxcorr = 0;
	featuretype* feature_temp_store = new featuretype[ThisData->D];
	featuretype* abc = new featuretype[Tree->Function->ABCNum];

	for (int t = 0; t < times; t++)
	{
		if (FuncRandomABC == NULL)
			Tree->Function->RandomABC(this, abc);
		else
			FuncRandomABC(this, ThisData, abc);
		
		double corr = FindCorrAndFit(abc, feature_temp_store);

		if (corr > maxcorr)
		{
			memcpy(ABC_Fit, abc, Tree->Function->ABCNum * sizeof(featuretype));
			maxcorr = corr;
		}
	}

	delete feature_temp_store;
	delete abc;

	if (maxcorr == 0)
	{
		return -1;
	}
	else
	{
		featuretype* feature_temp_store = new featuretype[ThisData->D];
		FindCorrAndFit(ABC_Fit, feature_temp_store);
		delete feature_temp_store;
		return 0;
	}
}

int Node::Vote()
{
	// for voting

	valuetype sum = 0;
	int n = 0;

	valuetype max = -1000000;
	valuetype min = 1000000;
	ThisDataPointers->FastInit();
	for (int i = 0; i < ThisDataPointers->N; i++)
	{
		float v = ThisDataPointers->GetValueNext();
		sum += v;
		n++;

		if (v > max) max = v;
		if (v < min) min = v;
	}
	ThisDataPointers->FastClose();
	FitMaxCap = max;
	FitMinCap = min;

	if (n > 0)
		AverageValue = sum / n;

	
	// for linear fitting
	if (n > 1)
	{
		FindCorrManyTimes(Tree->CandidatesEachNode);
	}
	else
	{
		FitCoefA = AverageValue;
		FitCoefB = 0;
	}
	
	return 0;
}

void Node::Release()
{
	if (!IsLeaf && Left != NULL)
		Left->Release();
	if (!IsLeaf && Right != NULL)
		Right->Release();

	if (ThisDataPointers != NULL)
		ThisDataPointers->Release();
	if (ABC != NULL)
		delete ABC;
	if (ABC_Fit != NULL)
		delete ABC_Fit;
	delete this;
}