#pragma once
// Determine Model for Simple Assembly Line Balancing Problem
// denote it as "DSALB"
// type £º Type -2
//	Objective : minimize the cycle time with a known number of "Stations"

#ifndef _DSALB_H_
#define _DSALB_H_

#include <algorithm>
#include <numeric>

#include <ilcplex/ilocplex.h>
ILOSTLBEGIN

#include "SequeGraph.h"

// construct function:
// input : Sequence Graph + num_stations / cycle time + type ('I' / 'II' / 'E' /'F') 
class DSALB
{
public:
	inline DSALB(SequeGraph& Graph, int num_stations, string type)
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
	//~DSALB();

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
	}
	void addConstraint1();
	void addConstraint2();
	void addConstraint3();
	void addConstraint4();

	void solveModel();

	int getObjective();
	void printsolution();

	inline int getCycleTime() {
		return cycletime;
	}
	inline double getAverageBalanceRate() {
		return averageBalanceRate;
	}

private:
	// data
	SequeGraph Graph;
	IloInt num_tasks;				// I
	IloInt num_stations;		// J
	vector<int> workSet;
	vector<int> assemblyTimeSet;	//t_i
	vector<vector <pair< int , int>> > PiSet;		//the task set after task i

	// problem type
	string type;

	// model
	IloEnv env;
	IloModel model;
	IloCplex cplex;
	IloNumVar ct;
	IloArray<IloBoolVarArray> x;

	// final solution
	int cycletime;
	double averageBalanceRate;
};

//DSALB::~DSALB()
//{
//}

#endif // !_DSALB_H_
