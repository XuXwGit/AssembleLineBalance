#include "DSALB.h"

void DSALB::generateData()
{
	setNumTasks(Graph.getNumOfVertices());
	vector<int> _timeSet;
	vector<int> _workSet;
	vector<vector<pair<int, int>>> _PiSet;
	for (size_t i = 0; i < Graph.getNumOfVertices(); i++)
	{
		_workSet.push_back(Graph.getVertexSet()[i].work);
		_timeSet.push_back(Graph.getVertexSet()[i].time);
		_PiSet.push_back(Graph.getPostWork(_workSet[i]));
	}
	setAssemblyTimeSet(_timeSet);
	setWorkSet(_workSet);
	setPiSet(_PiSet);
}
void DSALB::createDecisions()
{
	ct = IloNumVar(env, 0, IloInfinity);
	
	x = IloArray<IloBoolVarArray>(env, num_tasks);
	for (int i = 0; i < num_tasks; i++)
	{
		x[i] = IloBoolVarArray(env, num_stations);
		for (int j = 0; j < num_stations; j++)
		{
			string var_name = "x" + to_string(i + 1) + "_" + to_string(j + 1);
			x[i][j].setName(var_name.c_str());
		}
	}
}

void DSALB::addConstraint1()
{
	for (int i = 0; i < num_tasks; i++) {
		IloIntExpr expr(env);
		for (int j = 0; j < num_stations; j++) {
			expr += x[i][j];
		}
		string constr = "c1-" + to_string(i + 1) ;
		model.add(expr == 1).setName(constr.c_str());
	}
}

void DSALB::addConstraint2()
{
	for (int j = 0; j < num_stations; j++) {
		IloIntExpr expr(env);
		for (int i = 0; i < num_tasks; i++) {
			expr += x[i][j];
		}
		string constr = "c2-" + to_string(j + 1);
		model.add(expr >= 1).setName(constr.c_str());
	}
}

void DSALB::addConstraint3()
{
	for (int i = 0; i < num_tasks; i++)
	{
		for (int _i = 0; _i < PiSet[i].size(); _i++)
		{
			IloIntExpr left(env); 
			IloIntExpr right(env);

			int index = PiSet[i][_i].first - 1;

			for (int j = 0; j < num_stations; j++)
			{
				left += (j + 1) * x[i][j];
				right += (j + 1) * x[index][j];
			}
			string constr = "c3-" + to_string(i + 1) +"_" + to_string(_i + 1);
			model.add(left <= right).setName(constr.c_str());
		}
	}
}

void DSALB::addConstraint4()
{
	for (int j = 0; j < num_stations; j++) {
		IloIntExpr expr(env);
		for (int i = 0; i < num_tasks; i++) {
			expr += assemblyTimeSet[i] * x[i][j];
		}
		string constr = "c4" + to_string(j + 1);
		model.add(expr <= ct).setName(constr.c_str());
	}
}

void DSALB::solveModel()
{
	//cplex.setOut(env.getNullStream());
	cplex.exportModel("dsalb.lp");
	if (cplex.solve()) {
		printsolution();
	}
}

int DSALB::getObjective()
{
	return cplex.getObjValue();
}

void DSALB::printsolution()
{
	cout << "Solution status: " << cplex.getStatus() << endl;
	cout << "The Optimal Cycle Time(CT): " << getObjective()<<endl;
	cout << "Solving Time: " << cplex.getCplexTime()<<endl;

	vector<double> balanceRateSet;
	for (size_t j = 0; j < num_stations; j++)
	{
		double totalTaskTime = 0;
		cout << "Station " << j+1 << " : ";
		for (int i = 0; i < num_tasks; i++)
		{
			if (cplex.getValue(x[i][j]) != 0) {
				cout << "(" << workSet[i] << "," << assemblyTimeSet[i] <<")" << ",";
				totalTaskTime += assemblyTimeSet[i];
			}
		}
		cout << endl <<endl;
		balanceRateSet.push_back(totalTaskTime / cplex.getObjValue());
	}
	
	cout << "Balance Rate : ";
	for (size_t j = 0; j < num_stations; j++)
	{
		cout << balanceRateSet[j] << "\t";
	}
	cout << endl;

	// set average balance rate
	double sumBalanceRate = accumulate(balanceRateSet.begin(), balanceRateSet.end(), 0.0);
	setAverageBalanceRate(sumBalanceRate / num_stations);
	cout << "Average Balance Rate : " << this->averageBalanceRate;
	cout << endl;
}

