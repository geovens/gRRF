
#include "RandomForest.h"
#include "RandomTree.h"
#include "Node.h"
#include "Data.h"

RandomForest::RandomForest()
{
	SavePath = "./output/";
	TreeCount = 0;
}

int RandomForest::Plant(int count)
{
	TreeCount = count;
	Trees = new RandomTree[count]();
	for (int n = 0; n < count; n++)
	{
		Trees[n].ID = n;
		Trees[n].Plant();
	}
	return 0;
}

int RandomForest::SetSavePath(std::string path)
{
	if (path.back() != '/' || path.back() != '\\')
		path += '/';
	SavePath = path;
	for (int n = 0; n < TreeCount; n++)
	{
		Trees[n].SavePath = path;
	}
	return 0;
}

int RandomForest::SetFunction(Function* function)
{
	for (int n = 0; n < TreeCount; n++)
	{
		Trees[n].Function = function;
	}
	return 0;
}

int RandomForest::SetCandidatesEachNode(int cen)
{
	for (int n = 0; n < TreeCount; n++)
	{
		Trees[n].CandidatesEachNode = cen;
	}
	return 0;
}

int RandomForest::SetMaxTreeDepth(int depth)
{
	for (int n = 0; n < TreeCount; n++)
	{
		Trees[n].MaxDepth = depth;
	}
	return 0;
}

int RandomForest::SetMaxThreadNumber(int thread)
{
	for (int n = 0; n < TreeCount; n++)
	{
		Trees[n].MaxThreadNumber = thread;
	}
	return 0;
}

int RandomForest::SetLeafVotingMode(int mode)
{
	LeafVotingMode = mode;
	return 0;
}

int RandomForest::SetMinSampleNOnLeaf(int minn)
{
	for (int n = 0; n < TreeCount; n++)
	{
		Trees[n].MinSampleNOnLeaf = minn;
	}
	return 0;
}

int RandomForest::TrainNew(Data* data, int linkermode)
{
	_mkdir(SavePath.c_str());
	if (linkermode == 2) _mkdir((SavePath + "internal").c_str());

	for (int n = 0; n < TreeCount; n++)
	{
		printf("Tree No. %d\n", n);
		int rec = Trees[n].TrainNew(data, linkermode);
		if (rec < 0) return rec;
	}
	return 0;
}

int RandomForest::Train(Data* data, int linkermode)
{
	_mkdir(SavePath.c_str());
	if (linkermode == 2) ((SavePath + "internal").c_str());

	for (int n = 0; n < TreeCount; n++)
	{
		printf("Tree No. %d\n", n);
		int rec = Trees[n].Train(data, linkermode);
		if (rec < 0) return rec;
	}
	return 0;
}

int RandomForest::Load()
{
	for (int t = 0; t < TreeCount; t++)
	{
		int rec = Trees[t].ReadNodeFile();
		if (rec < 0)
		{
			TreeCount = t;
			printf("failed load tree %d\n", t);
			return -1;
		}
	}
	return 0;
}

int RandomForest::Test(Data* data)
{
	for (int n = 0; n < TreeCount; n++)
	{
		Trees[n].ThisData = data;
	}

	featuretype* feature_temp_store = new featuretype[data->D];
	for (int i = 0; i < data->N; i++)
	{
		valuetype sum = 0;
		int sumn = 0;
		Node* node = NULL;
		for (int n = 0; n < TreeCount; n++)
		{
			node = Trees[n].TestFeature(i, feature_temp_store);
			if (LeafVotingMode == 1)
			{
				sum += node->AverageValue * node->N;
			}
			else
			{
				data->GetFeature(i, node->ABC_Fit, feature_temp_store);
				valuetype v = node->FitCoefA + node->FitCoefB * feature_temp_store[0];
				if (v > node->FitMaxCap)
					v = node->FitMaxCap;
				if (v < node->FitMinCap)
					v = node->FitMinCap;
				sum += v * node->N;
			}
			sumn += node->N;
			if (node->N == 0) printf("WARNING: N==0");
		}

		data->SetPrediction(i, sum / sumn);
		data->SetReachedNode(i, node);
	}
	delete feature_temp_store;
	return 0;
}

int RandomForest::Test(Data* data, int level)
{
	for (int n = 0; n < TreeCount; n++)
	{
		Trees[n].ThisData = data;
	}

	featuretype* feature_temp_store = new featuretype[data->D];
	for (int i = 0; i < data->N; i++)
	{
		valuetype sum = 0;
		int sumn = 0;
		Node* node = NULL;
		for (int n = 0; n < TreeCount; n++)
		{
			node = Trees[n].TestFeature(i, level, feature_temp_store);
			if (LeafVotingMode == 1)
			{
				sum += node->AverageValue * node->N;
			}
			else
			{
				data->GetFeature(i, node->ABC_Fit, feature_temp_store);
				valuetype v = node->FitCoefA + node->FitCoefB * feature_temp_store[0];
				if (v > node->FitMaxCap)
					v = node->FitMaxCap;
				if (v < node->FitMinCap)
					v = node->FitMinCap;
				sum += v * node->N;
			}
			sumn += node->N;
		}

		data->SetPrediction(i, sum / sumn);
		data->SetReachedNode(i, node);
	}
	delete feature_temp_store;
	return 0;
}

void RandomForest::Release()
{
	for (int n = 0; n < TreeCount; n++)
	{
		Trees[n].Release();
	}
	TreeCount = 0;
}