#include "luxianguihua.h"
#include <qpushbutton.h>
#include <qfiledialog.h>
#include <qcheckbox.h>
#include <qtextedit.h>
#include <qprogressdialog.h>
#include <qtimer.h>
#include <QTime>

int GAtime = 1; //�Ŵ��㷨����
extern double W[4];
extern int car_kind;//��������
extern double car_time;
extern double unload_time;
extern double car_speed;
extern double car_distance;
extern double P;//�ͷ�ϵ��
extern vector<double> car_weight;//��¼ÿ�ֳ���������
extern vector<int> car_num;//��¼ÿ�ֳ�������
extern vector<double>car_oil;//��¼ÿ�ֳ����ͷ�
extern graph *gp;

luxianguihua::luxianguihua(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	setWindowTitle(QString::fromLocal8Bit("·�߹滮"));
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
		QStringList str_path_list = QFileDialog::getOpenFileNames(this, QString::fromLocal8Bit("ѡ���ı��ļ�"), 
			QString::fromLocal8Bit("��������1.txt"), QString::fromLocal8Bit("�ı��ļ�(*.txt);;"));
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

	ui.textBrowser->append(QString::fromLocal8Bit("���سɹ�"));
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
	/*ֻȡ50��ԭ��
	 *��Ϊ����ϸ�������ǵ�Ŀ�꺯������Ϊȡ�˶������������ǵĹ�ģ������10^10�������䶯Ҳ������1��30֮���ĳ������
	 *�����ڳͷ�ϵ������Ŀ�꺯��ֵ���ж�ȡһ��Զ����Ŀ�꺯����ֵ�Ĺ���������ֻȡһ��50�ƺ��Ϳ�����
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
	//ԭʼ���������ַ���
	vector<vector<string>> oPath(roadnum);
	for (unsigned int i = 0; i < roadnum; i++) {
		for (unsigned int j = 0; j < each_road[i].size(); j++) {
			oPath[i].push_back(CopyvertexArray[each_road[i][j]].name);
		}
	}
	vector<vector<string>> bestpath = find_path(each_road, roadnum);

	vector<double> bestcar = bestone->Chromosome2;
	ui.textBrowser->append(QString::fromLocal8Bit("���շ��䷽��Ϊ: "));
	for (unsigned int i = 0; i < bestpath.size(); i++) {
		str.sprintf("%d", i + 1);
		ui.textBrowser->append(QString::fromLocal8Bit("��") + str +QString::fromLocal8Bit("�ַ��䷽��Ϊ:"));
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
		ui.textBrowser->append(QString::fromLocal8Bit("�������䷽��Ϊ������Ϊ ") + str +
			QString::fromLocal8Bit(" �ĳ���"));
	}

	str.clear();
	str.sprintf("%.2lf", bestone->C);
	ui.textBrowser->append(QString::fromLocal8Bit("�÷��������ɱ�Ϊ��") + str);
	str.sprintf("%.2lf", bestone->D);
	ui.textBrowser->append(QString::fromLocal8Bit("�÷����������Ϊ��") + str);
	str.sprintf("%d", bestone->N);
	ui.textBrowser->append(QString::fromLocal8Bit("�÷�����������Ϊ��") + str);
	str.sprintf("%.2lf", bestone->L * 100);
	ui.textBrowser->append(QString::fromLocal8Bit("�÷�����������Ϊ��") + str + "%");
	str.sprintf("%.2lf", bestone->value);
	ui.textBrowser->append(QString::fromLocal8Bit("�÷����ۺ�Ŀ�꺯���������ĸ����ۺϣ�Ϊ��") + str);
	str.sprintf("%lf", bestone->Fitness);
	ui.textBrowser->append(QString::fromLocal8Bit("�÷�����Ӧ��Ϊ��") + str);

	QTime current_time = QTime::currentTime();
	int h = current_time.hour();
	int m = current_time.minute();
	int s= current_time.second();
	str.sprintf("%d:%d:%d", h, m, s);
	ui.textBrowser->append(QString::fromLocal8Bit("����ʱ��Ϊ��") + str);

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