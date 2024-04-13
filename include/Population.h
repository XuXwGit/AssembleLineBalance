#pragma once
#ifndef _POPULATION_H_
#define _POPULATION_H_

#include <vector>
using namespace std;

#include "Individual.h"
#include "SequeGraph.h"
#include "publicPara.h"

class Population : public pubPara
{
private:
	vector<Individual> population;
	Individual bestIndividual;
	int populationSize;
	SequeGraph Graph;
public:
	Population();
	Population(int populationSize);
	Population(int populationSize, const SequeGraph&);
	Population(const SequeGraph&);

	void InitializePopulation();

	double CalculateFitness();

	void CalculateProbability(double totalFitness);

	void Roulutte(vector<Individual>&);

	void Select(double totalFitness);

	void CrossIndividual(Individual&, Individual&);

	int getBeginPos(int work, vector<int>&);
	int getEndPos(int work, vector<int>&);

	vector<Individual>& getPopulation();

	void Cross();

	void redistribute(vector<int>& genome, SequeGraph& genomeGraph, set<int>& RS, set<int>& PS);

	void Mutation();

	Individual& lastBestIndividual();

	Individual& getBestIndividual();
};


#endif // !_POPULATION_H_