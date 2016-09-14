#include "qarduinoexamplewidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QArduinoExampleWidget w;
    w.show();

    return a.exec();
}
