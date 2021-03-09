#pragma once

#include <QWidget>
#include "ui_about_.h"

class about_ : public QWidget
{
	Q_OBJECT

public:
	about_(QWidget *parent = Q_NULLPTR);
	~about_();

private:
	Ui::about_ ui;
};
