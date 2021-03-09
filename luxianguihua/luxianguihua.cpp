#include "luxianguihua.h"
#include <qpushbutton.h>
#include <qfiledialog.h>
#include <qcheckbox.h>
#include <qtextedit.h>
#include <qprogressdialog.h>
#include <qtimer.h>
#include <QTime>

int GAtime = 1; //遗传算法次数
extern double W[4];
extern int car_kind;//车辆种类
extern double car_time;
extern double unload_time;
extern double car_speed;
extern double car_distance;
extern double P;//惩罚系数
extern vector<double> car_weight;//记录每种车的载重量
extern vector<int> car_num;//记录每种车的数量
extern vector<double>car_oil;//记录每种车的油费
extern graph *gp;

luxianguihua::luxianguihua(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	setWindowTitle(QString::fromLocal8Bit("路线规划"));
	flag = 0;
	weight = 0;
	connect(ui.pushButton, &QPushButton::clicked, this, &luxianguihua::loading);
	connect(ui.checkBox, &QCheckBox::clicked, this, &luxianguihua::weight_c);
	connect(ui.checkBox_2, &QCheckBox::clicked, this, &luxianguihua::weight_d);
	connect(ui.checkBox_3, &QCheckBox::clicked, this, &luxianguihua::weight_n);
	connect(ui.checkBox_4, &QCheckBox::clicked, this, &luxianguihua::weight_l);
	connect(ui.pushButton_2, &QPushButton::clicked, this, &luxianguihua::_show);
	connect(ui.pushButton_3, &QPushButton::clicked, this, &luxianguihua::imformation);
	connect(ui.pushButton_4, &QPushButton::clicked, this, &luxianguihua::send_signal);
	connect(ui.pushButton_5, &QPushButton::clicked, this, &luxianguihua::send_upgrade_signal);
}

void luxianguihua::loading() {
	srand((int)(time(0)));
	p = new(graph);
	QString str_path = "";
	for (;;) {
		QStringList str_path_list = QFileDialog::getOpenFileNames(this, QString::fromLocal8Bit("选择文本文件"), 
			QString::fromLocal8Bit("测试用例1.txt"), QString::fromLocal8Bit("文本文件(*.txt);;"));
		if (str_path_list.size() == 0)
			continue;
		for (int i = 0; i < str_path_list.size(); i++) {
			str_path = str_path_list[i];
		}	
		break;
	}
	filename = str_path;
	std::string _str_path = (string)str_path.toLocal8Bit();

	p->Init(_str_path);
	floyd(*p);
	getCopyvertexArray(*p);

	ui.textBrowser->append(QString::fromLocal8Bit("加载成功"));
	ui.textBrowser->append(QString::fromLocal8Bit(""));
	gp = p;
}

void luxianguihua::imformation() {
	QString str;
	QStringList str_list;
	car_kind = ui.lineEdit->text().toInt();
	car_weight.resize(car_kind);
	car_num.resize(car_kind);
	car_oil.resize(car_kind);

	str = ui.lineEdit_2->text();
	str_list = str.split(" ");
	for (int i = 0; i < car_kind; i++) {
		car_oil[i] = str_list[i].toDouble();
	}
	str.clear();
	str_list.clear();

	str = ui.lineEdit_3->text();
	str_list = str.split(" ");
	for (int i = 0; i < car_kind; i++) {
		car_num[i] = str_list[i].toInt();
	}
	str.clear();
	str_list.clear();

	str = ui.lineEdit_4->text();
	str_list = str.split(" ");
	for (int i = 0; i < car_kind; i++) {
		car_weight[i] = str_list[i].toDouble();
	}
	str.clear();
	str_list.clear();

	car_distance = ui.lineEdit_5->text().toDouble();
	car_speed = ui.lineEdit_6->text().toDouble();
	car_time = ui.lineEdit_7->text().toDouble();
	unload_time = ui.lineEdit_8->text().toDouble();
	/*只取50的原因：
	 *因为我仔细看了咱们的目标函数，因为取了对数，导致我们的规模即便是10^10的量级变动也会是在1到30之间的某个数字
	 *所以在惩罚系数根据目标函数值来判断取一个远大于目标函数的值的规则来看，只取一个50似乎就可以了
	 */
	P = 100;
}

void luxianguihua::weight_c() {
	if (ui.checkBox->isChecked()) {
		weight += 8;
	}
	else
		weight -= 8;
}

void luxianguihua::weight_d() {
	if (ui.checkBox_2->isChecked()) {
		weight += 4;
	}
	else
		weight -= 4;
}

void luxianguihua::weight_n() {
	if (ui.checkBox_3->isChecked()) {
		weight += 2;
	}
	else
		weight -= 2;
}

void luxianguihua::weight_l() {
	if (ui.checkBox_4->isChecked()) {
		weight += 1;
	}
	else
		weight -= 1;
}

void luxianguihua::_show() {
	switch (weight) {
	case 0:
		W[0] = 1;
		W[1] = 1;
		W[2] = 1;
		W[3] = 1;
		break;
	case 1:
		W[0] = 1;
		W[1] = 1;
		W[2] = 1;
		W[3] = 3;
		break;
	case 2:
		W[0] = 1;
		W[1] = 1;
		W[2] = 3;
		W[3] = 1;
		break;
	case 4:
		W[0] = 1;
		W[1] = 3;
		W[2] = 1;
		W[3] = 1;
		break;
	case 8:
		W[0] = 3;
		W[1] = 1;
		W[2] = 1;
		W[3] = 1;
		break;
	default:
		W[0] = 0;
		W[1] = 1;
		W[2] = 0;
		W[3] = 0;
		break;
	}
	
	bestone = new(individuality);
	for (int i = 0; i < GAtime; i++)
	{
		population a;
		a.GA(a);
		if (bestone->Fitness < a.group[0].Fitness)
		{
			*bestone = a.group[0];
		}
	}

	QString str;
	vector<vector<int>> each_road = bestone->get_each_road();
	unsigned int roadnum = bestone->Chromosome2.size() - 1;
	for (unsigned i = 0; i < each_road.size(); i++) {
		for (unsigned j = 0; j < each_road[i].size(); j++) {
			each_road[i][j] = indexmap[(each_road[i][j])];
		}
	}
	//原始方案序列字符串
	vector<vector<string>> oPath(roadnum);
	for (unsigned int i = 0; i < roadnum; i++) {
		for (unsigned int j = 0; j < each_road[i].size(); j++) {
			oPath[i].push_back(CopyvertexArray[each_road[i][j]].name);
		}
	}
	vector<vector<string>> bestpath = find_path(each_road, roadnum);

	vector<double> bestcar = bestone->Chromosome2;
	ui.textBrowser->append(QString::fromLocal8Bit("最终分配方案为: "));
	for (unsigned int i = 0; i < bestpath.size(); i++) {
		str.sprintf("%d", i + 1);
		ui.textBrowser->append(QString::fromLocal8Bit("第") + str +QString::fromLocal8Bit("种分配方案为:"));
		unsigned int k = 0;
		for (unsigned int j = 0; j < bestpath[i].size(); j++) {
			str.clear();
			if (bestpath[i][j] != oPath[i][k])
			{
				str =  QString::fromStdString("("+bestpath[i][j]+")");
			}
			else
			{
				str = QString::fromStdString(bestpath[i][j]);
				k++;
			}
			ui.textBrowser->insertPlainText(str);
			if (j != bestpath[i].size() - 1) {
				ui.textBrowser->insertPlainText("->");
			}
		}
		str.clear();
		str.sprintf("%d", (int)bestone->Chromosome2[i]);
		ui.textBrowser->append(QString::fromLocal8Bit("车辆分配方案为载重量为 ") + str +
			QString::fromLocal8Bit(" 的车辆"));
	}

	str.clear();
	str.sprintf("%.2lf", bestone->C);
	ui.textBrowser->append(QString::fromLocal8Bit("该方案车辆成本为：") + str);
	str.sprintf("%.2lf", bestone->D);
	ui.textBrowser->append(QString::fromLocal8Bit("该方案车辆里程为：") + str);
	str.sprintf("%d", bestone->N);
	ui.textBrowser->append(QString::fromLocal8Bit("该方案车辆数量为：") + str);
	str.sprintf("%.2lf", bestone->L * 100);
	ui.textBrowser->append(QString::fromLocal8Bit("该方案车载重率为：") + str + "%");
	str.sprintf("%.2lf", bestone->value);
	ui.textBrowser->append(QString::fromLocal8Bit("该方案综合目标函数（以上四个的综合）为：") + str);
	str.sprintf("%lf", bestone->Fitness);
	ui.textBrowser->append(QString::fromLocal8Bit("该方案适应度为：") + str);

	QTime current_time = QTime::currentTime();
	int h = current_time.hour();
	int m = current_time.minute();
	int s= current_time.second();
	str.sprintf("%d:%d:%d", h, m, s);
	ui.textBrowser->append(QString::fromLocal8Bit("结束时间为：") + str);

	ui.textBrowser->append(QString::fromLocal8Bit(""));
}

void luxianguihua::send_signal() {
	flag = 0;
	emit sub();
}

void luxianguihua::send_upgrade_signal() {
	flag = 1;
	emit upgrade_signal();
}