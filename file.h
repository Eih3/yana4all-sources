#ifndef FILE_H
#define FILE_H

#include <QFile>
#include <QTextStream>

class File
{
public:
    File();
    static QString read(QString filePath);
    static QStringList readLine(QString filePath);
    static void touch(QString filePath);
    static void write(QString filePath,QString content);
    static void remove(QString filePath);

};

#endif // FILE_H
