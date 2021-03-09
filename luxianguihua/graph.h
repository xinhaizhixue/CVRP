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
	int adjvex; //顶点的索引下标
	double distance;//边的距离
	struct _EdgeNode *next; //指向下一条出边
}EdgeNode;

typedef struct _VertexNode {
	string name;//送货点的名称
	double goods_num;//送货点所需要的货物
	EdgeNode *firstedge;//指向第一条出边
	int flag;//节点类型
}VertexNode;
extern vector<vector<double> > dis;//距离矩阵
extern vector<vector<int> > path;//路径矩阵
extern map<int, int>indexmap;//基因查找映射
extern deque<VertexNode> CopyvertexArray; //节点数组的副本

class graph {
public:
	deque<VertexNode> vertexArray; //节点数组
	map<string, int> name_indexMap; //结点名称数组下标映射
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