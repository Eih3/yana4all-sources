#ifndef LISTEN_H
#define LISTEN_H

#include "QVariantMap"
#include <pocketsphinx.h>
#include <QCoreApplication>
#include "functions.h"
#include "json.h"
class Listen
{
public:
    Listen(QVariantMap commands);
    void start();
    void wlisten();
    void stop();
    QVariantMap commands;
private:
    QProcess* process;
};

#endif // LISTEN_H
