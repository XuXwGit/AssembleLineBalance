#pragma once
#ifndef _PUBLICPARA_H_
#define _PUBLICPARA_H_

class pubPara
{
public:
	int POPULATION_SIZE = 30;			//��Ⱥ��С
	int MAX_GENERATIONS = 60;		//��������
	double P_CROSS = 0.7;					//�������
	double P_MUTATION1 = 0.2;			//�������1;
	double P_MUTATION2 = 0.2;			//�������2;
	int CT = 500;
};

#endif _PUBLICPARA_H_