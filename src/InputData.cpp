#include "InputData.h"

InputData::InputData(string& filepath)
{
	//Graph.inputGraph();
	//Graph.outputGraph();

	// Input data of work node and edge node
	ifstream finVertices, finEdges;

	finVertices.open(filepath+"Vertices.txt");
	finEdges.open(filepath + "Edges.txt");

	if (!finVertices.is_open())
	{
		cout << "无法找到文件Vertices.txt！" << endl;
	}
	if (!finEdges.is_open())
	{
		cout << "无法找到文件Edges.txt！" << endl;
	}

	int work, time;
	char direction;
	cout << "add thr Vertices:  work , time , direction :" << endl;
	while (finVertices >> work >> time >> direction)
	{
		cout << work << " " << time << " " << direction << endl;
		WorkSet.push_back(WorkNode(work, time, direction));
		Graph.insertVertex(work, time, direction);
	}
	finVertices.close();

	int work1, work2;
	cout << "add the Edges : " << endl;
	while (finEdges >> work1 >> work2)
	{
		cout << work1 << " " << work2 << endl;
		EdgeSet.push_back(WorkEdge(work1, work2));
		Graph.insertEdge(work1, work2);
	}
	finEdges.close();
}

InputData::~InputData()
{
}