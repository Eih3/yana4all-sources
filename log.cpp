#include "log.h"
#include "singleton.h"

Log::Log()
{


}

void Log::put(QString message){
    this->put(message,"notice");
}

void Log::put(QString message,QString type){
    QDateTime date =  QDateTime::currentDateTime();
    qDebug(qPrintable(date.toString("hh:mm:ss")+" "+message.toUtf8()));
    logElement.appendInside("<li class='log-"+type+"'><i></i><span>"+date.toString("hh:mm:ss")+"<span> "+message+"</li>");
}
