#include "speak.h"

Speak::Speak()
{


}

void Speak::setSentence(QString sentence){
    this->sentence = sentence;
}
void Speak::start(){
     if (Functions::getOs() == "Windows") this->wspeech();
     if (Functions::getOs() != "Windows") this->gspeech();
}

void Speak::handleError(QMediaPlayer::Error error){
    this->espeak();
}

void Speak::gspeech(){
    QString url = "http://translate.google.com/translate_tts?ie=UTF-8&tl=%1&q=%2";

    playlist = new QMediaPlaylist;
    player = new QMediaPlayer;

    QObject::connect(player, SIGNAL(error(QMediaPlayer::Error)),this, SLOT(handleError(QMediaPlayer::Error)));

        if (QMultimedia::Available == 0)  {
                QString text = this->sentence;
                text.replace(" ","+");
                QString finalUrl = url.arg("fr").arg(text);
                playlist->addMedia(QUrl(finalUrl));
                player->setPlaylist(playlist);
                player->play();
        }

}

void Speak::wspeech(){
    QString program = "\""+QCoreApplication::applicationDirPath()+"\\bin\\wintts.exe\"";
    program += " -t \""+this->sentence+"\"";
    qDebug(program.toLatin1());
    Functions::command(program);
}





void Speak::espeak(){
    QString program = "\""+QCoreApplication::applicationDirPath()+"\\bin\\espeak";
    if (Functions::getOs() == "Mac") program += "mac";
    program += "\" -v french+f2 -p 30 -s 130 \""+this->sentence+"\"";

    Functions::command(program);
}
