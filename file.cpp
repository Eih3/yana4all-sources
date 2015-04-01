#include "file.h"

File::File(){}

QString File::read(QString filePath) {
    QFile file(filePath);
    QString texte;

    if ( file.open(QIODevice::ReadOnly | QIODevice::Text) ) {
        QTextStream flux(&file);
        flux.setCodec("UTF-8");
        texte = flux.readAll();
        file.close();
    } else {
    }
    return texte;
}
void File::write(QString filePath,QString content){
    QFile file(filePath);
    if (file.open(QFile::WriteOnly)) {
        QTextStream out(&file);
        out.setCodec("UTF-8");
        out << content;
    }
}
void File::remove(QString filePath){
    QFile file(filePath);
    file.remove();
}
void File::touch(QString filePath){
    File::write(filePath,"");
}
