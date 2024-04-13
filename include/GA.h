#pragma once
#ifndef _GA_H_
#define _GA_H_

#include "SequeGraph.h"
#include "Population.h"
#include "publicPara.h"

using namespace std;

class GA : public pubPara
{
private:
	SequeGraph Graph;

public:
	GA(SequeGraph& graph);
	~GA();
};

#endif