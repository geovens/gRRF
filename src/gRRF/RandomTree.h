
#pragma once
#include "Type.h"
#include "stdafx.h"
class Node;
class Function;
class Data;

class RandomTree
{
public:
	// output file location
	std::string SavePath;
	// index of tree
	int ID;
	// max allowed depth of tree
	int MaxDepth;
	// min sample number on leaf nodes
	int MinSampleNOnLeaf;
	// real depth of tree
	int Depth;
	// how many times the process that params abc is random selected and entropy is calculated is done on each node.
	int CandidatesEachNode;
	// number of threads used for training
	int MaxThreadNumber;
	// a pointer to the root node
	Node* Root;
	// decision function used to split samples on each node
	Function* Function;
	// the training/testing data
	// there is only one occurence of Data attached on the tree
	Data* ThisData;
	// storage mode
	int PointersMode;

	int ThreadCount;
	std::mutex ThreadCount_mutex;
	//int IsWritingNode;
	FILE* FNode;


	__declspec(dllexport) RandomTree();
	__declspec(dllexport) ~RandomTree();

	__declspec(dllexport) int Plant();
	// grow unless MaxDepth reached
	__declspec(dllexport) int Grow(Node* node);
	// grow and increas MaxDepth if it's reached
	__declspec(dllexport) int ForceGrow(Node* node);
	__declspec(dllexport) int PlantGrowFull(int depth);
	__declspec(dllexport) int ReadNodeFile();
	// automatically start training new model or resume previous unfinished training process
	__declspec(dllexport) int Train(Data* data, int linkermode);
	// start training new model
	__declspec(dllexport) int TrainNew(Data* data, int linkermode);

	__declspec(dllexport) int Test(Data* data);
	__declspec(dllexport) int Test(Data* data, int level);
	__declspec(dllexport) Node* TestFeature(featuretype* feature);
	__declspec(dllexport) Node* TestFeature(featuretype* feature, int level);
	__declspec(dllexport) Node* TestFeature(int index, featuretype* feature_temp_store);
	__declspec(dllexport) Node* TestFeature(int index, int level, featuretype* feature_temp_store);
	__declspec(dllexport) bool TestFeatureReach(int index, Node* node, featuretype* feature);
	__declspec(dllexport) Node* FindNode(int level, unsigned long long index);
	static int NewThread(void* lpParameter);

	int LoadDataPointers(Node* node);

	__declspec(dllexport) void Release();

protected:
	int RecursionNewChildNodes(Node* node);
	int RecursionSplitTrainingSet(Node* node);
	int ReadTrainingProcess();
	int WriteNode(Node* node);
};