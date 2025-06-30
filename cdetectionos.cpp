#include "cdetectionos.h"

CDetectionOS::CDetectionOS() {
    osWin = false;
    osLinux = false;
    osApple = false;
    #if defined(Q_OS_WIN)
        osWin = true;
    #elif defined(Q_OS_LINUX)
        osLinux = true;
    #elif defined(Q_OS_MAC)
        osApple = true;
    #endif
}

bool CDetectionOS::getosLinux(){
    return osLinux;
}

bool CDetectionOS::getosWin(){
    return osWin;
}

bool CDetectionOS::getosApple(){
    return osApple;
}
