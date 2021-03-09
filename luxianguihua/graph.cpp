#include "graph.h"
vector<vector<double>> dis;//距离矩阵
vector<vector<int> > path;//路径矩阵
map<int, int>indexmap;//基因查找映射
deque<VertexNode> CopyvertexArray; //节点数组的副本

void graph::init_graph()
{
	node_init();
	edge_init();
}

void graph::node_init() {
	int vertexNum; //节点数目
	cout << "请输入初始节点数：";
	cin >> vertexNum;
	for (int i = 0; i < vertexNum; i++)
	{
		VertexNode newNode;
		cout << "请输入节点名称（确保无重复）：";
		cin >> newNode.name; //节点名称
		name_indexMap.insert(pair<string, int>(newNode.name, i)); //节点名称与索引号的映射
		cout << "请输入节点标签（配送中心为0，普通节点为1，路口节点为2）：";
		cin >> newNode.flag;
		cout << "请输入该节点的需求货量：";
		cin >> newNode.goods_num; //货物需求量
		newNode.firstedge = NULL;
		vertexArray.push_back(newNode); //保存到数组中
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
	name_indexMap.insert(pair<string, int>(newNode.name, i)); //节点名称与索引号的映射
	vertexArray.push_back(newNode); //保存到数组中
}

void graph::edge_init() {
	int edgeNum;//边数目
	cout << "请输入初始边数：";
	cin >> edgeNum;
	for (int i = 0; i < edgeNum; i++)
	{
		string startNode; //边的第一个节点名称
		string endNode; //边的第二个节点名称
		double value; //边的路程值
		int startIndex = 0; //第一个节点的数组下标
		int endIndex = 0; //第二个节点的数组下标
		EdgeNode* newNode1 = new(EdgeNode); //申请空间
		EdgeNode* newNode2 = new(EdgeNode);
		map<string, int>::iterator it;
		cout << "请输入边的一端节点名称：";
		cin >> startNode;
		cout << "请输入边的另一端节点名称：";
		cin >> endNode;
		cout << "请输入该边的路程值：";
		cin >> value;
		it = name_indexMap.find(startNode); //查找第一个节点名称对应下标，返回地址给指针it
		if (it == name_indexMap.end())
		{
			cout << "未查找到！" << endl;
		}
		else
		{
			startIndex = it->second;
		}
		it = name_indexMap.find(endNode); //查找第二个节点名称对应下标，返回地址给指针it
		if (it == name_indexMap.end())
		{
			cout << "未查找到！" << endl;
		}
		else
		{
			endIndex = it->second;
		}
		newNode1->adjvex = endIndex; //初始化边节点结构体中的数组下标值成员
		newNode1->distance = value; //初始化路程值
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
	int startIndex = 0; //第一个节点的数组下标
	int endIndex = 0; //第二个节点的数组下标
	EdgeNode* newNode1 = new(EdgeNode); //申请空间
	EdgeNode* newNode2 = new(EdgeNode);
	map<string, int>::iterator it;
	it = name_indexMap.find(startNode); //查找第一个节点名称对应下标，返回地址给指针it
	if (it == name_indexMap.end())
	{
		cout << "未查找到！" << endl;
	}
	else
	{
		startIndex = it->second;
	}
	it = name_indexMap.find(endNode); //查找第二个节点名称对应下标，返回地址给指针it
	if (it == name_indexMap.end())
	{
		cout << "未查找到！" << endl;
	}
	else
	{
		endIndex = it->second;
	}
	newNode1->adjvex = endIndex; //初始化边节点结构体中的数组下标值成员
	newNode1->distance = value; //初始化路程值
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

//已知头节点，找出节点末端
EdgeNode* graph::xun_wei(int start_index) {
	EdgeNode *p = vertexArray[start_index].firstedge;
	//循环找出尾节点
	for (; p->next != NULL; p = p->next);
	return p;
}

//已知边两端节点，找出该边
EdgeNode* graph::xun_bian(int start_index, int end_index) {
	EdgeNode *p = vertexArray[start_index].firstedge;
	if (p == NULL)
		return NULL;
	//循环找出目标节点
	for (; p->adjvex != end_index && p->next != NULL; p = p->next);
	if (p->next == NULL && p->adjvex != end_index) {
		return NULL;
	}
	else
		return p;
}

//节点查重
bool graph::node_chachong(string name) {
	map<string, int>::iterator it;
	it = name_indexMap.find(name);
	if (it != name_indexMap.end())
		return true;
	else
		return false;
}

//边查重
bool graph::edge_chachong(int start_index, int end_index) {
	EdgeNode *p;
	p = xun_bian(start_index, end_index);
	if (p != NULL)
		return true;
	else
		return false;
}

//插入节点
void graph::insert_node() {
	int vertex_num;
	VertexNode *node = new(VertexNode);
	cout << "请输入插入节点数：";
	cin >> vertex_num;
	for (int i = 0; i < vertex_num; i++) {
		//节点信息输入
		cout << "请输入节点名称：";
		cin >> node->name;
		if (node_chachong(node->name)) {
			cout << "该点已存在！" << endl;
			i--;
			continue;
		}
		cout << "请输入节点标签：";
		cin >> node->flag;
		cout << "请输入节点配送量：";
		cin >> node->goods_num;

		node->firstedge = NULL;
		vertexArray.push_back(*node);
		name_indexMap.insert(pair<string, int>(node->name, vertexArray.size() - 1));

		//节点边的插入
		int edge_num;
		int start_index;
		int end_index;
		int value;
		string end_node;
		cout << "请输入该点相关的边数：";
		cin >> edge_num;
		for (int i = 0; i < edge_num; i++) {
			start_index = name_indexMap.find(node->name)->second;
			cout << "请输入与其相连的节点名称：";
			cin >> end_node;
			end_index = name_indexMap.find(end_node)->second;
			cout << "请输入边长距离：";
			cin >> value;
			insert_one_edge(start_index, end_index, value);
		}
	}
}

//删除节点
void graph::delete_node() {
	int num;
	string start_node;
	int start_index;
	EdgeNode *p;
	deque<VertexNode>::iterator it = vertexArray.begin();
	deque<VertexNode> temp(vertexArray);//保存原来的队列
	cout << "请输入删除节点数：";
	cin >> num;
	for (int i = 0; i < num; i++) {
		cout << "请输入删除的节点名称：";
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
		//释放队列
		temp.clear();
	}
}



//更新映射
void graph::upgrade_map() {
	name_indexMap.clear();
	for (unsigned int i = 0; i < vertexArray.size(); i++) {
		name_indexMap.insert(pair<string, int>(vertexArray[i].name, i));
	}
}

//更新自身序号
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

//总更新
void graph::upgrade(deque<VertexNode> temp) {
	upgrade_map();
	upgrade_adjvex(temp);
}

//插入多条边
void graph::insert_edge() {
	int edge_num;
	cout << "请输入你想要增加的边数：";
	cin >> edge_num;
	for (int i = 0; i < edge_num; i++) {
		string start_node;
		string end_node;
		int value;
		int start_index;
		int end_index;
		map<string, int>::iterator it;
		cout << "请输入边的一端节点名称：";
		cin >> start_node;
		cout << "请输入边的另一端节点名称：";
		cin >> end_node;
		cout << "请输入该边的路程值：";
		cin >> value;
		it = name_indexMap.find(start_node);
		if (it == name_indexMap.end()) {
			cout << "未查找到！" << endl;
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

//插入一条边，分几种情况
void graph::insert_one_edge(int start_index, int end_index, int value) {
	VertexNode *p = &vertexArray[start_index];
	EdgeNode *q = p->firstedge;
	EdgeNode *q0 = new(EdgeNode);
	EdgeNode *q1 = new(EdgeNode);
	//孤立节点情况
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
	//重复边，则选择是否修改
	else if (edge_chachong(start_index, end_index)) {
		upgrade_edge(start_index, end_index, value);
	}
	//一般情况插入末尾即可
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

//删除边
void graph::delete_edge() {
	int num;
	string start_node;
	string end_node;
	int start_index;
	int end_index;
	map<string, int>::iterator it0, it1;
	cout << "请输入删除边数：";
	cin >> num;
	for (int i = 0; i < num; i++) {
		cout << "请输入边的一端节点名称：";
		cin >> start_node;
		cout << "请输入边的另一端节点名称：";
		cin >> end_node;
		it0 = name_indexMap.find(start_node);
		it1 = name_indexMap.find(end_node);
		if (it0 == name_indexMap.end() || it1 == name_indexMap.end()) {
			cout << "未查找到！" << endl;
		}
		else {
			start_index = it0->second;
			end_index = it1->second;
			delete_one_edge(start_index, end_index);
		}
	}
}

//删除一条边
void graph::delete_one_edge(int start_index, int end_index) {
	delete_edge_(start_index, end_index);
	delete_edge_(end_index, start_index);
}

//删除边的具体操作，因为设计双向操作，我将其单独拿了出来
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

//更新边（若重复了，则进行该步）
void graph::upgrade_edge(int start_index, int end_index, int value) {
	int flag;
	EdgeNode *p;
	p = xun_bian(start_index, end_index);
	cout << "已有该边，距离为" << p->distance << "，是否修改该边?" << endl;
	cout << "1:修改" << endl << "0:不修改" << endl;
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

//打印邻接表
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
	for (k = 0; k < p.vertexArray.size(); k++)//把第K个点添加到中间点集合中
	{
		for (i = 0; i < p.vertexArray.size(); i++)//逐行
		{
			for (j = 0; j < p.vertexArray.size(); j++)//逐列
			{
				if (dis[i][k] + dis[k][j] < dis[i][j])//i-->k-->j
				{
					dis[i][j] = dis[i][k] + dis[k][j];
					path[i][j] = path[i][k];
				}
			}
		}
	}
	for (i = 0, k = 0; i < p.vertexArray.size(); i++) {//重新建立新的映射
		if ((it + i)->flag == 2) {
			continue;
		}
		indexmap.insert(pair<int, int>(k, i));
		k++;
	}


	//打印矩阵
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
