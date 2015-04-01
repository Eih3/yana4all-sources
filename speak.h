#ifndef SPEAK_H
#define SPEAK_H

#include <QString>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QCoreApplication>
#include <QObject>
#include "functions.h"



class Speak: QObject
{
    Q_OBJECT
public:
    Speak();
    void setSentence(QString sentence);
    void espeak();
    void gspeech();
    void wspeech();
    void start();
private:
    QString sentence;
    QMediaPlaylist *playlist;
    QMediaPlayer *player;

public slots:
    void handleError(QMediaPlayer::Error error);



};

#endif // SPEAK_H
