#pragma once
#include "graph.h"
#include <iostream>
using namespace std;
extern graph *gp;

class individuality {
public:
	double Fitness;//��Ӧ��
	double value;
	double C;
	double D;
	int N;
	double L;
	vector<int> Chromosome1;//·������
	vector<double> Chromosome2;//�������䷽��
	vector<double> Chromosome3;//��·������������
public:
	individuality(); //���캯��
	//~individuality();//��������
	void getChromosome2();
	void getChromosome3();
	int illegal_road();
	bool illegal_distance(vector<int> single_rode);//�Ƿ񳬳�����
	bool illegal_time(vector<int> single_rode);//�Ƿ�ʱ
	bool illegal_weight(int index_road);//�Ƿ���
	bool illegal_cross(vector<int> single_road);//�Ƿ�ֱ�ӿ�Խ��
	int illegal_car_num();//�Ƿ�������

	double target_value();//Ŀ�꺯��
	double target_C();//�ɱ�
	double target_D();//����
	double single_D(vector<int> single_rode);//������·������
	double single_L(vector<int> single_rode);//������·������Ҫ������
	double target_N();//����
	double target_L();//������

	vector<vector<int>> get_each_road();//��ȡ����ÿ��·��
	int get_num_of_road();
	int get_kind_of_road(int index_road);//·������
	double get_load_of_road(int index_road);//��ȡ·������
	void Fitness_calculation();//��Ӧ�ȼ���
};

//Ⱥ����
class population {
public:
	int Populationsize; //��Ⱥ��ģ
	int Maxevolution; //��������
	int cishu = 0;//��ǰ��������
	int S = 0;//����������δ�ı��Ⱦɫ�����
	vector<individuality> group; //��Ⱥ
public:
	population() {
		Populationsize = 150;//��Ⱥ��ģ
		Maxevolution = 300;//��������
		individuality* p = new individuality[Populationsize];
		for (int i = 0; i < Populationsize; i++)
		{
			group.push_back(*(p++));
		}
	}
	population(int num, int max) {
		Populationsize = num;
		Maxevolution = max;
		individuality* p = new individuality[Populationsize];
		for (int i = 0; i < Populationsize; i++)
		{
			group.push_back(*(p++));
		}
	}
	~population()
	{
		//cout << "The end" << endl;
	}
	void choice(population &r, vector<individuality> &newGroup);//ѡ��
	void overlapping(population &r, individuality &farther, individuality &mother, double favg, double fmax);//����
	void variation(population &r, double favg);//����
	void GA(population &r);
};
