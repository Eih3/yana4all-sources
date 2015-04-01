#ifndef LOG_H
#define LOG_H
#include <QString>
#include <QWebElement>
#include <QDateTime>

class Log
{

public:
    Log();
    void put(QString);
    void put(QString message,QString type);
    QWebElement logElement;
private:



};

#endif // LOG_H
