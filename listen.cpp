#include "listen.h"

#define MODELDIR "D:/libs/sphinx/models"
Listen::Listen(QVariantMap commands)
{

    this->commands = commands;
   /* ps_decoder_t *ps;
           cmd_ln_t *config;

           config = cmd_ln_init(NULL, ps_args(), TRUE,"-hmm", MODELDIR "/lium_french_f0", "-lm", MODELDIR "/0991.lm","-dict", MODELDIR "/0991.dic",NULL);
           if (config != NULL){
               ps = ps_init(config);
           }

*/

}


void Listen::start()
{
    if (Functions::getOs() == "Windows") this->wlisten();
}

void Listen::stop()
{
    process->close();
    process->kill();
}

void Listen::wlisten()
{
    QString jsonCommands = QtJson::Json::serialize(this->commands);
    jsonCommands =  jsonCommands.toUtf8();
    jsonCommands = jsonCommands.replace("\"","'");
    QString program = "\""+QCoreApplication::applicationDirPath()+"\\bin\\winstt.exe\"";
    program += " -l \""+jsonCommands+"\"";
    qDebug(program.toUtf8());
    process = Functions::command(program);
}
