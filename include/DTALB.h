#pragma once
// Determine Model for Two-side Assembly Line Balancing Problem
// denote it as "DTALB"
// type £º Type -2
//	Objective : minimize the cycle time with a given number of "Stations"

#ifndef _DTALB_H_
#define _DTALB_H_

#include <algorithm>
#include <numeric>
#include <tuple>

#include <ilcplex/ilocplex.h>
ILOSTLBEGIN

#include "SequeGraph.h"

// construct function:
// input : Sequence Graph + num_stations / cycle time + type ('I' / 'II' / 'E' / 'F') 
class DTALB
{
public:
	inline DTALB(SequeGraph& Graph, int num_stations, string type)
		: Graph(Graph),
		type(type)
	{
		if (type == "II") {
			setNumStations(num_stations);
		}

		generateData();
		env = IloEnv();
		model = IloModel(env);
		cplex = IloCplex(model);

		createDecisions();
		setObjective();
		addConstraints();
	}
	//~DTALB();

	inline void setNumTasks(int num_task) {
		this->num_tasks = num_task;
	}
	inline void setNumStations(int num_station) {
		this->num_stations = num_station;
	}
	inline void setWorkSet(vector<int>& works) {
		this->workSet = works;
	}
	inline void setAssemblyTimeSet(vector<int>& timeSet) {
		this->assemblyTimeSet = timeSet;
	}
	inline void setPiSet(vector<vector<pair<int, int>>>& Pi) {
		this->PiSet = Pi;
	}
	inline void setCycleTime(int cycle_time) {
		this->cycletime = cycle_time;
	}
	inline void setAverageBalanceRate(double aveBalaRate) {
		this->averageBalanceRate = aveBalaRate;
	}
	inline void setSequenceSet(vector<vector<tuple<int, int, int>>>& sequence_set) {
		this->sequenceSet = sequence_set;
	}

	void generateData();

	void createDecisions();
	inline void setObjective() {
		model.add(IloMinimize(env, ct));
	}

	inline void addConstraints() {
		addConstraint1();
		addConstraint2();
		addConstraint3();
		addConstraint4();
		addConstraint5();
		addConstraint6();
	}
	void addConstraint1();
	void addConstraint2();
	void addConstraint3();
	void addConstraint4();
	void addConstraint5();
	void addConstraint6();

	void solveModel();

	inline double getObjective() {
		return cplex.getObjValue();
	}
	void printsolution();

	inline int getCycleTime() {
		return cycletime;
	}
	inline double getAverageBalanceRate() {
		return averageBalanceRate;
	}
	inline vector<vector<tuple<int, int, int>>>& getSequenceSet() {
		return sequenceSet;
	}
	inline vector< tuple<int, int, int>>& getSequenceOnStation(int k) {
		return sequenceSet[k];
	}

private:
	// data
	SequeGraph Graph;
	IloInt num_tasks;				// I
	IloInt num_stations;			// J
	IloInt num_sides = 2;				// K
	vector<int> workSet;
	vector<int> assemblyTimeSet;	//t_i
	vector<vector <pair< int, int>> > PiSet;		//the task set after task i

	// problem type
	string type;

	// model
	IloEnv env;
	IloModel model;
	IloCplex cplex;

	// decision variables
	// CT: cycle time
	IloNumVar ct;
	// x[i][j][k] : 1, if task 1 is assigned to work station j at side k; 0, otherwise
	IloArray <IloArray< IloBoolVarArray >> x;
	// y[i][i'] : auxiliary variables
	IloArray<IloBoolVarArray> y;
	// st[i] : the start time of task i in it's work station
	IloArray<IloNumVar> st;

	const IloNum M = 5000;

	// final solution
	int cycletime;
	double averageBalanceRate;
	vector<vector<tuple<int, int, int>>> sequenceSet;
};

//DTALB::~DTALB()
//{
//}

#endif // !_DTALB_H_
