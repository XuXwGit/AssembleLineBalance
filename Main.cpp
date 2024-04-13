#include<vector>
#include<iomanip>
#include<fstream>
#include<time.h>

#include <ilcplex/ilocplex.h>

#include "SequeGraph.h"
#include "Individual.h"
#include "Population.h"
#include "GA.h"
#include "InputData.h"
#include "Test.h"

using namespace std;

int main()
{
	// initial random sand
	srand((unsigned)time(0));

	vector<string> instanceSet = {"P4",  "P12", "P16", "P24", "P30", "P65", "P148", "P205"};

	for (auto& instance : instanceSet)
	{
		//string instance = "P148";
		string filepath = "Data\\" + instance + "\\";
		InputData data(filepath);
		SequeGraph Graph = data.getGraph();

		Graph.printself();

		testModel(Graph);

		GA GA(Graph);
	}



	return 0;
}