#pragma once
#ifndef _Individual_h_

#define _Individual_h_

#include<iostream>
#include<vector>
#include<utility>

#include"SequeGraph.h"
#include "publicPara.h"

using namespace std;

class Individual : public pubPara
{
private:
	vector<int> genome;
	SequeGraph myGraph;
	int Stations;
	double fitness;
	double selectProbility;
	double accumulateProbability;
public:
	Individual();
	Individual(vector<int>&);
	Individual(vector<int>&, const SequeGraph&);
	Individual(const Individual& Individual);
	~Individual();

	int CaculateStations();
	int getStations();

	void InitializeGenome(SequeGraph&);
	void CaculateIndividualFitness();
	
	void setSelectProbability(double selectProbability);
	void setAccumulateProbability(double accumulateProbability);
	void setGenome(vector<int>&);
	void setGraph(SequeGraph&);

	void printIndividual();
	void printGenome();

	double getFitness();
	double getSelectProbability();
	double getAccumulateProbability();

	vector<int>& getGenome();

	Individual& operator = (Individual&);
};

#endif // !_GaGenome_h_