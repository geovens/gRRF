
#pragma once
#include "Type.h"
class Node;

class Data
{
public:
	// number of samples
	int N;
	// dimension of feature
	int D;
	// settings of how data is storaged during training
	int StorageMode;

	// features. (2018.05.02: seems that this is only used for fixed feature values of finite dimension)
	featuretype* Features;
	// value of each sample (for regression)
	valuetype* Values;
	
private:	
	// predicted value of each sample. 
	// (2016.08: seems like only available after testing with a forest instead of a single tree)
	valuetype* Predictions;
	// the leaf node that each sample reaches. 
	// (2016.08: if tested with a single tree instead of a forest, this records only the leaf node of the last tree in the forest)
	// format: ReachedNodes[i] is a pointer to the leaf node that the i'th sample reaches
	Node** ReachedNodes;

public:
	__declspec(dllexport) Data();
	__declspec(dllexport) virtual Data* GetSample(int index, int* local_index_out);
	__declspec(dllexport) virtual featuretype* GetFeatureP(int index, featuretype* abc);
	__declspec(dllexport) virtual void GetFeature(int index, featuretype* abc, featuretype* feature_out);
	__declspec(dllexport) virtual valuetype* GetValueP(int index);
	__declspec(dllexport) virtual void GetFeatureValue(int index, featuretype* abc, featuretype* feature_out, valuetype* value_out);
	__declspec(dllexport) virtual void GetValue(int index, valuetype* value_out);

	__declspec(dllexport) virtual int SetPrediction(int index, valuetype prediction);
	__declspec(dllexport) virtual valuetype GetPrediction(int index);
	__declspec(dllexport) virtual int SetReachedNode(int index, Node* prediction);
	__declspec(dllexport) virtual Node* GetReachedNode(int index);


	__declspec(dllexport) virtual int Release();
};