#include "fileserverapplication.h"

#include "httprequest.h"
#include "httpresponse.h"
#include "httpclientconnection.h"

FileserverApplication::FileserverApplication(const QJsonObject &config, QObject *parent) :
    WebApplication(parent)
{

}

void FileserverApplication::start()
{

}

void FileserverApplication::handleRequest(HttpClientConnection *connection, const HttpRequest &request)
{
    HttpResponse response;
    response.protocol = request.protocol;
    response.statusCode = HttpResponse::StatusCode::OK;
    connection->sendResponse(response, "Hello from FileserverApplication: " + request.path);
}
