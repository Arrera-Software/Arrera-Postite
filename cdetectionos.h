#ifndef CDETECTIONOS_H
#define CDETECTIONOS_H
#include <QCoreApplication>

class CDetectionOS
{
private :
    bool osLinux;
    bool osWin;
    bool osApple;
public:
    CDetectionOS();
    bool getosLinux();
    bool getosWin();
    bool getosApple();
};

#endif // CDETECTIONOS_H
