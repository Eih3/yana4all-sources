#ifndef TRAYICON_H
#define TRAYICON_H

#include <QSystemTrayIcon>
#include <QCoreApplication>
#include <QMenu>
#include <QAction>



class MainIcon
{

public:
    MainIcon(QWidget *parent = 0);
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
};

#endif // TRAYICON_H
