#include "fileserverplugin.h"

#include "fileserverapplication.h"

FileserverPlugin::FileserverPlugin(QObject *parent) :
    WebPlugin(parent)
{

}

QString FileserverPlugin::pluginName() const
{
    return QStringLiteral("fileserver");
}

WebApplication *FileserverPlugin::createApplication(const QJsonObject &config, WebServer &webServer) const
{
    return new FileserverApplication(config, webServer);
}
