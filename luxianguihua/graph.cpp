#include "graph.h"
vector<vector<double>> dis;//�������
vector<vector<int> > path;//·������
map<int, int>indexmap;//�������ӳ��
deque<VertexNode> CopyvertexArray; //�ڵ�����ĸ���

void graph::init_graph()
{
	node_init();
	edge_init();
}

void graph::node_init() {
	int vertexNum; //�ڵ���Ŀ
	cout << "�������ʼ�ڵ�����";
	cin >> vertexNum;
	for (int i = 0; i < vertexNum; i++)
	{
		VertexNode newNode;
		cout << "������ڵ����ƣ�ȷ�����ظ�����";
		cin >> newNode.name; //�ڵ�����
		name_indexMap.insert(pair<string, int>(newNode.name, i)); //�ڵ������������ŵ�ӳ��
		cout << "������ڵ��ǩ����������Ϊ0����ͨ�ڵ�Ϊ1��·�ڽڵ�Ϊ2����";
		cin >> newNode.flag;
		cout << "������ýڵ�����������";
		cin >> newNode.goods_num; //����������
		newNode.firstedge = NULL;
		vertexArray.push_back(newNode); //���浽������
	}
	cout << endl;
}

void graph::node_init1(string s, int kind, double weight, int i)
{
	VertexNode newNode;
	newNode.name = s;
	newNode.flag = kind;
	newNode.goods_num = weight;
	newNode.firstedge = NULL;
	name_indexMap.insert(pair<string, int>(newNode.name, i)); //�ڵ������������ŵ�ӳ��
	vertexArray.push_back(newNode); //���浽������
}

void graph::edge_init() {
	int edgeNum;//����Ŀ
	cout << "�������ʼ������";
	cin >> edgeNum;
	for (int i = 0; i < edgeNum; i++)
	{
		string startNode; //�ߵĵ�һ���ڵ�����
		string endNode; //�ߵĵڶ����ڵ�����
		double value; //�ߵ�·��ֵ
		int startIndex = 0; //��һ���ڵ�������±�
		int endIndex = 0; //�ڶ����ڵ�������±�
		EdgeNode* newNode1 = new(EdgeNode); //����ռ�
		EdgeNode* newNode2 = new(EdgeNode);
		map<string, int>::iterator it;
		cout << "������ߵ�һ�˽ڵ����ƣ�";
		cin >> startNode;
		cout << "������ߵ���һ�˽ڵ����ƣ�";
		cin >> endNode;
		cout << "������ñߵ�·��ֵ��";
		cin >> value;
		it = name_indexMap.find(startNode); //���ҵ�һ���ڵ����ƶ�Ӧ�±꣬���ص�ַ��ָ��it
		if (it == name_indexMap.end())
		{
			cout << "δ���ҵ���" << endl;
		}
		else
		{
			startIndex = it->second;
		}
		it = name_indexMap.find(endNode); //���ҵڶ����ڵ����ƶ�Ӧ�±꣬���ص�ַ��ָ��it
		if (it == name_indexMap.end())
		{
			cout << "δ���ҵ���" << endl;
		}
		else
		{
			endIndex = it->second;
		}
		newNode1->adjvex = endIndex; //��ʼ���߽ڵ�ṹ���е������±�ֵ��Ա
		newNode1->distance = value; //��ʼ��·��ֵ
		newNode1->next = NULL;
		if (vertexArray[startIndex].firstedge == NULL)
		{
			vertexArray[startIndex].firstedge = newNode1;
		}
		else
		{
			EdgeNode* p;
			for (p = vertexArray[startIndex].firstedge; p->next != NULL; p = p->next);
			p->next = newNode1;
		}
		newNode2->adjvex = startIndex;
		newNode2->distance = value;
		newNode2->next = NULL;
		if (vertexArray[endIndex].firstedge == NULL)
		{
			vertexArray[endIndex].firstedge = newNode2;
		}
		else
		{
			EdgeNode* p;
			for (p = vertexArray[endIndex].firstedge; p->next != NULL; p = p->next);
			p->next = newNode2;
		}
	}
}

void graph::edge_init1(string startNode, string endNode, double value)
{
	int startIndex = 0; //��һ���ڵ�������±�
	int endIndex = 0; //�ڶ����ڵ�������±�
	EdgeNode* newNode1 = new(EdgeNode); //����ռ�
	EdgeNode* newNode2 = new(EdgeNode);
	map<string, int>::iterator it;
	it = name_indexMap.find(startNode); //���ҵ�һ���ڵ����ƶ�Ӧ�±꣬���ص�ַ��ָ��it
	if (it == name_indexMap.end())
	{
		cout << "δ���ҵ���" << endl;
	}
	else
	{
		startIndex = it->second;
	}
	it = name_indexMap.find(endNode); //���ҵڶ����ڵ����ƶ�Ӧ�±꣬���ص�ַ��ָ��it
	if (it == name_indexMap.end())
	{
		cout << "δ���ҵ���" << endl;
	}
	else
	{
		endIndex = it->second;
	}
	newNode1->adjvex = endIndex; //��ʼ���߽ڵ�ṹ���е������±�ֵ��Ա
	newNode1->distance = value; //��ʼ��·��ֵ
	newNode1->next = NULL;
	if (vertexArray[startIndex].firstedge == NULL)
	{
		vertexArray[startIndex].firstedge = newNode1;
	}
	else
	{
		EdgeNode* p1;
		for (p1 = vertexArray[startIndex].firstedge; p1->next != NULL; p1 = p1->next);
		p1->next = newNode1;
	}
	newNode2->adjvex = startIndex;
	newNode2->distance = value;
	newNode2->next = NULL;
	if (vertexArray[endIndex].firstedge == NULL)
	{
		vertexArray[endIndex].firstedge = newNode2;
	}
	else
	{
		EdgeNode* p1;
		for (p1 = vertexArray[endIndex].firstedge; p1->next != NULL; p1 = p1->next);
		p1->next = newNode2;
	}
}

//��֪ͷ�ڵ㣬�ҳ��ڵ�ĩ��
EdgeNode* graph::xun_wei(int start_index) {
	EdgeNode *p = vertexArray[start_index].firstedge;
	//ѭ���ҳ�β�ڵ�
	for (; p->next != NULL; p = p->next);
	return p;
}

//��֪�����˽ڵ㣬�ҳ��ñ�
EdgeNode* graph::xun_bian(int start_index, int end_index) {
	EdgeNode *p = vertexArray[start_index].firstedge;
	if (p == NULL)
		return NULL;
	//ѭ���ҳ�Ŀ��ڵ�
	for (; p->adjvex != end_index && p->next != NULL; p = p->next);
	if (p->next == NULL && p->adjvex != end_index) {
		return NULL;
	}
	else
		return p;
}

//�ڵ����
bool graph::node_chachong(string name) {
	map<string, int>::iterator it;
	it = name_indexMap.find(name);
	if (it != name_indexMap.end())
		return true;
	else
		return false;
}

//�߲���
bool graph::edge_chachong(int start_index, int end_index) {
	EdgeNode *p;
	p = xun_bian(start_index, end_index);
	if (p != NULL)
		return true;
	else
		return false;
}

//����ڵ�
void graph::insert_node() {
	int vertex_num;
	VertexNode *node = new(VertexNode);
	cout << "���������ڵ�����";
	cin >> vertex_num;
	for (int i = 0; i < vertex_num; i++) {
		//�ڵ���Ϣ����
		cout << "������ڵ����ƣ�";
		cin >> node->name;
		if (node_chachong(node->name)) {
			cout << "�õ��Ѵ��ڣ�" << endl;
			i--;
			continue;
		}
		cout << "������ڵ��ǩ��";
		cin >> node->flag;
		cout << "������ڵ���������";
		cin >> node->goods_num;

		node->firstedge = NULL;
		vertexArray.push_back(*node);
		name_indexMap.insert(pair<string, int>(node->name, vertexArray.size() - 1));

		//�ڵ�ߵĲ���
		int edge_num;
		int start_index;
		int end_index;
		int value;
		string end_node;
		cout << "������õ���صı�����";
		cin >> edge_num;
		for (int i = 0; i < edge_num; i++) {
			start_index = name_indexMap.find(node->name)->second;
			cout << "���������������Ľڵ����ƣ�";
			cin >> end_node;
			end_index = name_indexMap.find(end_node)->second;
			cout << "������߳����룺";
			cin >> value;
			insert_one_edge(start_index, end_index, value);
		}
	}
}

//ɾ���ڵ�
void graph::delete_node() {
	int num;
	string start_node;
	int start_index;
	EdgeNode *p;
	deque<VertexNode>::iterator it = vertexArray.begin();
	deque<VertexNode> temp(vertexArray);//����ԭ���Ķ���
	cout << "������ɾ���ڵ�����";
	cin >> num;
	for (int i = 0; i < num; i++) {
		cout << "������ɾ���Ľڵ����ƣ�";
		cin >> start_node;
		start_index = name_indexMap.find(start_node)->second;
		for (unsigned int i = 0; i < vertexArray.size(); i++) {
			p = xun_bian(i, start_index);
			if (p == NULL)
				continue;
			else {
				delete_one_edge(i, start_index);
			}
		}
		vertexArray.erase(it + start_index);
		upgrade(temp);
		//�ͷŶ���
		temp.clear();
	}
}



//����ӳ��
void graph::upgrade_map() {
	name_indexMap.clear();
	for (unsigned int i = 0; i < vertexArray.size(); i++) {
		name_indexMap.insert(pair<string, int>(vertexArray[i].name, i));
	}
}

//�����������
void graph::upgrade_adjvex(deque<VertexNode> temp) {
	EdgeNode *p;
	for (unsigned int i = 0; i < vertexArray.size(); i++) {
		p = vertexArray[i].firstedge;
		if (p == NULL)
			continue;
		else {
			for (; p != NULL; p = p->next) {
				p->adjvex = name_indexMap.find(temp[p->adjvex].name)->second;
			}
		}
	}
}

//�ܸ���
void graph::upgrade(deque<VertexNode> temp) {
	upgrade_map();
	upgrade_adjvex(temp);
}

//���������
void graph::insert_edge() {
	int edge_num;
	cout << "����������Ҫ���ӵı�����";
	cin >> edge_num;
	for (int i = 0; i < edge_num; i++) {
		string start_node;
		string end_node;
		int value;
		int start_index;
		int end_index;
		map<string, int>::iterator it;
		cout << "������ߵ�һ�˽ڵ����ƣ�";
		cin >> start_node;
		cout << "������ߵ���һ�˽ڵ����ƣ�";
		cin >> end_node;
		cout << "������ñߵ�·��ֵ��";
		cin >> value;
		it = name_indexMap.find(start_node);
		if (it == name_indexMap.end()) {
			cout << "δ���ҵ���" << endl;
			i--;
		}
		else {
			start_index = it->second;
			it = name_indexMap.find(end_node);
			end_index = it->second;
			insert_one_edge(start_index, end_index, value);
		}
	}
}

//����һ���ߣ��ּ������
void graph::insert_one_edge(int start_index, int end_index, int value) {
	VertexNode *p = &vertexArray[start_index];
	EdgeNode *q = p->firstedge;
	EdgeNode *q0 = new(EdgeNode);
	EdgeNode *q1 = new(EdgeNode);
	//�����ڵ����
	if (q == NULL) {
		q0->adjvex = end_index;
		q0->distance = value;
		q0->next = NULL;
		p->firstedge = q0;
		q = xun_wei(end_index);
		q1->adjvex = start_index;
		q1->distance = value;
		q1->next = NULL;
		q->next = q1;
	}
	//�ظ��ߣ���ѡ���Ƿ��޸�
	else if (edge_chachong(start_index, end_index)) {
		upgrade_edge(start_index, end_index, value);
	}
	//һ���������ĩβ����
	else {
		q = xun_wei(start_index);
		q0->adjvex = end_index;
		q0->distance = value;
		q0->next = NULL;
		q->next = q0;
		q = xun_wei(end_index);
		q1->adjvex = start_index;
		q1->distance = value;
		q1->next = NULL;
		q->next = q1;
	}
}

//ɾ����
void graph::delete_edge() {
	int num;
	string start_node;
	string end_node;
	int start_index;
	int end_index;
	map<string, int>::iterator it0, it1;
	cout << "������ɾ��������";
	cin >> num;
	for (int i = 0; i < num; i++) {
		cout << "������ߵ�һ�˽ڵ����ƣ�";
		cin >> start_node;
		cout << "������ߵ���һ�˽ڵ����ƣ�";
		cin >> end_node;
		it0 = name_indexMap.find(start_node);
		it1 = name_indexMap.find(end_node);
		if (it0 == name_indexMap.end() || it1 == name_indexMap.end()) {
			cout << "δ���ҵ���" << endl;
		}
		else {
			start_index = it0->second;
			end_index = it1->second;
			delete_one_edge(start_index, end_index);
		}
	}
}

//ɾ��һ����
void graph::delete_one_edge(int start_index, int end_index) {
	delete_edge_(start_index, end_index);
	delete_edge_(end_index, start_index);
}

//ɾ���ߵľ����������Ϊ���˫��������ҽ��䵥�����˳���
void graph::delete_edge_(int start_index, int end_index) {
	EdgeNode *p0, *p, *p1;
	if (vertexArray[start_index].firstedge->adjvex == end_index) {
		p = vertexArray[start_index].firstedge;
		p1 = p->next;
		vertexArray[start_index].firstedge = p1;
		delete p;
		p = NULL;
	}
	else {
		for (p0 = vertexArray[start_index].firstedge; p0->next != NULL; p0 = p0->next) {
			p = p0->next;
			p1 = p0->next->next;
			if (p1 == NULL) {
				delete p;
				p = NULL;
				p0->next = NULL;
				break;
			}
			if (p->adjvex == end_index) {
				p0->next = p1;
				delete p;
				p = NULL;
			}
		}
	}
}

//���±ߣ����ظ��ˣ�����иò���
void graph::upgrade_edge(int start_index, int end_index, int value) {
	int flag;
	EdgeNode *p;
	p = xun_bian(start_index, end_index);
	cout << "���иñߣ�����Ϊ" << p->distance << "���Ƿ��޸ĸñ�?" << endl;
	cout << "1:�޸�" << endl << "0:���޸�" << endl;
	cin >> flag;
	switch (flag) {
	case 0:
		break;
	case 1:
		EdgeNode *p;
		p = xun_bian(start_index, end_index);
		p->distance = value;
		p = xun_bian(end_index, start_index);
		p->distance = value;
	}
}

//��ӡ�ڽӱ�
void graph::print_graph()
{
	for (unsigned int i = 0; i < vertexArray.size(); i++)
	{
		cout << vertexArray[i].name;
		if (vertexArray[i].firstedge == NULL)
		{
			cout << endl;
		}
		else
		{
			EdgeNode* p;
			for (p = vertexArray[i].firstedge; p != NULL; p = p->next)
			{
				cout << " " << p->adjvex;
			}
			cout << endl;
		}
	}
}
void init(int vertexnum) {
	dis.resize(vertexnum);
	path.resize(vertexnum);
	for (int i = 0; i < vertexnum; i++) {
		dis[i].resize(vertexnum);
		path[i].resize(vertexnum);
	}
	for (int i = 0; i < vertexnum; i++) {
		for (int j = 0; j < vertexnum; j++) {
			if (j == i) {
				dis[i][j] = 0;
			}
			else
			{
				dis[i][j] = INF;
			}
			path[i][j] = j;
		}

	}

}
void floyd(graph p) {
	unsigned int i, j, k;
	deque<VertexNode>::iterator it = p.vertexArray.begin();
	unsigned int vertexnum = p.vertexArray.size();
	init(vertexnum);
	for (i = 0; i < p.vertexArray.size(); i++) {
		_EdgeNode* s = (it + i)->firstedge;
		while (s)
		{
			dis[i][s->adjvex] = s->distance;
			s = s->next;
		}
	}
	for (k = 0; k < p.vertexArray.size(); k++)//�ѵ�K������ӵ��м�㼯����
	{
		for (i = 0; i < p.vertexArray.size(); i++)//����
		{
			for (j = 0; j < p.vertexArray.size(); j++)//����
			{
				if (dis[i][k] + dis[k][j] < dis[i][j])//i-->k-->j
				{
					dis[i][j] = dis[i][k] + dis[k][j];
					path[i][j] = path[i][k];
				}
			}
		}
	}
	for (i = 0, k = 0; i < p.vertexArray.size(); i++) {//���½����µ�ӳ��
		if ((it + i)->flag == 2) {
			continue;
		}
		indexmap.insert(pair<int, int>(k, i));
		k++;
	}


	//��ӡ����
	/*for (i = 0; i < p.vertexArray.size(); i++)
	{
		for (j = 0; j < p.vertexArray.size(); j++)
		{
			printf("%d\t", path[i][j]);
		}
		printf("\n");
	}*/
}

void getCopyvertexArray(graph p) {
	CopyvertexArray = p.vertexArray;
}
