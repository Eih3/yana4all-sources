#include "mainicon.h"

MainIcon::MainIcon(QWidget *parent)
{
    QIcon appIcon = QIcon(QCoreApplication::applicationDirPath()+"\\icon.ico");
    QAction* actionExit = new QAction( "Quitter", parent );
    QObject::connect( actionExit, SIGNAL( triggered() ), parent, SLOT( exit()));

    trayIconMenu = new QMenu( parent );
    trayIconMenu->addAction( actionExit );

    trayIcon = new QSystemTrayIcon(parent);


    trayIcon->setToolTip("Yana");
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setIcon(appIcon);
    trayIcon->show();

    QObject::connect( trayIcon, SIGNAL( activated(QSystemTrayIcon::ActivationReason) ), parent, SLOT( trayCliqued(QSystemTrayIcon::ActivationReason)));

}
