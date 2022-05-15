#include "./include/widget.h"
#include <QApplication>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

    QApplication::setStyle("Fusion");
    QFontDatabase::addApplicationFont(":/fonts/Raleway-SemiBold.ttf");

	Widget w;
	w.show();

	return a.exec();
}
