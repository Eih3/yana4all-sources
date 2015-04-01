#include "server.h"

Server::Server(QWidget *parent)
    :   QDialog(parent), tcpServer(0), networkSession(0)
    {

    tcpServer = new QTcpServer(this);

    Configuration conf(QCoreApplication::applicationDirPath()+CONFIGURATION_PATH);

    if(conf.integer("USE_PROXY") == 1){
        QNetworkProxy proxy;
        proxy.setType(QNetworkProxy::HttpProxy);
        proxy.setHostName(conf.string("PROXY_HOST"));
        proxy.setPort(conf.integer("PROXY_PORT"));
        if(conf.string("PROXY_LOGIN")!="") proxy.setUser(conf.string("PROXY_LOGIN"));
        if(conf.string("PROXY_PASSWORD")!="") proxy.setPassword(conf.string("PROXY_PASSWORD"));
        tcpServer->setProxy(proxy);
    }


    if (!tcpServer->listen(QHostAddress::Any,11000)) {
        qDebug(QString("Unable to start the server: "+tcpServer->errorString()+".").toLatin1());

        return;
    }
    QString ipAddress;



    qDebug(QString("The server is running on\n\nIP: "+ipAddress+"\nport: 11000").toLatin1());
    messageSize = 0;
    QObject::connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newConnected()));
}
void Server::newConnected()
 {
     qDebug("new connection...");
     QByteArray block;
     QDataStream out(&block, QIODevice::WriteOnly);
     out.setVersion(QDataStream::Qt_4_0);
     out << (quint16)0;
     out << "OK";
     out.device()->seek(0);
     out << (quint16)(block.size() - sizeof(quint16));
     QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
     clients << clientConnection;


     connect(clientConnection, SIGNAL(readyRead()), this, SLOT(newDataReceived()));
     connect(clientConnection, SIGNAL(disconnected()),this, SLOT(newDisconnected()));


     clientConnection->write(block);
    // clientConnection->disconnectFromHost();
}

void Server::newDataReceived()
{
    qDebug("Données recues");
    // 1 : on reçoit un paquet (ou un sous-paquet) d'un des clients

    // On détermine quel client envoie le message (recherche du QTcpSocket du client)
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket == 0) // Si par hasard on n'a pas trouvé le client à l'origine du signal, on arrête la méthode
        return;

    // Si tout va bien, on continue : on récupère le message
    //QDataStream in(socket);
    QString data = socket->readAll();

    QVariantMap json = QtJson::Json::parse(data).toMap();
    qDebug(data.toUtf8());

    emit commandReceived(json);

}

void Server::newDisconnected()
 {
    qDebug("new disconnection...");


   // On détermine quel client se déconnecte
   QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
   if (socket == 0) // Si par hasard on n'a pas trouvé le client à l'origine du signal, on arrête la méthode
       return;

   clients.removeOne(socket);

   socket->deleteLater();
}

void Server::sendToAll(const QString &message)
{
    // Préparation du paquet
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    out << (quint16) 0; // On écrit 0 au début du paquet pour réserver la place pour écrire la taille
    out << message; // On ajoute le message à la suite
    out.device()->seek(0); // On se replace au début du paquet
    out << (quint16) (paquet.size() - sizeof(quint16)); // On écrase le 0 qu'on avait réservé par la longueur du message


    // Envoi du paquet préparé à tous les clients connectés au serveur
    for (int i = 0; i < clients.size(); i++)
    {
        clients[i]->write(paquet);
    }

}
