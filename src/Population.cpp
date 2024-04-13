#include "Population.h"

Population::Population()
	: population(vector<Individual>(POPULATION_SIZE)), populationSize(POPULATION_SIZE)
{
}

Population::Population(int populationSize)
	: population(vector<Individual>(populationSize)), populationSize(populationSize)
{
}

Population::Population(int populationSize, const SequeGraph& graph)
	: population(vector<Individual>(populationSize)), populationSize(populationSize), Graph(graph)
{
}

Population::Population(const SequeGraph& graph)
	: population(vector<Individual>(POPULATION_SIZE)), populationSize(POPULATION_SIZE), Graph(graph)
{
}

void Population::InitializePopulation()
{
	for (int i = 0; i < populationSize; i++)
	{
		population[i].InitializeGenome(Graph);
	}
}

double Population::CalculateFitness()
{

	double totalFitness = 0;
	for (int i = 0; i < populationSize; i++)
	{
		this->population[i].CaculateIndividualFitness();
		totalFitness += population[i].getFitness();
	}
	Individual lastBestIndividual = this->lastBestIndividual();
	Individual currentBestIndividual = this->getBestIndividual();
	if (lastBestIndividual.getFitness() > currentBestIndividual.getFitness())
	{

		totalFitness += lastBestIndividual.getFitness() - population[0].getFitness();
		population[0] = lastBestIndividual;
	}

	//������Ӧ��֮��
	return totalFitness;
}

void Population::CalculateProbability(double totalFitness)
{
	//double totalFitness = CalculateFitness();
	double selectProbability = 0;
	double accumulateProbability = 0;;

	for (int i = 0; i < populationSize; i++)
	{
		selectProbability = population[i].getFitness() / totalFitness;
		accumulateProbability += selectProbability;
		population[i].setSelectProbability(selectProbability);
		population[i].setAccumulateProbability(accumulateProbability);
	}
}

void Population::Roulutte(vector<Individual>& tempPopulation)
{
	for (int i = 0; i < populationSize; i++)
	{
		double probability = (double)rand() / (RAND_MAX + 1.0);
		for (int j = 0; j < populationSize; j++)
		{
			if (probability < population[j].getAccumulateProbability())
			{
				tempPopulation[i].setGenome(population[j].getGenome());
				tempPopulation[i].setSelectProbability(population[i].getSelectProbability());
				tempPopulation[i].setGraph(Graph);
				break;
			}
		}
	}
}

void Population::Select(double totalFitness)
{
	this->CalculateProbability(totalFitness);

	vector<Individual> tempPopulation(populationSize);

	Roulutte(tempPopulation);

	for (int i = 0; i < populationSize; i++)
	{
		population[i] = tempPopulation[i];
	}
}

void Population::CrossIndividual(Individual& individual1, Individual& individual2)
{
	SequeGraph graph1(Graph);
	SequeGraph graph2(Graph);
	int GenomeSize = individual1.getGenome().size();
	int h1 = rand() % (GenomeSize);
	int h2 = h1 + rand() % (GenomeSize - h1);
	vector<int> in1(individual1.getGenome());
	vector<int> in2(individual2.getGenome());
	vector<int> temp1(GenomeSize, -1);
	vector<int> temp2(GenomeSize, -1);

	set<int> RS1;
	set<int> RS2;
	set<int> PS1;
	set<int> PS2;

	//����λ�ü�����ֵ
	for (int i = 0; i < GenomeSize; i++)
	{
		PS1.insert(i);
		PS2.insert(i);
	}

	//Step1: �ֱ���[0, h1) �εĶԷ���ͬ�����Ӵ�ȥ
	for (int i = 0; i < h1; i++)
	{
		int j;
		//��� ����in1[i]�Ƿ������in2[0~h1)
		for (j = 0; j < h1; j++)
		{
			if (in1[i] == in2[j])
			{
				temp1[j] = in2[j];
				graph1.removeVertex(temp1[j]);
				PS1.erase(j);
				break;
			}
		}
		//������������ٷ��伯��RS1��
		if (j == h1)
		{
			RS1.insert(in1[i]);
		}
		//��� ����in2[i]�Ƿ������in1[0~h1)
		for (j = 0; j < h1; j++)
		{
			if (in2[i] == in1[j])
			{
				temp2[j] = in1[j];
				graph2.removeVertex(temp2[j]);
				PS2.erase(j);
				break;
			}
		}
		//������������ٷ��伯��RS2��
		if (j == h1)
		{
			RS2.insert(in2[i]);
		}
	}

	//Step2: �ֱ𽻻�[h1, h2) �ε�ȫ���Լ������Ӵ�ȥ
	for (int i = h1; i < h2; i++)
	{
		temp1[i] = individual1.getGenome()[i];
		graph1.removeVertex(temp1[i]);
		PS1.erase(i);
		temp2[i] = individual2.getGenome()[i];
		graph2.removeVertex(temp2[i]);
		PS2.erase(i);
	}

	//Step3: �ֱ���[h2, GenomeSize) �ε���ͬ�����Ӵ�ȥ
	for (int i = h2; i < GenomeSize; i++)
	{
		int j;
		//��� ����in1[i]�Ƿ������in2[0~h1)
		for (j = h2; j < GenomeSize; j++)
		{
			if (in1[i] == in2[j])
			{
				temp1[j] = in2[j];
				graph1.removeVertex(temp1[j]);
				PS1.erase(j);
				break;
			}
		}
		//������������ٷ��伯��RS1��
		if (j == GenomeSize)
		{
			RS1.insert(in1[i]);
		}
		//��� ����in2[i]�Ƿ������in1[0~h1)
		for (j = h2; j < GenomeSize; j++)
		{
			if (in2[i] == in1[j])
			{
				temp2[j] = in1[j];
				graph2.removeVertex(temp2[j]);
				PS2.erase(j);
				break;
			}
		}
		//������������ٷ��伯��RS2��
		if (j == GenomeSize)
		{
			RS2.insert(in2[i]);
		}
	}

	//Step4: �ٷ�����ԣ���RS1��RS2����δ����Ļ���ֱ���䵽��Ӧ�Ӵ�PS1��PS2��
	redistribute(temp1, graph1, RS1, PS1);
	redistribute(temp2, graph2, RS2, PS2);

	//step5: ���Ӵ�Ⱦɫ��temp1��temp2��ֵ������
	individual1.setGenome(temp1);
	individual2.setGenome(temp2);
}

int Population::getBeginPos(int work, vector<int>& genome)
{
	int pos = 0;
	for (auto x : Graph.getFrontVertexs(work))
	{
		int pos_x = find(genome.begin(), genome.end(), x) - genome.begin();
		if (pos_x > pos)
		{
			pos = pos_x;
		}
	}
	return pos;
}

int Population::getEndPos(int work, vector<int>& genome)
{
	int pos = Graph.numberOfVertices() - 1;
	for (int x : Graph.getNextVertexs(work))
	{
		int pos_x = find(genome.begin(), genome.end(), x) - genome.begin();
		if (pos > pos_x)
		{
			pos = pos_x;
		}
	}
	return pos;
}

vector<Individual>& Population::getPopulation()
{
	return this->population;
}

void Population::Cross()
{
	////���ѡ��һ������j��Ϊ parent2
	//int j = rand() % (populationSize );
	//a wrong methods

	for (int i = 0; i < populationSize; i++)
	{
		double CrossProbability1 = (double)rand() / (RAND_MAX + 1.0);
		if (CrossProbability1 < P_CROSS)
		{
			for (int j = i + 1; j < populationSize; j++)
			{
				double CrossProbability2 = (double)rand() / (RAND_MAX + 1.0);
				if (CrossProbability2 < P_CROSS)
				{
					CrossIndividual(population[i], population[j]);
					break;
				}
			}
		}
	}
}

void Population::redistribute(vector<int>& genome, SequeGraph& genomeGraph, set<int>& RS, set<int>& PS)
{
	if (PS.size() == RS.size())
	{
		while (!RS.empty())
		{
			set<int>::iterator it = RS.begin();
			int r = rand() % (RS.size());
			advance(it, r);
			if (it != RS.end() && !genomeGraph.getInDegree(*it))
			{
				int work = *it;

				//�����������½�
				int beginPos = this->getBeginPos(work, genome);
				int endPos = this->getEndPos(work, genome);

				int s = PS.size();
				for (int i = 0; i < s; i++)
				{
					set<int>::iterator x = PS.begin();
					advance(x, i);
					if (x != PS.end())
					{
						//����������
						//�����ϲ�����֡�=�����������Ϊ*xΪδ���ŵ�λ��
						if (*x >= beginPos && *x <= endPos)
						{
							int pos = *x;
							genome[pos] = work;
							genomeGraph.removeVertex(work);
							PS.erase(pos);
							RS.erase(work);
							break;
						}
						//if (s == 1 && *x < beginPos)
						//�����������
						else if (*x < beginPos)
						{
							int i = *x;
							PS.erase(*x);
							for (; i < beginPos; i++)
							{
								if (genome[i + 1] == -1)			//���λ�����*x+1��PS���棬��*x+1����δ*x
								{
									PS.erase(i + 1);
									PS.insert(i);
								}
								genome[i] = genome[i + 1];
							}
							genome[beginPos] = work;
							genomeGraph.removeVertex(work);
							RS.erase(work);
							break;
						}
						//���������Ҳ�
						else if (*x > endPos)
						{
							int i = *x;
							PS.erase(*x);
							for (; i > endPos; i--)
							{
								if (genome[i - 1] == -1)			//���λ�����*x+1��PS���棬��*x+1����δ*x
								{
									PS.erase(i - 1);
									PS.insert(i);
								}
								genome[i] = genome[i - 1];
							}
							genome[endPos] = work;
							genomeGraph.removeVertex(work);
							RS.erase(work);
							break;
						}
					}
				}
			}
		}
	}
	else
	{
		cout << "error: RS.size() != PS.size()!";
	}
}

void Population::Mutation()
{
	int GenomeSize = population[0].getGenome().size();

	for (int i = 0; i < populationSize; i++)
	{
		SequeGraph genomeGraph(Graph);
		set<int> RS;
		set<int> PS;
		double probability = (double)rand() / (RAND_MAX + 1.0);
		if (probability < P_MUTATION1)
		{
			vector<int> genome = population[i].getGenome();

			for (int j = 0; j < GenomeSize; j++)
			{
				double probability2 = (double)rand() / (RAND_MAX + 1.0);
				int work = genome[j];
				if (probability2 < P_MUTATION2)				//��ѡ��Ļ�������ٷ��伯��
				{
					RS.insert(work);
					PS.insert(j);
					genome[j] = -1;
				}
				else                          //δ��ѡ��Ļ��򲻱����ͼ��ɾȥ(�Ѱ���)
				{
					genomeGraph.removeVertex(work);
				}
			}

			//�ٷ���
			redistribute(genome, genomeGraph, RS, PS);
			population[i].setGenome(genome);
		}
	}
}

Individual& Population::lastBestIndividual()
{
	return bestIndividual;
}

Individual& Population::getBestIndividual()
{
	//Individual bestIndividual;
	for (int i = 0; i < populationSize; i++)
	{
		if (population[i].getFitness() >= this->bestIndividual.getFitness())
		{
			this->bestIndividual = population[i];
		}
	}
	return this->bestIndividual;
}