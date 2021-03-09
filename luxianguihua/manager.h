#pragma once

#include <QWidget>
#include "ui_manager.h"
#include "luxianguihua.h"
#include "create_.h"
#include "about_.h"

class manager : public QWidget
{
	Q_OBJECT
public:
	manager(QWidget *parent = Q_NULLPTR);
	~manager();
	void _create();
	void load();
	void _about();
	void _create1();
	void _create2();
	void input_le();
private:
	Ui::manager ui;
	luxianguihua lxgh;
	create_ ct;
	about_ ab;
	QString filename;
};
