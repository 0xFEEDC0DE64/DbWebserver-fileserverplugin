#pragma once

#include "webapplication.h"

#include <QDir>

class QJsonObject;

class WebServer;

class FileserverApplication : public WebApplication
{
    Q_OBJECT
    static const QString SERVER_NAME;
    static const QDir PLUGIN_RESOURCES_DIR;

public:
    FileserverApplication(const QJsonObject &config, WebServer &webServer);

    void start() Q_DECL_OVERRIDE;

    void handleRequest(HttpClientConnection *connection, const HttpRequest &request) Q_DECL_OVERRIDE;

private:
    static QString formatSize(qint64 size);

    WebServer &m_webServer;
    QDir m_rootPath;
};
