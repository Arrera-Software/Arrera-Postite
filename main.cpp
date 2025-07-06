#include "cuiapostite.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("arrera");
    QCoreApplication::setApplicationName("Arrera-Postite");

    QApplication a(argc, argv);
    CUIAPostite w;
    if (w.osDect.getosApple()){
        QFile styleFile(":/style/MacOS.qss");
        if (styleFile.open(QIODevice::ReadOnly | QIODevice::Text)){
            w.setStyleSheet(QString::fromUtf8(styleFile.readAll()));
            styleFile.close();
        }
    }
    w.show();
    return a.exec();
}
