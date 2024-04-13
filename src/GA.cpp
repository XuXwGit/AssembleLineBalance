#include "GA.h"

GA::GA(SequeGraph& graph)
: Graph(graph)
{
	Population population(Graph);

	population.InitializePopulation();

	double totalFitness = population.CalculateFitness();
	clock_t start, end;

	start = clock();
	for (int i = 0; i < MAX_GENERATIONS; i++)
	{
		//选择算子（包含对Graph(开始时间、操作方位等)的重置）
		population.Select(totalFitness);

		//变异算子
		population.Mutation();

		//交叉算子
		population.Cross();

		//计算适应度（包含对开始时间、操作方位的设置）
		totalFitness = population.CalculateFitness();

		cout << "-----------------------------------------------------------The " << i + 1 << " Generations result : -----------------------------------------------------------" << endl;
		/*for (int j = 0; j < POPULATION_SIZE; j++)
		{*/
			//population.getPopulation()[i].printGenome();
			//cout <<population.getPopulation()[j].getFitness()<< "  "<<population.getPopulation()[j].getStations();
			//cout << endl;
		//}

		//输出当前最优个体
		population.getBestIndividual().printIndividual();
	}
	population.getBestIndividual().printIndividual();
	end = clock();

	cout << "The total time cost is " << (double)(end - start) / CLOCKS_PER_SEC << "s" << endl;
	cout << "The programm is ended!" << endl;
}

GA::~GA()
{
}
