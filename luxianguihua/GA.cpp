#include "ga.h"
#include <math.h>
#include <ctime>

int car_kind;//车辆种类
double car_time;
double unload_time;
double car_speed;
double car_distance;
double W[4];
double P;//惩罚系数
graph *gp;

vector<double> car_weight;//记录每种车的载重量
vector<int> car_num;//记录每种车的数量
vector<double>car_oil;//记录每种车的油费

individuality::individuality()
{
	int customer_node = indexmap.size() - 1;//客户节点编号最大值
	int genesnum = 2 * customer_node + 1;
	for (int i = 0; i < genesnum; i++) {
		Chromosome1.push_back(i);
	}
	int randnum = rand() % 100;
	for (int i = 0; i < randnum; i++)
	{
		random_shuffle((Chromosome1.begin() + 1), (Chromosome1.end() - 1));
	}
	getChromosome3();
	getChromosome2();
	Fitness_calculation();
}

/*individuality::~individuality()
{
	Chromosome1.clear();
	Chromosome2.clear();
	Chromosome3.clear();
}*/

vector<vector<int>> individuality::get_each_road() {//获取染色体子路径
	vector<vector<int>> each_road;
	int customer_node = indexmap.size() - 1;//客户节点编号最大值
	int rode_num = get_num_of_road();//路径数量
	unsigned int j = 0;
	vector<int> single_rode;//子路径
	for (int i = 0; i < rode_num; i++) {
		single_rode.push_back(0);
		j++;
		for (; j < Chromosome1.size(); j++) {
			if (Chromosome1[j] > customer_node) {//若为虚拟配送中心
				continue;
			}
			else if (Chromosome1[j] <= customer_node) {
				single_rode.push_back(Chromosome1[j]);
				if (Chromosome1[j + 1] > customer_node) {
					single_rode.push_back(0);
					j++;
					break;
				}
			}
		}
		each_road.push_back(single_rode);
		single_rode.clear();
	}
	return each_road;
}

int individuality::get_num_of_road()
{
	int num_of_road = 0;
	vector<int> temp(Chromosome1);
	for (unsigned int i = 0; i < temp.size(); i++) {
		if ((unsigned int)temp[i] > indexmap.size() - 1) {
			temp[i] = 0;
		}
	}
	for (unsigned int i = 0; i < temp.size() - 1; i++) {
		if (temp[i] == 0 && temp[i + 1] != 0) {
			num_of_road++;
		}
	}
	return num_of_road;
}





void individuality::getChromosome2() {
	Chromosome2.clear();
	for (unsigned int i = 0; i < Chromosome3.size(); i++) {
		for (int j = 0; j < car_kind; j++) {
			if (car_weight[j] >= Chromosome3[i]) {
				Chromosome2.push_back(car_weight[j]);
				break;
			}
			if (Chromosome3[i] > car_weight[car_kind - 1]) {
				Chromosome2.push_back(car_weight[car_kind - 1]);
				break;
			}
		}
	}
	Chromosome2.push_back(Chromosome3.size());

}

void individuality::getChromosome3() {
	Chromosome3.clear();
	vector<vector<int>> each_road;
	each_road = get_each_road();
	for (unsigned int i = 0; i < each_road.size(); i++) {
		Chromosome3.push_back(single_L(each_road[i]));
	}
}

int individuality::illegal_road() {
	int count = 0;
	vector<vector<int>> each_road;
	each_road = get_each_road();
	for (unsigned int i = 0; i < each_road.size(); i++) {
		if (illegal_distance(each_road[i]) || illegal_time(each_road[i]) || illegal_weight(i)
			/*||illegal_cross(each_road[i])*/) {
			count++;
		}
	}
	count += illegal_car_num();
	return count;
}

bool individuality::illegal_distance(vector<int> single_rode) {
	if (single_D(single_rode) > car_distance)
		return true;
	return false;
}



bool individuality::illegal_time(vector<int> single_rode) {
	double T = single_D(single_rode) / car_speed + ((double)(single_rode.size() - 2)) * unload_time / 60;
	if (T > car_time)
		return true;
	return false;
}

bool individuality::illegal_cross(vector<int> single_road) {
	for (int i = 0; i < single_road.size() - 1; i++) {
		if (gp->xun_bian(single_road[i], single_road[i + 1]) == NULL) {
			return false;
		}

	}
	return true;
}

int individuality::get_kind_of_road(int index_road) {//第几种车
//第几种车
	for (int i = 0; i < car_kind; i++) {
		if (Chromosome2[index_road] == car_weight[i])
			return i + 1;
	}
	return 0;
}

double individuality::get_load_of_road(int index_road) {
	/*for (int i = 0; i < car_kind; i++) {
		if (Chromosome3[index_road] == car_weight[i])
			return (double)(i + 1.0);
	}
	return 0.0;*/
	return Chromosome3[index_road];
}

bool individuality::illegal_weight(int index_road) {
	if (get_load_of_road(index_road) > car_weight[get_kind_of_road(index_road) - 1])
		return true;
	return false;
}

int individuality::illegal_car_num() {
	vector<int> _car_weight;
	for (int i = 0; i < car_kind; i++) {
		_car_weight.push_back(0);
	}
	int num_of_overcar = 0;
	for (unsigned int i = 0; i < Chromosome2.size() - 1; i++) {
		for (int j = 0; j < car_kind; j++) {
			if (Chromosome2[i] == car_weight[j]) {
				_car_weight[j]++;
			}
		}
	}
	for (int i = 0; i < car_kind; i++) {
		if (_car_weight[i] > car_num[i])
			num_of_overcar++;
	}
	return num_of_overcar;
}

double individuality::target_value() {
	double value;
	double S[4];
	S[0] = log((target_C() * 100));//成本
	S[1] = log((target_D() * 100));//距离
	S[2] = log((target_N() * 100));//车辆数
	S[3] = log((target_L() * 100));//满载率
	value = W[0] * S[0] + W[1] * S[1] + W[2] * S[2] + W[3] * S[3];
	this->value = value;
	return value;
}

double individuality::target_C() {
	double Cost = 0;
	int kind;
	vector<vector<int>> each_road;
	each_road = get_each_road();
	for (unsigned int i = 0; i < each_road.size(); i++) {
		kind = get_kind_of_road(i);
		Cost += (single_D(each_road[i]) * car_oil[kind - 1]);
	}
	C = Cost;
	return Cost;
}

double individuality::target_D() {
	double Distance = 0;
	vector<vector<int>> each_road;
	each_road = get_each_road();
	for (unsigned int i = 0; i < each_road.size(); i++) {
		Distance += single_D(each_road[i]);
	}
	D = Distance;
	return Distance;
}

double individuality::single_D(vector<int> single_rode) {
	double distance = 0;
	int original_index1;
	int original_index2;
	for (unsigned int i = 0; i < single_rode.size() - 1; i++) {
		original_index1 = indexmap.find(single_rode[i])->second;
		original_index2 = indexmap.find(single_rode[i + 1])->second;
		distance += dis[original_index1][original_index2];
	}
	return distance;
}

double individuality::single_L(vector<int> single_rode) {
	double load = 0;
	int original_index = 0;
	for (unsigned int i = 0; i < single_rode.size(); i++) {
		original_index = indexmap.find(single_rode[i])->second;
		load += CopyvertexArray[original_index].goods_num;
	}
	return load;
}

double individuality::target_N() {
	N = Chromosome2.back();
	return Chromosome2.back();
}

double individuality::target_L() {
	double Load = 0;
	double load;
	vector<vector<int>> each_road;
	each_road = get_each_road();
	for (unsigned int i = 0; i < each_road.size(); i++) {
		load = get_load_of_road(i) / car_weight[get_kind_of_road(i) - 1];
		if (load > 1)
			load = load - 1;
		Load += load;
	}

	L = Load / each_road.size();
	Load = 1 / (Load / each_road.size());
	return Load;
}

void individuality::Fitness_calculation() {
	int count = illegal_road();
	double value = target_value();
	Fitness = 1 / (value + (double)P * count);
}

//种群的相关操作
void population::choice(population &r, vector<individuality> &newGroup) //选择操作
{
	const int chooseSize = (int)(r.Populationsize * 0.10); //选取组规模， 设置为种群规模的60%
	double maxFitness = r.group[0].Fitness; //最大适应度
	int maxFitnessIndex = 0; //最大适应度个体的索引值
	for (int i = 1; i < r.Populationsize; i++) //找到适应度最大的个体
	{
		if (r.group[i].Fitness > maxFitness)
		{
			maxFitness = r.group[i].Fitness;
			maxFitnessIndex = i;
		}
	}
	class individuality cup; //个体交换位置时的临时变量
	//将种群中适应度最大的个体放在种群数组的第一个
	cup = r.group[0];
	r.group[0] = r.group[maxFitnessIndex];
	r.group[maxFitnessIndex] = cup;

	//开始选择出父代
	newGroup.push_back(r.group[0]);//按照精英保留策略直接将适应度最大的个体放入父代
	for (int k = 0; k < r.Populationsize - 1; k++)
	{
		vector<int> temp; //临时变量，存储产生的随机数列
		for (int i = 1; i < r.Populationsize; i++) //产生数列
		{
			temp.push_back(i);
		}
		int rnum = rand() % 100;
		for (int i = 0; i < rnum; i++)
		{
			random_shuffle(temp.begin(), temp.end()); //打乱数列
		}
		double maxAdaptiveValue = r.group[temp[0]].Fitness; //最大适应度
		int index = temp[0]; //适应度最大的索引值
		for (int i = 0; i < chooseSize - 1; i++) //找到最大适应度
		{
			if (r.group[temp[i]].Fitness > maxAdaptiveValue)
			{
				maxAdaptiveValue = r.group[temp[i]].Fitness;
				index = temp[i];
			}
		}
		newGroup.push_back(r.group[index]); //将选择组中目标函数值最小的个体放入父代数组中
	}
}

void population::overlapping(population &r, individuality &father, individuality &mother, double favg, double fmax)	//交叉操作
{
	double pc = 0.6;	//交叉概率
	double f_;		//要交叉的个体中比较大的适应度
	if (father.Fitness > mother.Fitness)
	{
		f_ = father.Fitness;
	}
	else
	{
		f_ = mother.Fitness;
	}
	double f_max = fmax; //每代种群的最大适应值
	double f_avg = favg; //每代种群的平均适应度
	double pc1 = 0.6, pc2 = 0.3;
	int t = r.cishu; //当前进化代数
	int S = r.S;	 //为进化过程中最优解没改变的个体数目
	int T = r.Maxevolution; //最大终止代数
	int M = r.Populationsize;	//种群大小
	int randtime = (int)(father.Chromosome1.size() * 0.6); //交叉时产生随机数的次数
	if (f_ >= f_avg)	//计算交叉概率
	{
		pc = pc1 - ((pc1 - pc2) * (f_ - f_avg)) / ((f_max - f_avg) * (1 + exp(-t * S / T * M)));
		//cout << "1pc:" << pc << endl;
	}
	else
	{
		pc = pow(pc1, (1 + exp(-t * S / T * M)));
	}
	//产生随机数决定是否发生交叉
	int a = rand() % 100;
	double randnum = (double)a / 100.0;
	//cout << "randnum" << randnum << endl;
	if (randnum >= 0 && randnum <= pc) //发生交叉
	{
		vector<int> dad;
		vector<int> mom;
		for (unsigned int i = 0; i < father.Chromosome1.size(); i++)
		{
			dad.push_back(father.Chromosome1[i]);
			mom.push_back(mother.Chromosome1[i]);
			//father.Chromosome1[i] = -1;
			//mother.Chromosome1[i] = -1;
		}
		int tag = 0;
		for (unsigned int i = 0; i < dad.size(); i++)
		{
			if (dad[i] != mom[i])
			{
				tag = 1;
				break;
			}
		}
		vector<int> temp; //临时变量，存储产生的随机数列
		for (unsigned int i = 1; i < father.Chromosome1.size() - 1; i++) //产生数列
		{
			temp.push_back(i);
		}
		int rnum = rand() % 100;
		for (int n = 0; n < rnum; n++)
		{
			random_shuffle(temp.begin(), temp.end()); //打乱数列
		}
		//开始交叉操作
		for (int i = 0; i < randtime; i++) //将要染色体中要被插入另一条染色体基因的位置内容置为-1
		{
			for (unsigned int j = 0; j < mother.Chromosome1.size(); j++)
			{
				if (mother.Chromosome1[j] == dad[temp[i]])
				{
					mother.Chromosome1[j] = -1;
				}
			}
			for (unsigned int j = 0; j < father.Chromosome1.size(); j++)
			{
				if (father.Chromosome1[j] == mom[temp[i]])
				{
					father.Chromosome1[j] = -1;
				}
			}
		}
		if (tag == 0)
		{
			for (int i = 0; i < randtime; i++) //将要染色体中要被插入另一条染色体基因的位置内容置为-1
			{
				for (unsigned int j = 0; j < mother.Chromosome1.size(); j++)
				{
					if (mother.Chromosome1[j] == -1)
					{
						mother.Chromosome1[j] = dad[temp[i]];
						break;
					}
				}
				for (unsigned int j = 0; j < father.Chromosome1.size(); j++)
				{
					if (father.Chromosome1[j] == -1)
					{
						father.Chromosome1[j] = mom[temp[i]];
						break;
					}
				}
			}
		}
		else
		{
			//开始填入基因
			for (int i = 0; i < randtime; i++) //往母体基因中添加父体基因，产生新子体1
			{
				for (unsigned int j = 0; j < dad.size(); j++)
				{
					int flag = 0;
					for (int k = 0; k < randtime; k++)
					{
						if (dad[j] == dad[temp[k]])
						{
							flag = 1;
							break;
						}
					}
					if (flag == 1)
					{
						for (unsigned int k = 0; k < mother.Chromosome1.size(); k++)
						{
							if (mother.Chromosome1[k] == -1)
							{
								mother.Chromosome1[k] = dad[j];
								break;
							}
						}
					}
					else
					{
						continue;
					}
				}
			}
			for (int i = 0; i < randtime; i++) //往父体基因中插入母体基因，产生新子体2
			{
				for (unsigned int j = 0; j < mom.size(); j++)
				{
					int flag = 0;
					for (int k = 0; k < randtime; k++)
					{
						if (mom[j] == mom[temp[k]])
						{
							flag = 1;
							break;
						}
					}
					if (flag == 1)
					{
						for (unsigned int k = 0; k < father.Chromosome1.size(); k++)
						{
							if (father.Chromosome1[k] == -1)
							{
								father.Chromosome1[k] = mom[j];
								break;
							}
						}
					}
					else
					{
						continue;
					}
				}
			}
		}
		//对交叉产生的新个体重新进行车辆方案、单条路径配送量、适应度的计算
		father.getChromosome3();
		father.getChromosome2();
		father.Fitness_calculation();
		mother.getChromosome3();
		mother.getChromosome2();
		mother.Fitness_calculation();
	}
}

void population::variation(population &r, double favg) {
	double pm1 = 0.01;
	double pm2 = 0.002;//变异概率参数
	double pm;
	int T = r.Maxevolution;
	int populationsize = r.Populationsize;//种群规模
	double K = 1.0 + exp((double)r.cishu *((double)r.S / (double)T) * (double)populationsize);
	double fmax = r.group[0].Fitness;//原种群最大适应度
	for (int j = 1; j < populationsize; j++)//变异过程
	{
		double f = r.group[j].Fitness;
		if (f >= favg)
		{
			pm = pm1 - ((pm1 - pm2) * (fmax - f)) / ((fmax - favg) * K);
			//cout << "PM1:" << pm << endl;
		}
		else
		{
			pm = (double)pow(pm1, K);
			//cout << "PM2:" << pm << endl;
		}
		int a = rand() % 10000000;
		double num1 = (double)a / 10000000.0;
		//cout << "num1:" << num1 << endl;
		if (num1 < pm) {
			/*cout << "变异前：";
			for (int n = 0; n < r.group[j].Chromosome1.size(); n++)
			{
				cout << r.group[j].Chromosome1[n] << " ";
			}
			cout << endl;*/
			for (int i = 0; i < 3; i++) {
				int num2 = 1 + rand() % (r.group[j].Chromosome1.size() - 2 - 1 + 1);//随机产生变异位置1
				int num3 = 1 + rand() % (r.group[j].Chromosome1.size() - 2 - 1 + 1);//随机产生变异位置2
				while (num2 == num3) {
					num3 = 1 + rand() % (r.group[j].Chromosome1.size() - 2 - 1 + 1);//随机产生变异位置2
				}
				//cout << "num2:" << num2 << endl;
				//cout << "num3:" << num3 << endl;
				int m = r.group[j].Chromosome1[num2];
				r.group[j].Chromosome1[num2] = r.group[j].Chromosome1[num3];//交换
				r.group[j].Chromosome1[num3] = m;

			}
			/*cout << "变异后：";
			for (int n = 0; n < r.group[j].Chromosome1.size(); n++)
			{
				cout << r.group[j].Chromosome1[n] << " ";
			}
			cout << endl << endl;*/
		}
	}

	for (unsigned int i = 0; i < r.group.size() - 1; i++) { //从第二个个体开始更新适应度值
		r.group[i].getChromosome3();
		r.group[i].getChromosome2();
		r.group[i].Fitness_calculation();
	}
	int x = 0;//最大值下标
	double max = r.group[0].Fitness;//假设第一条染色体是最大的值，所以下面循环的就不用比较循环a[0]了，已经是最大的值了就没必须再循环一次和其他值比较了  
	for (unsigned int i = 1; i < r.group.size(); i++) //从第二个个体开始比较
	{
		if (r.group[i].Fitness > max) //这行的i表示下标
		{
			max = r.group[i].Fitness;
			x = i;
		}
	}
	double maxFitness = r.group[0].Fitness; //最大适应度
	int maxFitnessIndex = 0; //最大适应度个体的索引值
	for (int i = 1; i < r.Populationsize; i++) //找到适应度最大的个体
	{
		if (r.group[i].Fitness > maxFitness)
		{
			maxFitness = r.group[i].Fitness;
			maxFitnessIndex = i;
		}
	}
	class individuality cup; //个体交换位置时的临时变量
	//将种群中适应度最大的个体放在种群数组的第一个
	cup = r.group[0];
	r.group[0] = r.group[maxFitnessIndex];
	r.group[maxFitnessIndex] = cup;
	if (x == 0) {
		S++;
		cishu++;
		return;
	}
	else
		cishu++;
	return;
}

void population::GA(population &r)
{
	int time_ = r.Maxevolution / 100;
	int temp = 0;
	for (int k = 0; k < r.Maxevolution; k++)
	{
		temp++;
		if (temp == time_) {
			temp = 0;
		}
		double f_avg = 0;
		for (unsigned int i = 0; i < group.size(); i++)
		{
			f_avg += group[i].Fitness;
		}
		f_avg = f_avg / group.size();
		
		vector<individuality> newGroup;
		choice(r, newGroup);
		double f_max = newGroup[0].Fitness;
		for (int i = 1; i < (int)(((newGroup.size() - 1) / 2) + 1); i++)
		{
			overlapping(r, newGroup[2 * i - 1], newGroup[2 * i], f_avg, f_max);
		}
		r.group = newGroup;
		variation(r, f_avg);
	}
}

void graph::Init(string _str_path) {
	FILE* fp;
	fopen_s(&fp,(char*)_str_path.data(), "r");
	if (fp == NULL) {
		printf("can not open file!\n");
		exit(0);
	}
	//fscanf_s(fp, "%lf %lf %lf %lf %lf", &W[0], &W[1], &W[2], &W[3], &P);
	//fscanf_s(fp, "%lf %lf %lf %lf", &car_distance, &car_speed, &car_time, &unload_time);
	//fscanf_s(fp, "%d", &car_kind);
	//car_weight.resize(car_kind);
	//car_num.resize(car_kind);
	//car_oil.resize(car_kind);
	/*for (int i = 0; i < car_kind; i++) {
		fscanf_s(fp, "%lf", &car_weight[i]);
	}
	for (int i = 0; i < car_kind; i++) {
		fscanf_s(fp, "%d", &car_num[i]);
	}
	for (int i = 0; i < car_kind; i++) {
		fscanf_s(fp, "%lf", &car_oil[i]);
	}*/
	int vertexNum; //节点数目
	int edgeNum;
	char p1[5];
	string s;
	int kind;
	double weight;
	fscanf_s(fp, "%d", &vertexNum);
	for (int i = 0; i < vertexNum; i++) {
		p1[0] = { '\0' };
		fscanf_s(fp, "%s", p1, _countof(p1));
		fscanf_s(fp, "%d", &kind);
		fscanf_s(fp, "%lf", &weight);
		s = p1;
		node_init1(s, kind, weight, i);
	}
	fscanf_s(fp, "%d", &edgeNum);
	char  p2[5];
	char  p3[5];
	string s2;
	string s3;
	double value;
	for (int i = 0; i < edgeNum; i++) {
		p2[0] = { '\0' };
		p3[0] = { '\0' };
		fscanf_s(fp, "%s", p2, _countof(p2));
		fscanf_s(fp, "%s", p3, _countof(p3));
		fscanf_s(fp, "%lf", &value);
		s2 = p2;
		s3 = p3;
		edge_init1(s2, s3, value);
	}
	upgrade(vertexArray);
	fclose(fp);
}


vector<vector<string> > find_path(vector<vector<int>> &each_road, int roadnum) {
	vector<vector<string>> find_path(roadnum);
	vector<int> road;
	int k;
	unsigned int j = 0;
	for (unsigned int i = 0; i < each_road.size(); i++) {
		for (j = 0; j < each_road[i].size() - 1; j++) {
			k = path[(each_road[i][j])][(each_road[i][j + 1])];
			road.push_back(each_road[i][j]);
			while (k != each_road[i][j + 1]) {
				road.push_back(k);
				k = path[k][(each_road[i][j + 1])];
			}

		}
		road.push_back(each_road[i][j]);
		each_road[i].clear();
		for (unsigned int k = 0; k < road.size(); k++) {
			each_road[i].push_back(road[k]);
		}
		road.clear();
	}
	for (unsigned int i = 0; i < roadnum; i++) {
		for (unsigned int j = 0; j < each_road[i].size(); j++) {
			find_path[i].push_back(CopyvertexArray[each_road[i][j]].name);
		}
	}
	return find_path;
}

//主函数
/*int main()
{
	int GAtime = 1; //遗传算法次数
	srand((int)(time(0)));
	graph p;
	p.Init("");
	floyd(p);
	getCopyvertexArray(p);
	individuality bestOne;
	for (int i = 0; i < GAtime; i++)
	{
		cout << "第" << i + 1 << "次遗传算法结果" << endl;
		population a;
		a.GA(a);
		cout << "最优方案为：";
		for (unsigned int j = 0; j < a.group[0].Chromosome1.size(); j++)
		{
			cout << a.group[0].Chromosome1[j] << " ";
		}
		cout << endl << "车辆分配方案为：";
		for (unsigned int j = 0; j < a.group[0].Chromosome2.size(); j++)
		{
			cout << a.group[0].Chromosome2[j] << " ";
		}
		cout << endl;
		cout << "最大适应度：" << a.group[0].Fitness;
		if (bestOne.Fitness < a.group[0].Fitness)
		{
			bestOne = a.group[0];
		}
		cout << endl << endl;
	}
	cout << endl << "最优方案为：";
	/*for (unsigned int i = 0; i < bestOne.Chromosome1.size(); i++)
	{
		cout << bestOne.Chromosome1[i] << " " ;
	}
	cout << endl << "车辆分配方案为：";
	for (unsigned int i = 0; i < bestOne.Chromosome2.size(); i++)
	{
		cout << bestOne.Chromosome2[i] << " ";
	}
	cout << endl;
	cout << "最优适应度：" << bestOne.Fitness <<endl;

	vector<vector<int>> each_road = bestOne.get_each_road();
	unsigned int roadnum = bestOne.Chromosome2.size() - 1;
	for (unsigned i = 0; i < each_road.size(); i++) {
		for (unsigned j = 0; j < each_road[i].size(); j++) {
			each_road[i][j] = indexmap[(each_road[i][j])];
		}
	}
	vector<vector<string>> bestpath = find_path(each_road, roadnum);
	vector<double> bestcar = bestOne.Chromosome2;
	cout << "最终分配方案为: " << endl;
	for (unsigned int i = 0; i < bestpath.size(); i++) {
		cout << "第" << i + 1 << "种分配方案为: " << endl;
		for (unsigned int j = 0; j < bestpath[i].size(); j++) {
			cout << bestpath[i][j];
			if (j != bestpath[i].size() - 1) {
				cout << "->";
			}
		}
		cout << "车辆分配方案为" << bestOne.Chromosome2[i] << endl;
	}

	cout << "byebye" << endl;
	system("pause");
	return 0;

}*/
