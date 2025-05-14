// carreraclient.cpp
#include "carreraclient.h"

CArreraClient::CArreraClient(QObject *parent, QString pnameSoft)
    : QObject{parent}, nameSoft{pnameSoft}
{
    // Connexion des signaux aux slots
    connect(&socketClient, &QWebSocket::connected, this, &CArreraClient::onConnected);
    connect(&socketClient, &QWebSocket::disconnected, this, &CArreraClient::onDisconnected);
    connect(&socketClient, &QWebSocket::textMessageReceived, this, &CArreraClient::onMessageResevied);
    connect(&socketClient, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error),
            this, &CArreraClient::onError);
}

CArreraClient::~CArreraClient()
{
    if (socketClient.state() == QAbstractSocket::ConnectedState) {
        socketClient.close();
    }
}

bool CArreraClient::connectToServeur(const QString url)
{
    socketClient.open(QUrl(url));
    return true;
}

void CArreraClient::disconnectFromServer()
{
    if (socketClient.state() == QAbstractSocket::ConnectedState) {
        socketClient.close();
    }
}

bool CArreraClient::sendMessage(const QString &message)
{
    if (!isConnected) {
        return false;
    }
    socketClient.sendTextMessage(message);
    return true;
}

void CArreraClient::onConnected()
{
    isConnected = true;
    // Envoyer le nom du logiciel dès la connexion
    socketClient.sendTextMessage("namesoft "+nameSoft + "\n");
    // Émettre le signal pour indiquer que la connexion est prête
    emit connectionEstablished();
}

void CArreraClient::onDisconnected()
{
    isConnected = false;
    emit connectionClosed();
}

void CArreraClient::onMessageResevied(const QString &message)
{
    // Émettre un signal lorsqu'un message est reçu
    emit messageReceived(message);

    if (message != "Message Received") {
        socketClient.sendTextMessage("Message Received");
    }
}

void CArreraClient::onError(QAbstractSocket::SocketError error)
{
    cout << "Erreur WebSocket: " << socketClient.errorString().toStdString()
    << " (code: " << error << ")" << endl;
}
