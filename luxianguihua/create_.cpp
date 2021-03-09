#include "create_.h"
#include <qpushbutton.h>
int Flag_isOpen = 0;       //标记：判断是否打开或创建了一个文件
int Flag_IsNew = 0;        //标记：如果新建了文件就为1，初始值为0
QString Last_FileName;     //最后一次保存的文件的名字
QString Last_FileContent;  //最后一次保存文件的内容

create_::create_(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setWindowTitle(QString::fromLocal8Bit("创建"));
	connect(ui.pushButton, &QPushButton::clicked, this, &create_::create_ct);
}

create_::~create_()
{

}

void create_::create_ct() {
	emit sub_ct();
}

void create_::ct_load(luxianguihua *lxgh) {
	lxgh->p = new(graph);
	string s;
	QString str, str_;
	QStringList strl_name, strl_kind, strl_need,strl,strl_edge,strl_es;
	int vertexnum;
	int edgenum;
	vertexnum = ui.lineEdit->text().toInt();

	strl_name = ui.lineEdit_2->text().split(" ");
	strl_kind = ui.lineEdit_3->text().split(" ");
	strl_need = ui.lineEdit_4->text().split(" ");
	for (int i = 0; i < vertexnum; i++) {
		string s = (string)strl_name[i].toLocal8Bit();
		lxgh->p->node_init1(s, strl_kind[i].toInt(),
			strl_need[i].toDouble(), i);
	}

	edgenum = ui.lineEdit_5->text().toInt();
	strl_edge = ui.lineEdit_6->text().split("|");
	strl_es = ui.lineEdit_7->text().split(" ");
	for (int i = 0; i < edgenum; i++) {
		strl = strl_edge[i].split(" ");
		str = strl[0];
		str_ = strl[1];
		lxgh->p->edge_init1(str.toStdString(),
			str_.toStdString(), strl_es[i].toDouble());
	}

	lxgh->p->upgrade(lxgh->p->vertexArray);

	floyd(*(lxgh->p));
	getCopyvertexArray(*(lxgh->p));

	
	if(flag == 0){
		QFileDialog fileDialog;
		filename = fileDialog.getSaveFileName(this, tr("Open File"), tr("Text File.txt"), QString::fromLocal8Bit("文本文件(*.txt);;"));
		if (filename == "")
		{
			return;
		}
	}
	
	
	QFile file(filename);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("打开文件失败"));
		return;
	}
	else
	{
		QTextStream textStream(&file);
		textStream << ui.lineEdit->text() << '\n';
		for (int i = 0; i < vertexnum; i++) {
			textStream << strl_name[i] <<" ";
			textStream << strl_kind[i] << " ";;
			textStream << strl_need[i] << " ";;
			textStream << '\n';
		}
		textStream << ui.lineEdit_5->text() << '\n';
		for (int i = 0; i < edgenum; i++) {
			textStream << strl_edge[i] << " ";;
			textStream << strl_es[i] << " ";;
			textStream << '\n';
		}
		file.close();
	}
}

void create_::input_le(QString _filename) {
	QFile file(_filename);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))//打开指定文件
	{
		QMessageBox::about(NULL, "文件", "文件打开失败");
	}

	QTextStream txtInput(&file);
	QString linestr, str_1, str_2, str_3, str_4, str_5, str_6, str_7;
	QStringList stl;
	while (!txtInput.atEnd())
	{
		linestr = txtInput.readLine();  //读取数据
		stl.push_back(linestr);
	}

	int vertexnum = stl[0].toInt();
	QStringList stl1;
	ui.lineEdit->setText(stl[0]);
	for (int i = 1; i < vertexnum + 1; i++) {
		stl1 = stl[i].split(" ");
		str_1.append(stl1[0] + " ");
		str_2.append(stl1[1] + " ");
		str_3.append(stl1[2] + " ");
		stl1.clear();
	}
	ui.lineEdit_2->setText(str_1);
	ui.lineEdit_3->setText(str_2);
	ui.lineEdit_4->setText(str_3);

	int edgenum = stl[vertexnum + 1].toInt();
	ui.lineEdit_5->setText(stl[vertexnum + 1]);
	for (int i = vertexnum + 2; i < vertexnum + 2 + edgenum; i++) {
		stl1 = stl[i].split(" ");
		str_6.append(stl1[0] + " " + stl1[1]);
		if (i != vertexnum + 1 + edgenum) {
			str_6.append("|");
		}
		str_7.append(stl1[2] + " ");
	}
	ui.lineEdit_6->setText(str_6);
	ui.lineEdit_7->setText(str_7);

	file.close();
}
