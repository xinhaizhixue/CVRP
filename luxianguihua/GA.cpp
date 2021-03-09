#include "ga.h"
#include <math.h>
#include <ctime>

int car_kind;//��������
double car_time;
double unload_time;
double car_speed;
double car_distance;
double W[4];
double P;//�ͷ�ϵ��
graph *gp;

vector<double> car_weight;//��¼ÿ�ֳ���������
vector<int> car_num;//��¼ÿ�ֳ�������
vector<double>car_oil;//��¼ÿ�ֳ����ͷ�

individuality::individuality()
{
	int customer_node = indexmap.size() - 1;//�ͻ��ڵ������ֵ
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

vector<vector<int>> individuality::get_each_road() {//��ȡȾɫ����·��
	vector<vector<int>> each_road;
	int customer_node = indexmap.size() - 1;//�ͻ��ڵ������ֵ
	int rode_num = get_num_of_road();//·������
	unsigned int j = 0;
	vector<int> single_rode;//��·��
	for (int i = 0; i < rode_num; i++) {
		single_rode.push_back(0);
		j++;
		for (; j < Chromosome1.size(); j++) {
			if (Chromosome1[j] > customer_node) {//��Ϊ������������
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

int individuality::get_kind_of_road(int index_road) {//�ڼ��ֳ�
//�ڼ��ֳ�
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
	S[0] = log((target_C() * 100));//�ɱ�
	S[1] = log((target_D() * 100));//����
	S[2] = log((target_N() * 100));//������
	S[3] = log((target_L() * 100));//������
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

//��Ⱥ����ز���
void population::choice(population &r, vector<individuality> &newGroup) //ѡ�����
{
	const int chooseSize = (int)(r.Populationsize * 0.10); //ѡȡ���ģ�� ����Ϊ��Ⱥ��ģ��60%
	double maxFitness = r.group[0].Fitness; //�����Ӧ��
	int maxFitnessIndex = 0; //�����Ӧ�ȸ��������ֵ
	for (int i = 1; i < r.Populationsize; i++) //�ҵ���Ӧ�����ĸ���
	{
		if (r.group[i].Fitness > maxFitness)
		{
			maxFitness = r.group[i].Fitness;
			maxFitnessIndex = i;
		}
	}
	class individuality cup; //���彻��λ��ʱ����ʱ����
	//����Ⱥ����Ӧ�����ĸ��������Ⱥ����ĵ�һ��
	cup = r.group[0];
	r.group[0] = r.group[maxFitnessIndex];
	r.group[maxFitnessIndex] = cup;

	//��ʼѡ�������
	newGroup.push_back(r.group[0]);//���վ�Ӣ��������ֱ�ӽ���Ӧ�����ĸ�����븸��
	for (int k = 0; k < r.Populationsize - 1; k++)
	{
		vector<int> temp; //��ʱ�������洢�������������
		for (int i = 1; i < r.Populationsize; i++) //��������
		{
			temp.push_back(i);
		}
		int rnum = rand() % 100;
		for (int i = 0; i < rnum; i++)
		{
			random_shuffle(temp.begin(), temp.end()); //��������
		}
		double maxAdaptiveValue = r.group[temp[0]].Fitness; //�����Ӧ��
		int index = temp[0]; //��Ӧ����������ֵ
		for (int i = 0; i < chooseSize - 1; i++) //�ҵ������Ӧ��
		{
			if (r.group[temp[i]].Fitness > maxAdaptiveValue)
			{
				maxAdaptiveValue = r.group[temp[i]].Fitness;
				index = temp[i];
			}
		}
		newGroup.push_back(r.group[index]); //��ѡ������Ŀ�꺯��ֵ��С�ĸ�����븸��������
	}
}

void population::overlapping(population &r, individuality &father, individuality &mother, double favg, double fmax)	//�������
{
	double pc = 0.6;	//�������
	double f_;		//Ҫ����ĸ����бȽϴ����Ӧ��
	if (father.Fitness > mother.Fitness)
	{
		f_ = father.Fitness;
	}
	else
	{
		f_ = mother.Fitness;
	}
	double f_max = fmax; //ÿ����Ⱥ�������Ӧֵ
	double f_avg = favg; //ÿ����Ⱥ��ƽ����Ӧ��
	double pc1 = 0.6, pc2 = 0.3;
	int t = r.cishu; //��ǰ��������
	int S = r.S;	 //Ϊ�������������Ž�û�ı�ĸ�����Ŀ
	int T = r.Maxevolution; //�����ֹ����
	int M = r.Populationsize;	//��Ⱥ��С
	int randtime = (int)(father.Chromosome1.size() * 0.6); //����ʱ����������Ĵ���
	if (f_ >= f_avg)	//���㽻�����
	{
		pc = pc1 - ((pc1 - pc2) * (f_ - f_avg)) / ((f_max - f_avg) * (1 + exp(-t * S / T * M)));
		//cout << "1pc:" << pc << endl;
	}
	else
	{
		pc = pow(pc1, (1 + exp(-t * S / T * M)));
	}
	//��������������Ƿ�������
	int a = rand() % 100;
	double randnum = (double)a / 100.0;
	//cout << "randnum" << randnum << endl;
	if (randnum >= 0 && randnum <= pc) //��������
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
		vector<int> temp; //��ʱ�������洢�������������
		for (unsigned int i = 1; i < father.Chromosome1.size() - 1; i++) //��������
		{
			temp.push_back(i);
		}
		int rnum = rand() % 100;
		for (int n = 0; n < rnum; n++)
		{
			random_shuffle(temp.begin(), temp.end()); //��������
		}
		//��ʼ�������
		for (int i = 0; i < randtime; i++) //��ҪȾɫ����Ҫ��������һ��Ⱦɫ������λ��������Ϊ-1
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
			for (int i = 0; i < randtime; i++) //��ҪȾɫ����Ҫ��������һ��Ⱦɫ������λ��������Ϊ-1
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
			//��ʼ�������
			for (int i = 0; i < randtime; i++) //��ĸ���������Ӹ�����򣬲���������1
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
			for (int i = 0; i < randtime; i++) //����������в���ĸ����򣬲���������2
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
		//�Խ���������¸������½��г�������������·������������Ӧ�ȵļ���
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
	double pm2 = 0.002;//������ʲ���
	double pm;
	int T = r.Maxevolution;
	int populationsize = r.Populationsize;//��Ⱥ��ģ
	double K = 1.0 + exp((double)r.cishu *((double)r.S / (double)T) * (double)populationsize);
	double fmax = r.group[0].Fitness;//ԭ��Ⱥ�����Ӧ��
	for (int j = 1; j < populationsize; j++)//�������
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
			/*cout << "����ǰ��";
			for (int n = 0; n < r.group[j].Chromosome1.size(); n++)
			{
				cout << r.group[j].Chromosome1[n] << " ";
			}
			cout << endl;*/
			for (int i = 0; i < 3; i++) {
				int num2 = 1 + rand() % (r.group[j].Chromosome1.size() - 2 - 1 + 1);//�����������λ��1
				int num3 = 1 + rand() % (r.group[j].Chromosome1.size() - 2 - 1 + 1);//�����������λ��2
				while (num2 == num3) {
					num3 = 1 + rand() % (r.group[j].Chromosome1.size() - 2 - 1 + 1);//�����������λ��2
				}
				//cout << "num2:" << num2 << endl;
				//cout << "num3:" << num3 << endl;
				int m = r.group[j].Chromosome1[num2];
				r.group[j].Chromosome1[num2] = r.group[j].Chromosome1[num3];//����
				r.group[j].Chromosome1[num3] = m;

			}
			/*cout << "�����";
			for (int n = 0; n < r.group[j].Chromosome1.size(); n++)
			{
				cout << r.group[j].Chromosome1[n] << " ";
			}
			cout << endl << endl;*/
		}
	}

	for (unsigned int i = 0; i < r.group.size() - 1; i++) { //�ӵڶ������忪ʼ������Ӧ��ֵ
		r.group[i].getChromosome3();
		r.group[i].getChromosome2();
		r.group[i].Fitness_calculation();
	}
	int x = 0;//���ֵ�±�
	double max = r.group[0].Fitness;//�����һ��Ⱦɫ��������ֵ����������ѭ���ľͲ��ñȽ�ѭ��a[0]�ˣ��Ѿ�������ֵ�˾�û������ѭ��һ�κ�����ֵ�Ƚ���  
	for (unsigned int i = 1; i < r.group.size(); i++) //�ӵڶ������忪ʼ�Ƚ�
	{
		if (r.group[i].Fitness > max) //���е�i��ʾ�±�
		{
			max = r.group[i].Fitness;
			x = i;
		}
	}
	double maxFitness = r.group[0].Fitness; //�����Ӧ��
	int maxFitnessIndex = 0; //�����Ӧ�ȸ��������ֵ
	for (int i = 1; i < r.Populationsize; i++) //�ҵ���Ӧ�����ĸ���
	{
		if (r.group[i].Fitness > maxFitness)
		{
			maxFitness = r.group[i].Fitness;
			maxFitnessIndex = i;
		}
	}
	class individuality cup; //���彻��λ��ʱ����ʱ����
	//����Ⱥ����Ӧ�����ĸ��������Ⱥ����ĵ�һ��
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
	int vertexNum; //�ڵ���Ŀ
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

//������
/*int main()
{
	int GAtime = 1; //�Ŵ��㷨����
	srand((int)(time(0)));
	graph p;
	p.Init("");
	floyd(p);
	getCopyvertexArray(p);
	individuality bestOne;
	for (int i = 0; i < GAtime; i++)
	{
		cout << "��" << i + 1 << "���Ŵ��㷨���" << endl;
		population a;
		a.GA(a);
		cout << "���ŷ���Ϊ��";
		for (unsigned int j = 0; j < a.group[0].Chromosome1.size(); j++)
		{
			cout << a.group[0].Chromosome1[j] << " ";
		}
		cout << endl << "�������䷽��Ϊ��";
		for (unsigned int j = 0; j < a.group[0].Chromosome2.size(); j++)
		{
			cout << a.group[0].Chromosome2[j] << " ";
		}
		cout << endl;
		cout << "�����Ӧ�ȣ�" << a.group[0].Fitness;
		if (bestOne.Fitness < a.group[0].Fitness)
		{
			bestOne = a.group[0];
		}
		cout << endl << endl;
	}
	cout << endl << "���ŷ���Ϊ��";
	/*for (unsigned int i = 0; i < bestOne.Chromosome1.size(); i++)
	{
		cout << bestOne.Chromosome1[i] << " " ;
	}
	cout << endl << "�������䷽��Ϊ��";
	for (unsigned int i = 0; i < bestOne.Chromosome2.size(); i++)
	{
		cout << bestOne.Chromosome2[i] << " ";
	}
	cout << endl;
	cout << "������Ӧ�ȣ�" << bestOne.Fitness <<endl;

	vector<vector<int>> each_road = bestOne.get_each_road();
	unsigned int roadnum = bestOne.Chromosome2.size() - 1;
	for (unsigned i = 0; i < each_road.size(); i++) {
		for (unsigned j = 0; j < each_road[i].size(); j++) {
			each_road[i][j] = indexmap[(each_road[i][j])];
		}
	}
	vector<vector<string>> bestpath = find_path(each_road, roadnum);
	vector<double> bestcar = bestOne.Chromosome2;
	cout << "���շ��䷽��Ϊ: " << endl;
	for (unsigned int i = 0; i < bestpath.size(); i++) {
		cout << "��" << i + 1 << "�ַ��䷽��Ϊ: " << endl;
		for (unsigned int j = 0; j < bestpath[i].size(); j++) {
			cout << bestpath[i][j];
			if (j != bestpath[i].size() - 1) {
				cout << "->";
			}
		}
		cout << "�������䷽��Ϊ" << bestOne.Chromosome2[i] << endl;
	}

	cout << "byebye" << endl;
	system("pause");
	return 0;

}*/
