#ifndef VERSION_H
#define VERSION_H

#include <QString>

class version{
private :
    const QString version = "IXXXX-XXX" ;
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
