#pragma once
#ifndef _INPUT_DATA_H_
#define _INPUT_DATA_H_

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include "SequeGraph.h"

using namespace std;

struct WorkNode {
	int ID;
	int workingTime;
	char type;

	WorkNode(int ID, int workingTime, char type)
		: ID(ID), workingTime(workingTime), type(type)
	{
	}
};
struct WorkEdge {
	int originID;
	int destinationID;

	WorkEdge(int originID, int destinationID)
		: originID(originID), destinationID(destinationID)
	{
	}
};

class InputData
{
public:
	InputData(string& filepath);
	~InputData();

	inline SequeGraph& getGraph() {
		return Graph;
	}
	inline vector<WorkNode>& getWorkSet() {
		return WorkSet;
	}
	inline vector<WorkEdge>& getEdgeSet() {
		return EdgeSet;
	}

private:
	int num_of_work;
	vector<WorkNode> WorkSet;
	vector<WorkEdge> EdgeSet;
	SequeGraph Graph;
};

#endif // !_INPUT_DATA_H_
