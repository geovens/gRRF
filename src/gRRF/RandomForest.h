
#pragma once
#include <string>

class RandomTree;
class Data;
class Function;

class RandomForest
{
public:
	std::string SavePath;
	int TreeCount;
	// how to test data. 
	// 0 = use the linear fitting modal on leaf node of each tree
	// 1 = only use the average value of leaf node of each tree
	int LeafVotingMode = 0;

	RandomTree* Trees;

	__declspec(dllexport) RandomForest();
	__declspec(dllexport) int Plant(int count);
	__declspec(dllexport) int SetSavePath(std::string path);
	__declspec(dllexport) int SetFunction(Function* function);
	__declspec(dllexport) int SetCandidatesEachNode(int cen);
	__declspec(dllexport) int SetMaxTreeDepth(int depth);
	__declspec(dllexport) int SetMaxThreadNumber(int thread);
	__declspec(dllexport) int SetLeafVotingMode(int mode);
	// automatically start training new model or resume previous unfinished training process
	__declspec(dllexport) int Train(Data* data, int linkermode);
	// start training new model
	__declspec(dllexport) int TrainNew(Data* data, int linkermode);
	// read previously trained model
	__declspec(dllexport) int Load();
	__declspec(dllexport) int Test(Data* data);
	__declspec(dllexport) int Test(Data* data, int level);

	__declspec(dllexport) void Release();
};