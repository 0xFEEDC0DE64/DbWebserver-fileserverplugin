#pragma once

#include "webapplication.h"

class QJsonObject;

class FileserverApplication : public WebApplication
{
    Q_OBJECT

public:
    FileserverApplication(const QJsonObject &config, QObject *parent = Q_NULLPTR);

    void start() Q_DECL_OVERRIDE;

    void handleRequest(HttpClientConnection *connection, const HttpRequest &request) Q_DECL_OVERRIDE;
};
