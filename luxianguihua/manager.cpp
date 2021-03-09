#include "manager.h"
#include <qpushbutton.h>

manager::manager(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setWindowTitle(QString::fromLocal8Bit("路线规划"));
	connect(ui.pushButton, &QPushButton::clicked, this, &manager::_create);
	connect(ui.pushButton_2, &QPushButton::clicked, this, &manager::load);
	connect(ui.pushButton_3, &QPushButton::clicked, this, &manager::_about);
	connect(&ct, &create_::sub_ct, this, &manager::_create2);
	connect(&lxgh, &luxianguihua::sub, this, &manager::_create1);
	connect(&lxgh, &luxianguihua::upgrade_signal, this, &manager::_create1);
}

manager::~manager()
{
}

void manager::_create() {
	this->hide();
	ct.flag = lxgh.flag;
	ct.show();
}

void manager::_create1() {
	lxgh.hide();
	ct.flag = lxgh.flag;
	ct.show();
}

void manager::_create2() {
	ct.hide();
	ct.ct_load(&lxgh);
	lxgh.filename = ct.filename;
	filename = ct.filename;
	lxgh.show();
}

void manager::load() {
	this->hide();
	lxgh.loading();
	ct.filename = lxgh.filename;
	filename = lxgh.filename;
	input_le();
	lxgh.show();
}

void manager::_about() {
	ab.show();
}

void manager::input_le() {
	ct.input_le(filename);
}