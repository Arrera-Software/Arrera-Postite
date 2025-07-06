#ifndef CTIGERDEMON_H
#define CTIGERDEMON_H

#include <QCoreApplication>
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QEventLoop>
#include <QFile>
#include <QTextStream>
#include "version.h"

// Debug
#include <iostream>
using namespace std;

class CTigerDemon : public QObject
{
    Q_OBJECT
public:
    explicit CTigerDemon(const QString& url, const QString& nameSoft, QObject* parent = nullptr);
    bool checkUpdate();
    QString getVersionSoft();
private:
    QNetworkAccessManager* manager;
    QJsonObject contenuJSON;
    version fileversion;
};

#endif // CTIGERDEMON_H
