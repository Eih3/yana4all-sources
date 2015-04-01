#ifndef SERVER_H
#define SERVER_H
#include <QDialog>
#include <QTcpServer>
#include <QNetworkInterface>
#include <QNetworkSession>
#include <QNetworkProxy>
#include <QCoreApplication>
#include <QTcpSocket>
#include "constant.h"
#include "configuration.h"
#include "json.h"

class Server : public QDialog
{
     Q_OBJECT
public:
    Server(QWidget *parent = 0);
    void sendToAll(const QString &message);
signals:
    void commandReceived(QVariantMap);
public slots:
   void newConnected();
   void newDisconnected();
   void newDataReceived();
private:
    QTcpServer *tcpServer;
    QNetworkSession *networkSession;
    QList<QTcpSocket *> clients;
    quint16 messageSize;
};

#endif // SERVER_H
