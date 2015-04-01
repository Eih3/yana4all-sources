#include "functions.h"

Functions::Functions(){}

QString Functions::getOs(){
    QString OS = "Unknown";
    #ifdef Q_OS_LINUX
        OS = "Linux";
    #endif
    #ifdef Q_OS_WIN
        OS = "Windows";
    #endif
    #ifdef Q_OS_MACX
        OS = "Mac";
    #endif
        return OS;
}



QProcess* Functions::command(QString program){
    return Functions::command(program,false);
}

QProcess* Functions::command(QString program,bool synchronous){
    QProcess *process = new QProcess();
    process->start(program);
    if(synchronous) process->waitForFinished();
    return process;
}
