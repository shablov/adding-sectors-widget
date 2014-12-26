#include "sectorswidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	SectorsWidget w;
	w.show();

	return a.exec();
}
