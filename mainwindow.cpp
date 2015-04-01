#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // INITIALISATION
    ui->setupUi(this);
    this->dragFlag = false;
    micon = new MainIcon(this);
    log = &Singleton<Log>::Instance();
    log->put("Démarrage programme");

    setWindowTitle("Yana");
    resize(QSize(532,500));
    setWindowFlags(Qt::FramelessWindowHint);
    setStyleSheet("background:transparent;");
    setAttribute(Qt::WA_TranslucentBackground);


    conf = new Configuration(QCoreApplication::applicationDirPath()+CONFIGURATION_PATH);
    setWindowTitle(conf->string("VOCAL_ENTITY_NAME"));
    skinVariables["ip"] = conf->string("API_URL");
    skinVariables["version"] = PROGRAM_VERSION;
    skinVariables["name"] = PROGRAM_NAME;
    skinVariables["os"] = "Os : "+Functions::getOs() ;
    skinVariables["vocal-entity"] = conf->string("VOCAL_ENTITY_NAME");

    QIcon appIcon = QIcon(QCoreApplication::applicationDirPath()+"\\icon.ico");
    setWindowIcon(appIcon);

    //CHARGEMENT DE L'UI
    ui->webView->load(QUrl("file:///"+QCoreApplication::applicationDirPath()+SKIN_INDEX));
    ui->webView->page()->settings()->setAttribute(QWebSettings::LocalContentCanAccessRemoteUrls,true);
    ui->webView->page()->settings()->setAttribute(QWebSettings::LocalContentCanAccessFileUrls,true);
    ui->webView->page()->settings()->setAttribute(QWebSettings::LocalStorageEnabled,true);
    ui->webView->page()->settings()->setAttribute(QWebSettings::AutoLoadImages,true);
    ui->webView->page()->settings()->setAttribute(QWebSettings::PrivateBrowsingEnabled,true);
    ui->webView->page()->settings()->setMaximumPagesInCache(0);
    ui->webView->page()->settings()->clearMemoryCaches();

    QWebSettings::globalSettings()->setMaximumPagesInCache(0);
    QWebSettings::globalSettings()->setObjectCacheCapacities(0, 0, 0);

    QWebSettings::globalSettings()->setAttribute(QWebSettings::LocalContentCanAccessRemoteUrls,true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::LocalContentCanAccessFileUrls,true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::LocalStorageEnabled,true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::AutoLoadImages,true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::PrivateBrowsingEnabled,true);
    QWebSettings::globalSettings()->setMaximumPagesInCache(0);
    QWebSettings::globalSettings()->clearMemoryCaches();


    ui->webView->setContextMenuPolicy(Qt::DefaultContextMenu);
    ui->webView->page()->settings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);

    //Attaque http
    httpManager = new QNetworkAccessManager(this);
    connect(httpManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(requestReceived(QNetworkReply*)));

    if(conf->integer("USE_PROXY") == 1){
        QNetworkProxy proxy;
        proxy.setType(QNetworkProxy::HttpProxy);
        proxy.setHostName(conf->string("PROXY_HOST"));
        proxy.setPort(conf->integer("PROXY_PORT"));
        if(conf->string("PROXY_LOGIN")!="") proxy.setUser(conf->string("PROXY_LOGIN"));
        if(conf->string("PROXY_PASSWORD")!="") proxy.setPassword(conf->string("PROXY_PASSWORD"));
        httpManager->setProxy(proxy);
    }

    QNetworkRequest getCommands(QUrl(conf->string("API_URL")+"/"+conf->string("COMMAND_ACTION")+"&token="+conf->string("TOKEN")));

    httpManager->get(getCommands);


    //Lancement du serveur socket
    Server* server= new Server(this);



    //GESTION DES EVENEMENTS
    //Quand l'object javascript est intialisé
    connect(ui->webView->page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()), this, SLOT(mainFrame_javaScriptWindowObjectCleared()));
    //Quand la page est chargée
    connect(ui->webView, SIGNAL(loadFinished(bool)), this, SLOT(mainFrame_pageLoaded()));
    // Quand une commande socket est recue
    QObject::connect(server,SIGNAL(commandReceived(QVariantMap)),this, SLOT(commandReceived(QVariantMap)));


}

//Reception json par socket
void MainWindow::commandReceived(QVariantMap json){
      handleCommand(json);
}

//Traitement de la commande json reçu par socket
void MainWindow::handleCommand(QVariantMap json){

      QVariantList responses = json["responses"].toList();
      QVariantMap response = responses[0].toMap();

      if(response["type"].toString() == "talk"){
          log->put(QString("Commande serveur type 'parole' -> "+response["sentence"].toString()));
          Speak* speak = new Speak();
          speak->setSentence(response["sentence"].toString());
          messageToShow("bot",response["sentence"].toString(),"vocal-command");
          speak->start();
      }else if(response["type"].toString() == "execute"){
          log->put(QString("Commande serveur type 'execution' -> "+response["program"].toString()));
          Functions::command(response["program"].toString());
          messageToShow("bot",response["program"].toString()+"","execute-command");
      }else if(response["type"].toString() == "sound"){
          log->put(QString("Commande serveur type 'son' -> "+response["file"].toString()));
          qDebug(QString(QCoreApplication::applicationDirPath()+"\\sons\\"+response["file"].toString()).toLatin1());
          messageToShow("bot","*"+response["file"].toString()+"*","sound-command");
          QSound::play(QCoreApplication::applicationDirPath()+"\\sons\\"+response["file"].toString());
      }else if(response["type"].toString() == "listen"){

          log->put(QString("Commande vocale utilisateur -> "+response["sentence"].toString()));
          messageToShow("user",response["sentence"].toString(),"user-command");


          QVariantList list = vocalCommands["commands"].toList();
          QString commandUrl;
           foreach (QVariant command,list){
             QMap<QString,QVariant> map = command.toMap();

             if (map["command"].toString() == response["sentence"].toString()){
                 commandUrl = map["url"].toString();
             }
           }

          if(commandUrl!=NULL)
          httpManager->get(QNetworkRequest(QUrl(commandUrl)));
      }
}


//Reception réponse HTTP
void MainWindow::requestReceived(QNetworkReply* reply){
    QNetworkRequest r = reply->request();
    QString response = reply->readAll();

    if (response.contains("\\u")) {
       do {
       int idx = response.indexOf("\\u");
       QString strHex = response.mid(idx, 6);
       strHex = strHex.replace("\\u", QString());
       int nHex = strHex.toInt(0, 16);
       response.replace(idx, 6, QChar(nHex));
       } while (response.indexOf("\\u") != -1);
    }
    /*
    @TODO remplacer par :
    int idx = -1;
    while ( ( idx = indexOf("\\u") ) != -1 ) {
    int nHex = mid(idx + 2, 4).toInt(0, 16);
    replace(idx, 6, QChar(nHex));
    }
    */

    qDebug(response.toUtf8());
    QVariantMap jsonCommand = QtJson::Json::parse(response).toMap();



    if(r.url().toString() == conf->string("API_URL")+"/"+conf->string("COMMAND_ACTION")+"&token="+conf->string("TOKEN")){
        vocalCommands = jsonCommand;
        listen = new Listen(vocalCommands);
        listen->start();
    }else{
        handleCommand(jsonCommand);
    }

}


void MainWindow::messageToShow(QString emiter,QString message,QString css){



    QWebFrame *frame = ui->webView->page()->mainFrame();
    QWebElement templateDiv;
    if(emiter == "bot"){
        templateDiv = frame->findFirstElement("#messageResponse");
    }else{
        templateDiv = frame->findFirstElement("#messageQuestion");
    }
    QWebElement htmlMessage = templateDiv.clone();
    htmlMessage.addClass(css);
    htmlMessage.setStyleProperty("display","block");



    htmlMessage.findFirst(".message").setInnerXml(message);
    frame->findFirstElement("#messages").appendInside(htmlMessage);
}

MainWindow::~MainWindow()
{
    delete ui;
    listen->stop();
}
void MainWindow::mainFrame_pageLoaded(){

    //Remplacement des tags de variables de skin par leurs valeurs
    QWebFrame *frame = ui->webView->page()->mainFrame();
    log->logElement = frame->findFirstElement("#debugList");

    QHash<QString, QString>::iterator i;
    for (i = skinVariables.begin(); i != skinVariables.end(); ++i){
        QWebElementCollection collection =frame->findAllElements("yana-"+i.key());
        foreach(QWebElement content,collection ){
            content.replace(i.value());
        }
    }

    QSize docSize = frame->contentsSize();
    resize( QSize(docSize.width()+30,docSize.height()+30));

    this->messageToShow("user","Wake up "+conf->string("VOCAL_ENTITY_NAME")+" !","vocal-command");
    this->messageToShow("bot","Hello world !","vocal-command");

}


void MainWindow::mainFrame_javaScriptWindowObjectCleared(){
    ui->webView->page()->mainFrame()->addToJavaScriptWindowObject("yana", this);
}


void MainWindow::trayCliqued(QSystemTrayIcon::ActivationReason reason){
    this->show();
}

void MainWindow::hide(){
    this->setVisible(false);
}

void MainWindow::show(){
    this->setVisible(true);
}

void MainWindow::exit(){
    qApp->quit();
}

QString MainWindow::config(QString key){
    return conf->string(key);
}
void MainWindow::config(QString key,QString value){
    conf->put( key, value);
}

void MainWindow::minimize(){
    showMinimized();

}
void MainWindow::maximize(){
    showMaximized();
}
void MainWindow::restore(){
    showNormal();
}
void MainWindow::dragStart(){
    this->dragFlag = true;
    this->dragPosition = this->mousePosition;

}
void MainWindow::dragStop(){
    this->dragFlag = false;
    this->dragPosition = QPoint(0,0);
}
void MainWindow::dragMove(int x, int y){
    this->mousePosition = QPoint(x,y);
    if(this->dragFlag){
        move(QPoint(this->x()+x-this->dragPosition.x(),this->y()+y-this->dragPosition.y()));
    }
}

void MainWindow::moveWindow(int x, int y){
    move(QPoint(x,y));
}
void MainWindow::resizeWindow(int width, int height){
    resize(QSize(width,height));
}
void MainWindow::url(QString url){
   QDesktopServices::openUrl (url);
}
QString MainWindow::commands(){

    return  QtJson::Json::serialize(this->vocalCommands);
}

