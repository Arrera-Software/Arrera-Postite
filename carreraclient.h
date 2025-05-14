// carreraclient.h
#ifndef CARRERACLIENT_H
#define CARRERACLIENT_H

#include <QObject>
#include <QWebSocket>
#include <iostream>
#include <QJsonDocument>
#include <QJsonObject>

using namespace std;

class CArreraClient : public QObject
{
    Q_OBJECT
private:
    QWebSocket socketClient;
    QString nameSoft;
    volatile bool isConnected = false;

public:
    explicit CArreraClient(QObject *parent = nullptr, QString pnameSoft = "");
    ~CArreraClient();

    bool connectToServeur(const QString url);
    bool sendMessage(const QString &message);
    void disconnectFromServer();
    bool isServerConnected() const { return isConnected; }

signals:
    void messageReceived(const QString &message);
    void connectionEstablished();
    void connectionClosed();

private slots:
    void onConnected();
    void onDisconnected();
    void onMessageResevied(const QString &message);
    void onError(QAbstractSocket::SocketError error);
};

#endif // CARRERACLIENT_H
