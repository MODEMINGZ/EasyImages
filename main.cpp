#include "themainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    theMainWindow w;
    w.show();
    return a.exec();
}
