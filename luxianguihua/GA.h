#pragma once
#include "graph.h"
#include <iostream>
using namespace std;
extern graph *gp;

class individuality {
public:
	double Fitness;//适应度
	double value;
	double C;
	double D;
	int N;
	double L;
	vector<int> Chromosome1;//路径编码
	vector<double> Chromosome2;//车辆分配方案
	vector<double> Chromosome3;//子路径货物需求量
public:
	individuality(); //构造函数
	//~individuality();//析构函数
	void getChromosome2();
	void getChromosome3();
	int illegal_road();
	bool illegal_distance(vector<int> single_rode);//是否超出距离
	bool illegal_time(vector<int> single_rode);//是否超时
	bool illegal_weight(int index_road);//是否超重
	bool illegal_cross(vector<int> single_road);//是否直接跨越点
	int illegal_car_num();//非法车辆数

	double target_value();//目标函数
	double target_C();//成本
	double target_D();//距离
	double single_D(vector<int> single_rode);//返回子路径距离
	double single_L(vector<int> single_rode);//返回子路径所需要的载重
	double target_N();//车数
	double target_L();//载重率

	vector<vector<int>> get_each_road();//获取方案每条路径
	int get_num_of_road();
	int get_kind_of_road(int index_road);//路径种类
	double get_load_of_road(int index_road);//获取路径载重
	void Fitness_calculation();//适应度计算
};

//群体类
class population {
public:
	int Populationsize; //种群规模
	int Maxevolution; //最大迭代数
	int cishu = 0;//当前迭代次数
	int S = 0;//进化过程中未改变的染色体个数
	vector<individuality> group; //种群
public:
	population() {
		Populationsize = 150;//种群规模
		Maxevolution = 300;//迭代次数
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
	void choice(population &r, vector<individuality> &newGroup);//选择
	void overlapping(population &r, individuality &farther, individuality &mother, double favg, double fmax);//交叉
	void variation(population &r, double favg);//变异
	void GA(population &r);
};
