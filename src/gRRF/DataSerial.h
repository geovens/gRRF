
#pragma once
#include "Data.h"

class DataSerial : public Data
{
public:
	
	int ElementCount;
	Data** Elements;

	int ElementsMemoryAlloCount;

	int* EN;

	__declspec(dllexport) DataSerial();

	__declspec(dllexport) int AddElement(Data* data);
	
	__declspec(dllexport) virtual Data* GetSample(int index, int* local_index_out);
	__declspec(dllexport) virtual featuretype* GetFeatureP(int index, featuretype* abc);
	__declspec(dllexport) virtual void GetFeature(int index, featuretype* abc, featuretype* feature_out);
	__declspec(dllexport) virtual valuetype* GetValueP(int index);
	__declspec(dllexport) virtual void GetFeatureValue(int index, featuretype* abc, featuretype* feature_out, valuetype* value_out);
	__declspec(dllexport) virtual void GetValue(int index, valuetype* value_out);
	__declspec(dllexport) virtual int SetReachedNode(int index, Node* prediction);
	__declspec(dllexport) virtual Node* GetReachedNode(int index);

	//__declspec(dllexport) virtual int FastInit();
	//__declspec(dllexport) virtual double* GetFeatureFast();
	//__declspec(dllexport) virtual int GetLabelFast();
	//__declspec(dllexport) virtual void SetSplitFlagFast(int flag);

	//__declspec(dllexport) virtual int NewSplitFlags();
	//__declspec(dllexport) virtual int GetLabelCount(int k);

	////virtual int SetChildrenN0();
	//__declspec(dllexport) virtual Data** NewChildren();
	//__declspec(dllexport) virtual int Split(Data** child);

	__declspec(dllexport) virtual int Release();
};