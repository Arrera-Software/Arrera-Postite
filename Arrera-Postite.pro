QT       += core gui printsupport network websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    carreraclient.cpp \
    cdetectionos.cpp \
    ctigerdemon.cpp \
    fenetreview.cpp \
    main.cpp \
    cuiapostite.cpp \
    mytextedit.cpp \
    windowsupdate.cpp

HEADERS += \
    carreraclient.h \
    cdetectionos.h \
    ctigerdemon.h \
    cuiapostite.h \
    fenetreview.h \
    mytextedit.h \
    windowsupdate.h

FORMS += \
    cuiapostite.ui \
    fenetreview.ui \
    windowsupdate.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    postitesrc.qrc
