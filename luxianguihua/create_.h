#pragma once

#include <QWidget>
#include "ui_create_.h"
#include "luxianguihua.h"
#include <signal.h>
#include <qfile.h>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qtextstream.h>
#include <qtextedit.h>
#include <qfiledevice.h>
#include <qevent.h>
#include "GA.h"

class create_ : public QWidget
{
	Q_OBJECT

public:
	create_(QWidget *parent = Q_NULLPTR);
	~create_();
	void create_ct();
	void ct_load(luxianguihua *lxgh);
	void input_le(QString _filename);
	QString filename;
	int flag;
signals:
	void sub_ct();
private:
	Ui::create_ ui;
	luxianguihua lxgh;
};
