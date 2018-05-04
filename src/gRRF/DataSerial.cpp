
#include "DataSerial.h"
#include "stdafx.h"

DataSerial::DataSerial()
{
	N = 0;
	ElementCount = 0;
	Elements = NULL;
	ElementsMemoryAlloCount = 0;
}

int DataSerial::AddElement(Data* data)
{
	while (ElementCount >= ElementsMemoryAlloCount - 1)
	{
		Data** newelements = new Data*[ElementsMemoryAlloCount * 2 + 1];
		if (ElementCount > 0)
			memcpy(newelements, Elements, ElementCount * sizeof(Data*));
		if (Elements != NULL)
			delete Elements;
		Elements = newelements;

		int* newen = new int[ElementsMemoryAlloCount * 2 + 1];
		if (ElementCount > 0)
			memcpy(newen, EN, (ElementCount) * sizeof(int));
		if (EN != NULL)
			delete EN;
		EN = newen;

		ElementsMemoryAlloCount = ElementsMemoryAlloCount * 2 + 1;
	}
	Elements[ElementCount] = data;
	EN[ElementCount] = N;
	ElementCount++;
	N += data->N;
	EN[ElementCount] = N;

	return 0;
}

Data* DataSerial::GetSample(int index, int* local_index_out)
{
	int ei = 0;
	while (index >= EN[ei + 1])
		ei++;
	return Elements[ei]->GetSample(index - EN[ei], local_index_out);;
}

featuretype* DataSerial::GetFeatureP(int index, featuretype* abc)
{
	int ei = 0;
	while (index >= EN[ei + 1])
		ei++;
	return Elements[ei]->GetFeatureP(index - EN[ei], abc);
}
void DataSerial::GetFeature(int index, featuretype* abc, featuretype* feature_out)
{
	int ei = 0;
	while (index >= EN[ei + 1])
		ei++;
	Elements[ei]->GetFeature(index - EN[ei], abc, feature_out);
}

valuetype* DataSerial::GetValueP(int index)
{
	int ei = 0;
	while (index >= EN[ei + 1])
		ei++;
	return Elements[ei]->GetValueP(index - EN[ei]);
}
void DataSerial::GetFeatureValue(int index, featuretype* abc, featuretype* feature_out, valuetype* value_out)
{
	int ei = 0;
	while (index >= EN[ei + 1])
		ei++;
	Elements[ei]->GetFeatureValue(index - EN[ei], abc, feature_out, value_out);
}

void DataSerial::GetValue(int index, valuetype* value_out)
{
	int ei = 0;
	while (index >= EN[ei + 1])
		ei++;
	Elements[ei]->GetValue(index - EN[ei], value_out);
}

int DataSerial::SetReachedNode(int index, Node* prediction)
{
	int ei = 0;
	while (index >= EN[ei + 1])
		ei++;
	return Elements[ei]->SetReachedNode(index - EN[ei], prediction);
}

Node* DataSerial::GetReachedNode(int index)
{
	int ei = 0;
	while (index >= EN[ei + 1])
		ei++;
	return Elements[ei]->GetReachedNode(index - EN[ei]);
}

int DataSerial::Release()
{
	for (int e = 0; e < ElementCount; e++)
	{
		Elements[e]->Release();
	}
	return 0;
}