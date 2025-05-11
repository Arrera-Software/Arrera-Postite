#ifndef CTIGERDEMON_H
#define CTIGERDEMON_H

#include <QCoreApplication>
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <iostream>
#include <QEventLoop>
#include <QFile>
#include <QTextStream>

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
};

#endif // CTIGERDEMON_H
