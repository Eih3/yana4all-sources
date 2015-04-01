#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen>
#include <QTextCodec>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


   // QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));


    QPixmap pixmap(QCoreApplication::applicationDirPath()+SPLASH_PATH);
    QSplashScreen splash(pixmap);
    splash.show();
    a.processEvents();
    MainWindow w;
    w.show();
    splash.hide();

    return a.exec();
}
