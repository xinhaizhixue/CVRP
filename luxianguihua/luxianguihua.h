#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_luxianguihua.h"
#include "GA.h"
#include <signal.h>

extern vector<vector<int> > path;
extern vector<vector<string> > find_path(vector<vector<int>> &each_road, int roadnum);

class luxianguihua : public QMainWindow
{
	Q_OBJECT

public:
	luxianguihua(QWidget *parent = Q_NULLPTR);
	void loading();
	void weight_c();
	void weight_d();
	void weight_n();
	void weight_l();
	void _show();
	void imformation();
	void send_signal();
	void send_upgrade_signal();
	graph *p;
	QString filename;
	int flag;
signals:
	void sub();
	void upgrade_signal();
private:
	Ui::luxianguihuaClass ui;
	individuality *bestone;
	int weight;
};
