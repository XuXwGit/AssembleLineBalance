#pragma once
#ifndef _PUBLICPARA_H_
#define _PUBLICPARA_H_

class pubPara
{
public:
	int POPULATION_SIZE = 30;			//种群大小
	int MAX_GENERATIONS = 60;		//进化代数
	double P_CROSS = 0.7;					//交叉概率
	double P_MUTATION1 = 0.2;			//变异概率1;
	double P_MUTATION2 = 0.2;			//变异概率2;
	int CT = 500;
};

#endif _PUBLICPARA_H_