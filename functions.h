#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QString>
#include <QProcess>

class Functions
{
public:
    Functions();
    static QProcess *command(QString);
    static QProcess *command(QString,bool);
    static QString getOs();
};

#endif // FUNCTIONS_H
