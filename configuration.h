#ifndef CONFIGURATION_H
#define CONFIGURATION_H
#include "file.h"
#include "json.h"
class Configuration
{
public:
    Configuration(QString);
    QString string(QString);
    void put(QString,QString);
    bool boolean(QString);
    int integer(QString);
    QVariantMap json;
    QString path;
};

#endif // CONFIGURATION_H
