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
		//ѡ�����ӣ�������Graph(��ʼʱ�䡢������λ��)�����ã�
		population.Select(totalFitness);

		//��������
		population.Mutation();

		//��������
		population.Cross();

		//������Ӧ�ȣ������Կ�ʼʱ�䡢������λ�����ã�
		totalFitness = population.CalculateFitness();

		cout << "-----------------------------------------------------------The " << i + 1 << " Generations result : -----------------------------------------------------------" << endl;
		/*for (int j = 0; j < POPULATION_SIZE; j++)
		{*/
			//population.getPopulation()[i].printGenome();
			//cout <<population.getPopulation()[j].getFitness()<< "  "<<population.getPopulation()[j].getStations();
			//cout << endl;
		//}

		//�����ǰ���Ÿ���
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
