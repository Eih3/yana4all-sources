#include "configuration.h"


Configuration::Configuration(QString filePath)
{
     path = filePath;
     QString stream = File::read(path);
     json = QtJson::Json::parse(stream).toMap();
}
void Configuration::put(QString key,QString value)
{
       json[key] = value;
       QString stream = QtJson::Json::serialize(json);
       File::write(path,stream);
}
QString Configuration::string(QString key)
{
    return json.contains(key)?json[key].toString():"";
}
bool Configuration::boolean(QString key)
{
    return json.contains(key) && json[key].toString()=="true"?true:false;
}
int Configuration::integer(QString key)
{
    return json.contains(key)?json[key].toInt():0;
}

