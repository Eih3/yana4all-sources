#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWebView>
#include <QWebFrame>
#include <QNetworkProxy>
#include <QSound>
#include <QWebElementCollection>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDesktopServices>
#include "singleton.h"
#include "speak.h"
#include "constant.h"
#include "functions.h"
#include "mainicon.h"
#include "configuration.h"
#include "server.h"
#include "listen.h"
#include "log.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    Q_INVOKABLE void hide();
    Q_INVOKABLE void exit();
    Q_INVOKABLE void maximize();
    Q_INVOKABLE void minimize();
    Q_INVOKABLE void restore();
    Q_INVOKABLE void dragStart();
    Q_INVOKABLE void  dragStop();
    Q_INVOKABLE void dragMove(int x, int y);
    Q_INVOKABLE void moveWindow(int x, int y);
    Q_INVOKABLE void resizeWindow(int width, int height);
    Q_INVOKABLE QString config(QString key);
    Q_INVOKABLE void config(QString key,QString value);
    Q_INVOKABLE QString commands();
    Q_INVOKABLE void url(QString);
    Q_INVOKABLE void show();

    void handleCommand(QVariantMap json);

    ~MainWindow();
public slots:
    void mainFrame_javaScriptWindowObjectCleared();
    void mainFrame_pageLoaded();
    void requestReceived(QNetworkReply* reply);
    void messageToShow(QString emiter,QString message,QString css);
    void commandReceived(QVariantMap json);
    void trayCliqued(QSystemTrayIcon::ActivationReason);


private:
    Ui::MainWindow *ui;
    MainIcon *micon;
    QHash<QString, QString> skinVariables;
    bool dragFlag;
    QPoint mousePosition;
    QPoint dragPosition;
    Configuration *conf;
    QVariantMap vocalCommands;
    Log *log;
    QNetworkAccessManager *httpManager;
    Listen* listen;
};

#endif // MAINWINDOW_H
