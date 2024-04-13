#ifndef _GaGenome_cpp_
#define _GaGenome_cpp_

#include<algorithm>
#include"Individual.h"
#include"SequeGraph.h"

Individual::Individual()
	:genome(vector<int>()), fitness(0), selectProbility(0), accumulateProbability(0), Stations(0)
{
}

Individual::Individual(vector<int>& vec)
	: genome(vec), fitness(0), selectProbility(0), accumulateProbability(0), Stations(0)
{
}

Individual::Individual(vector<int>& vec, const SequeGraph& Graph)
	: genome(vec), fitness(0), selectProbility(0), accumulateProbability(0),myGraph(Graph), Stations(0)
{
}

Individual::Individual(const Individual& Individual)
	: genome(Individual.genome),
	fitness(Individual.fitness),
	Stations(Individual.Stations),
	selectProbility(Individual.selectProbility),
	accumulateProbability(Individual.accumulateProbability),
	myGraph(Individual.myGraph)
{
}

Individual::~Individual()
{
}

int Individual::CaculateStations()
{
	Stations = 1;
	int tl = 0, tr = 0;
	
	for (int i = 0; i < genome.size(); i++)
	{
		int work = genome[i];
		int time = myGraph.getTime(work);
		char direction = myGraph.getDirection(work);
		set<int> nextVertexs = myGraph.getNextVertexs(work);
		int setupTime = myGraph.getSetupTime(work);
		
		//检查入度是否为0
		if (myGraph.getInDegree(work) != 0)
		{
			cout << "There are stilled unscheduled works ahead of the work"<<work<<"!";
		}
		//修改work的出度信息及其后序作业的入度信息
		while (myGraph.getOutDegree(work))
		{
			for (int x : nextVertexs)
			{
				//更新后序作业的最早开始时间
				if (myGraph.getSetupTime(x) <setupTime  + myGraph.getTime(work))
				{
					myGraph.setVertexSetuptime(x, setupTime + myGraph.getTime(work));
				}
				myGraph.setInDegree(x, myGraph.getInDegree(x) - 1);
				myGraph.setOutDegree(work, myGraph.getOutDegree(work) - 1);
			}
		}

		if (setupTime/CT != (setupTime + time)/CT && (setupTime + time) % CT != 0)
		{
			setupTime = (setupTime / CT + 1) * CT;
			myGraph.setVertexSetuptime(work, setupTime);
		}

		if (direction == 'L')
		{
			if (tl > setupTime)
			{
				setupTime = tl;
				if (setupTime / CT != (setupTime + time) / CT && (setupTime + time) % CT != 0)
				{
					setupTime = (setupTime / CT + 1) * CT;
				}
				myGraph.setVertexSetuptime(work, setupTime);
			}
			tl = setupTime + time;
		}
		else if (direction == 'R')
		{
			if (tr > setupTime)
			{
				setupTime = tr;
				if (setupTime / CT != (setupTime + time) / CT && (setupTime + time) % CT != 0)
				{
					setupTime = (setupTime / CT + 1) * CT;
				}
				myGraph.setVertexSetuptime(work, setupTime);
			}
			tr = setupTime + time;
		}
		else
		{
			if (tl > tr)
			{
				myGraph.resetVertexDirection(work, 'R');
				if (tr > setupTime)
				{
					setupTime = tr;
					if (setupTime / CT != (setupTime + time) / CT && (setupTime + time) % CT != 0)
					{
						setupTime = (setupTime / CT + 1) * CT;
					}
					myGraph.setVertexSetuptime(work, setupTime);
				}
				tr = setupTime + time;
			}
			else
			{
				myGraph.resetVertexDirection(work, 'L');
				if (tl > setupTime)
				{
					setupTime = tl;
					if (setupTime / CT != (setupTime + time) / CT && (setupTime + time) % CT != 0)
					{
						setupTime = (setupTime / CT + 1) * CT;
					}
					myGraph.setVertexSetuptime(work, setupTime);
				}
				tl = setupTime + time;
			}
		}
	}
	if (((tl > tr) ? tl : tr) % CT == 0)
	{
		Stations = ((tl > tr) ? tl : tr) / CT;
	}
	else
	{
		Stations = ((tl > tr) ? tl : tr) / CT + 1;
	}
	return Stations;
}


int Individual::getStations()
{
	return this->Stations;
}

void Individual::InitializeGenome(SequeGraph& Graph)
{
	SequeGraph SeGraph(Graph);
	vector<int> genome = SeGraph.topoSort();
	this->setGenome(genome);
	this->myGraph = Graph;
}

void Individual::CaculateIndividualFitness()
{
	int Stations = CaculateStations();
	this->fitness = (double) 1.0/Stations;
}

void Individual::setSelectProbability(double selectProbability)
{
	this->selectProbility = selectProbability;
}

void Individual::setAccumulateProbability(double accumulateProbability)
{
	this->accumulateProbability = accumulateProbability;
}

void Individual::setGenome(vector<int>& genome)
{
	this->genome.clear();
	for (int i = 0; i < genome.size(); i++)
	{
		this->genome.push_back(genome[i]);
	}
	//this->genome = genome;
}

void Individual::setGraph(SequeGraph& Graph)
{
	myGraph = Graph;
}

void Individual::printIndividual()
{
	cout << "The work sort is :" << endl;
	//this->printGenome();
	cout << endl;
	cout << "The number of workStations is : " << this->getStations() << endl;
	cout << "The fitness is : " << fitness << endl;
	//cout << "The selectprobability is : " << selectProbility << endl;
	//cout << "The accumulateProbability is " << accumulateProbability << endl;
}

void Individual::printGenome()
{
	for (int i = 0; i < this->getGenome().size(); i++)
	{
		int work = this->getGenome()[i];
		char direction = this->myGraph.getDirection(work);
		cout << work;
		cout << "(" << myGraph.getSetupTime(work) << ","<< myGraph.getSetupTime(work)+myGraph.getTime(work)<< "," << direction << ")";
		cout << " ";
	}
}

double Individual::getFitness()
{
	return fitness;
}

double Individual::getSelectProbability()
{
	return selectProbility;
}

double Individual::getAccumulateProbability()
{
	return accumulateProbability;
}

vector<int>& Individual::getGenome()
{
	return this->genome;
}

Individual& Individual::operator=(Individual& individual)
{
	this->setGenome(individual.genome);
	this->myGraph = individual.myGraph;
	this->fitness = individual.fitness;
	this->selectProbility = individual.selectProbility;
	this->accumulateProbability = individual.accumulateProbability;
	this->Stations = individual.Stations;
	return *this;
}


#endif // !_GaGenome_cpp_