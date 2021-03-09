#include "luxianguihua.h"
#include "manager.h"
#include "create_.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	manager w;
	w.show();
	return a.exec();
}
