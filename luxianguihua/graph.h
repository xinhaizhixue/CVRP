#ifndef      _GRAPH_H
#define      _GRAPH_H
#include<ctime>
#include<iostream>
#include<string>
#include<vector>
#include<deque>
#include <stdlib.h>
#include<map>
#include<algorithm>
#include<stdio.h>
#include<cmath>
const int INF = 100000000;
const int lend = 0;
using namespace std;
typedef struct _EdgeNode {
	int adjvex; //����������±�
	double distance;//�ߵľ���
	struct _EdgeNode *next; //ָ����һ������
}EdgeNode;

typedef struct _VertexNode {
	string name;//�ͻ��������
	double goods_num;//�ͻ�������Ҫ�Ļ���
	EdgeNode *firstedge;//ָ���һ������
	int flag;//�ڵ�����
}VertexNode;
extern vector<vector<double> > dis;//�������
extern vector<vector<int> > path;//·������
extern map<int, int>indexmap;//�������ӳ��
extern deque<VertexNode> CopyvertexArray; //�ڵ�����ĸ���

class graph {
public:
	deque<VertexNode> vertexArray; //�ڵ�����
	map<string, int> name_indexMap; //������������±�ӳ��
public:
	void init_graph();
	void Init(string _str_path);
	void node_init();
	void node_init1(string s, int kind, double weight, int i);
	void edge_init();
	void edge_init1(string startNode, string endNode, double value);
	void print_graph();
	void upgrade_edge(int start_index, int end_index, int value);
	void upgrade_map();
	void upgrade_adjvex(deque<VertexNode> temp);
	void upgrade(deque<VertexNode> temp);
	void insert_edge();
	void insert_one_edge(int start_index, int end_index, int value);
	void delete_edge();
	void delete_one_edge(int start_index, int end_index);
	void delete_edge_(int start_index, int end_index);
	void insert_node();
	void delete_node();
	EdgeNode* xun_wei(int start_index);
	EdgeNode* xun_bian(int start_index, int end_index);
	bool node_chachong(string name);
	bool edge_chachong(int start_index, int end_index);
};
void floyd(graph p);
void getCopyvertexArray(graph p);



#endif