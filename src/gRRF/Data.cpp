
#include "Data.h"
#include "stdafx.h"


Data::Data()
{
	ReachedNodes = NULL;
}

Data* Data::GetSample(int index, int* local_index_out)
{
	*local_index_out = index;
	return this;
}

featuretype* Data::GetFeatureP(int index, featuretype* abc)
{
	return Features + index * D;
}

void Data::GetFeature(int index, featuretype* abc, featuretype* feature_out)
{
	memcpy(feature_out, Features + index * D, sizeof(featuretype)* D);
}

valuetype* Data::GetValueP(int index)
{
	return Values + index;
}

void Data::GetFeatureValue(int index, featuretype* abc, featuretype* feature_out, valuetype* value_out)
{
	/*
	memcpy(feature_out, Features + index * D, sizeof(featuretype) * D);
	*label_out = *(Values + index);
	*/
	GetFeature(index, abc, feature_out);
	GetValue(index, value_out);
}

void Data::GetValue(int index, valuetype* value_out)
{
	*value_out = *(Values + index);
}

int Data::SetReachedNode(int index, Node* node)
{
	if (ReachedNodes == NULL)
		ReachedNodes = new Node*[N];
	ReachedNodes[index] = node;
	return 0;
}

Node* Data::GetReachedNode(int index)
{
	if (ReachedNodes == NULL)
		return NULL;
	return ReachedNodes[index];
}

int Data::SetPrediction(int index, valuetype prediction)
{
	if (Predictions == NULL)
		Predictions = new valuetype[N];
	Predictions[index] = prediction;
	return 0;
}

valuetype Data::GetPrediction(int index)
{
	if (Predictions == NULL)
		return -99;
	return Predictions[index];
}

int Data::Release()
{
	delete Features;
	delete Values;
	//delete SplitFlags;
	//delete LabelCount;
	delete this;
	return 0;
}