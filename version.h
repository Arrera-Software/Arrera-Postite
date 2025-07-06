#ifndef VERSION_H
#define VERSION_H

#include <QString>

class version{
private :
    const QString version = "I2025-2.10" ;
    const QString name = "ARRERA-POSTITE" ;
public :
    QString getVersion(){
        return version;
    };

    QString getName(){
        return name;
    };
};

#endif // VERSION_H
