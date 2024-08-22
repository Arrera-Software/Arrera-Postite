#include "cuiapostite.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CUIAPostite w;
    w.show();
    return a.exec();
}
