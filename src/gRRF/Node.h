
#pragma once
#include "Type.h"
class Data;
class Linker;
class RandomTree;

class Node
{
public:
	// pointer to the tree this is belonging to
	RandomTree* Tree;
	unsigned long long Index;
	// root node is level 0. leaf nodes are level depth.
	int Level;
	// randomly generated params for splitting (on non-leaf nodes) 
	featuretype* ABC;
	// randomly generated params for linear fitting (on leaf nodes)
	featuretype* ABC_Fit;
	// whether this is a leaf node
	bool IsLeaf;
	// linear fitting coef (on leaf nodes)
	double FitCoefA;
	double FitCoefB;
	valuetype FitMaxCap;
	valuetype FitMinCap;
	// whether this node has been trained
	int Trained;
	// type of this node
	int Type;
	// number of samples reached this node
	int N;
	// average value of all samples on this node
	valuetype AverageValue;

	// data of the entire tree;
	Data* ThisData;
	// pointers to data that reached this node
	Linker* ThisDataPointers;

	Node* Parent;
	Node* Left;
	Node* Right;

	int* (*FuncRandomABC)(Node* node, Data* data, featuretype* abc);

	Node();

	int Split(featuretype* abc, featuretype* feature_temp_store, bool setsplitflags = false);
	int SplitManyTimes(int times);
	int CalEntropy(double* eout);
	double FindCorrAndFit(featuretype* abc, featuretype* feature_temp_store);
	int FindCorrManyTimes(int times);
	int Vote();

	void Release();
};