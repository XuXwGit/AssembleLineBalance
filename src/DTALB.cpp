#include "DTALB.h"

void DTALB::generateData()
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
void DTALB::createDecisions()
{
	ct = IloNumVar(env, 0, IloInfinity, "CT");

	x = IloArray <IloArray< IloBoolVarArray >> (env, num_tasks);
	y = IloArray<IloBoolVarArray>(env, num_tasks);
	st = IloArray<IloNumVar>(env, num_tasks);
	for (int i = 0; i < num_tasks; i++)
	{
		x[i] = IloArray<IloBoolVarArray>(env, num_stations);
		for (int j = 0; j < num_stations; j++)
		{
			x[i][j] = IloBoolVarArray(env, num_sides);

			for (IloInt k = 0; k < num_sides; k++)
			{
				string s = ("x." + to_string(i + 1) + "_" + to_string(j + 1) + "_" + to_string(k + 1) );
				x[i][j][k].setName(s.c_str());
			}
		}

		y[i] = IloBoolVarArray(env, num_tasks);
		for (IloInt ii = 0; ii < num_tasks; ii++)
		{
			string var_name = "y." + to_string(i + 1) +"_" + to_string(ii + 1);
			y[i][ii].setName(var_name.c_str());
		}

		string var_name = "st." + to_string(i + 1);
		st[i] = IloNumVar(env, 0, IloInfinity, IloNumVar::Float, var_name.c_str());
	}
}

void DTALB::addConstraint1()
{
	for (int i = 0; i < num_tasks; i++) 
	{
		IloIntExpr expr(env);
		for (int j = 0; j < num_stations; j++) 
		{
			for (IloInt k = 0; k < num_sides; k++)
			{
				expr += x[i][j][k];
			}
		}
		string constr = "c1." + to_string(i + 1);
		model.add(expr == 1).setName(constr.c_str());
	}
}

void DTALB::addConstraint2()
{
	for (int j = 0; j < num_stations; j++) 
	{
		IloIntExpr expr(env);
		for (int i = 0; i < num_tasks; i++) 
		{
			for (IloInt k = 0; k < num_sides; k++)
			{
				expr += x[i][j][k];
			}
		}
		string constr = "c2." + to_string(j + 1);
		model.add(expr >= 1).setName(constr.c_str());
	}
}

void DTALB::addConstraint3()
{
	for (int i = 0; i < num_tasks; i++)
	{
		for (int _i = 0; _i < PiSet[i].size(); _i++)
		{
			IloIntExpr left(env);
			IloIntExpr right(env);

			// get i'
			int index = PiSet[i][_i].first - 1;

			for (int j = 0; j < num_stations; j++)
			{
				for (IloInt k = 0; k < num_sides; k++)
				{
					left += (j + 1) * x[i][j][k];
					right += (j + 1) * x[index][j][k];
				}
			}
			string constr = "c3." + to_string(i + 1) + "_" + to_string(_i + 1);
			model.add(left <= right).setName(constr.c_str());
		}
	}
}

// 
// st_i + t_i <= CT
void DTALB::addConstraint4()
{
	for (int i = 0; i < num_tasks; i++) {
		string constr = "c4." + to_string(i + 1);
		model.add(st[i] + assemblyTimeSet[i] <= ct).setName(constr.c_str());
	}
}

// 
// a. i<i'
//		st_i + t_i <= st_i' + M[2 - (x_ijk + x_i'jk)] + M y_ii'
// b. i'<i
//		st_i' + t_i' <= st_i + M[2 - (x_ijk + x_i'jk)] + M(1 - y_ii')
void DTALB::addConstraint5()
{
	for (IloInt i = 0; i < num_tasks; i++)
	{
		for (IloInt ii = i + 1; ii < num_tasks; ii++)
		{
			for (IloInt j = 0; j < num_stations; j++)
			{
				for (IloInt k = 0; k < num_sides; k++)
				{
					string constr1 = "c5a." + to_string(i + 1) 
																	+ "_" + to_string(ii + 1)
																	+ "_" + to_string(j + 1)
																	+ "_" + to_string(k + 1);
					IloExpr left1 = st[i] + assemblyTimeSet[i];
					IloExpr right1 = st[ii] + M * (2 - x[i][j][k] - x[ii][j][k]) + M * y[i][ii];
					model.add(left1 <= right1).setName(constr1.c_str());

					string constr2 = "c5b." + to_string(i + 1)
																	+ "_" + to_string(ii + 1)
																	+ "_" + to_string(j + 1)
																	+ "_" + to_string(k + 1)	;
					IloExpr left2 = st[ii] + assemblyTimeSet[ii];
					IloExpr right2 = st[i] + M * (2 - x[i][j][k] - x[ii][j][k]) + M * (1 - y[i][ii]);
					model.add(left2 <= right2).setName(constr2.c_str());

					model.add(y[i][ii] + y[ii][i] == 1);
				}
			}
		}
	}
}

void DTALB::addConstraint6()
{
	for (IloInt i = 0; i < num_tasks; i++)
	{
		for (size_t ii = 0; ii < PiSet[i].size(); ii++)
		{
			int index_ii = PiSet[i][ii].first - 1;
			for (IloInt j = 0; j < num_stations; j++)
			{
				string constr = "c6." + to_string(i + 1) 
															+ "_" + to_string(ii + 1)
															+ "_" + to_string(j + 1);
				IloExpr left = st[i] + assemblyTimeSet[i];
				IloExpr right = st[index_ii] + M * 2;
				for (IloInt k = 0; k < num_sides; k++)
				{
					 right += M * ( - x[i][j][k] - x[index_ii][j][k]);
				}
				model.add(left <= right).setName(constr.c_str());
			}
		}
	}
}

void DTALB::solveModel()
{
	//cplex.setOut(env.getNullStream());
	cplex.exportModel("dtalb.lp");
	if (cplex.solve()) {
		printsolution();
	}
}

// greater
inline bool comp_work(tuple<int, int, int> left, tuple<int, int, int> right) {
	if ((std::get<2>(left)) < (std::get<2>(right))) {
		return 1;
	}
	else if ((std::get<1>(left)) > (std::get<1>(right)))
	{
		return 0;
	}
	else
	{
		return (std::get<1>(left)) < (std::get<1>(right));
	}
}

void DTALB::printsolution()
{
	cout << "Solution status: " << cplex.getStatus() << endl;
	cout << "The Optimal Cycle Time(CT):" << getObjective() << endl;

	// resort sequence
	vector<double> balanceRateSet;
	vector<vector<tuple<int, int, int>>> sequences;
	for (IloInt j = 0; j < num_stations; j++)
	{
		double totalTaskTime = 0;
		vector<tuple<int, int, int>> subsequence;
		for (IloInt k = 0; k < num_sides; k++)
		{
			for (IloInt i = 0; i < num_tasks; i++)
			{
				if (cplex.getValue(x[i][j][k]) != 0) {
					subsequence.push_back(tuple<int, int, int>(workSet[i], cplex.getValue(st[i]), k));
					totalTaskTime += assemblyTimeSet[i];
				}
			}
		}
		balanceRateSet.push_back(totalTaskTime / (2 * cplex.getObjValue()));
		sort(subsequence.begin(), subsequence.end(), comp_work);
		sequences.push_back(subsequence);
	}
	setSequenceSet(sequences);

	// print sequence
	for (size_t j = 0; j < num_stations; j++)
	{
		vector<tuple<int, int, int>> sequence = getSequenceOnStation(j);
		cout << "Station " << (j + 1) << "(" << 0 << "): ";
		for (auto& x : sequence) {
			if (std::get<2>(x) == 0) {
				int work = std::get<0>(x);
				int start_time = std::get<1>(x);
				cout <<"(" << work <<"," << start_time <<"," << start_time + assemblyTimeSet[work - 1] << ")  ";
			}
		}
		cout << endl;
		cout << "Station " << (j + 1) << "(" << 1 << "): ";
		for (auto& x : sequence) {
			if (std::get<2>(x) == 1) {
				int work = std::get<0>(x);
				int start_time = std::get<1>(x);
				cout << "(" << work << "," << start_time << "," << start_time + assemblyTimeSet[work - 1] << ")  ";
			}
		}
		cout << endl<<endl;
	}

	// print balance rate for each work station
	cout << "Balance Rate : ";
	for (IloInt j = 0; j < num_stations; j++)
	{
		cout << balanceRateSet[j] << "  ";
	}
	cout << endl;

	// set average balance rate
	double sumBalanceRate = accumulate(balanceRateSet.begin(), balanceRateSet.end(), 0.0);
	setAverageBalanceRate(sumBalanceRate / num_stations);
	cout << "Average Balance Rate : " << this->averageBalanceRate;
	cout << endl;

}



