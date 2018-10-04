#include "fileserverapplication.h"

#include <QJsonObject>
#include <QJsonValue>
#include <QFileInfo>
#include <QDir>
#include <QUrl>
#include <QFile>
#include <QDebug>

#include <memory>

#include "cpp14polyfills.h"
#include "webserver.h"
#include "httprequest.h"
#include "httpresponse.h"
#include "httpclientconnection.h"
#include "httpnotfoundexception.h"
#include "httpexception.h"

const QString FileserverApplication::SERVER_NAME(QStringLiteral("Fileserver 1.0"));
const QDir FileserverApplication::PLUGIN_RESOURCES_DIR(QStringLiteral(":/webserver/plugins/fileserverplugin"));

FileserverApplication::FileserverApplication(const QJsonObject &config, WebServer &webServer) :
    WebApplication(&webServer), m_webServer(webServer)
{
    if(!config.contains(QStringLiteral("rootPath")))
        throw std::runtime_error("Fileserver application does not contain rootPath");

    const auto rootPathVal = config.value(QStringLiteral("rootPath"));
    if(!rootPathVal.isString())
        throw std::runtime_error("Fileserver application rootPath is not a string");

    m_rootPath = QDir(rootPathVal.toString());
}

void FileserverApplication::start()
{
}

void FileserverApplication::handleRequest(HttpClientConnection *connection, const HttpRequest &request)
{
    static const QString pluginPrefix(QStringLiteral("/:fileserverplugin/"));
    const auto pluginResource = request.path.startsWith(pluginPrefix);

    const QFileInfo fileInfo((pluginResource ? PLUGIN_RESOURCES_DIR : m_rootPath)
                             .absoluteFilePath(request.path.mid(pluginResource ? pluginPrefix.length() : 1)));

    if(!fileInfo.exists())
        throw HttpNotFoundException(request);

    if(fileInfo.isDir())
    {
        if(!request.path.endsWith(QLatin1Char('/')))
        {
            const QString newUrl = request.path + '/';

            HttpResponse response;
            response.protocol = request.protocol;
            response.statusCode = HttpResponse::StatusCode::MovedPermanently;
            response.headers.insert(HttpResponse::HEADER_SERVER, SERVER_NAME);
            response.headers.insert(HttpResponse::HEADER_LOCATION, newUrl);
            connection->sendResponse(response, QStringLiteral("<a href=\"%0\">%1</a>").arg(QString(QUrl::toPercentEncoding(newUrl)).toHtmlEscaped()).arg(tr("Moved.")));
            return;
        }

        const QDir dir(fileInfo.absoluteFilePath());

        QString content;
        static const QString line(QStringLiteral("<tr><td><img src=\"/:fileserverplugin/images/%0\" /> <a href=\"%1\">%2</a></td><td>%3</td></tr>"));

        if(request.path != QStringLiteral("/"))
            content.append(line.arg(QStringLiteral("back.png")).arg(QStringLiteral("..")).arg(tr("Parent directory")).arg(QString()));

        for(auto info : dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot))
            content.append(line
                    .arg(info.isDir() ? QStringLiteral("folder.png") : QStringLiteral("text.png"))
                    .arg(QString(QUrl::toPercentEncoding(info.fileName())).toHtmlEscaped())
                    .arg(info.fileName().toHtmlEscaped())
                    .arg(info.isDir() ? QString() : formatSize(info.size())));

        HttpResponse response;
        response.protocol = request.protocol;
        response.statusCode = HttpResponse::StatusCode::OK;
        response.headers.insert(HttpResponse::HEADER_SERVER, SERVER_NAME);
        connection->sendResponse(response, QStringLiteral("<!doctype html>"
                                                          "<html lang=\"en\">"
                                                            "<head>"
                                                              "<meta charset=\"utf-8\">"
                                                              "<link rel=\"stylesheet\" href=\"https://stackpath.bootstrapcdn.com/bootstrap/4.1.3/css/bootstrap.min.css\" integrity=\"sha384-MCw98/SFnGE8fJT3GXwEOngsV7Zt27NXFoaoApmYm81iuXoPkFOJwJ8ERdknLPMO\" crossorigin=\"anonymous\">"
                                                            "</head>"
                                                            "<body>"
                                                              "<div class=\"container\">"
                                                                "<table class=\"table\">"
                                                                  "<thead>"
                                                                    "<tr>"
                                                                      "<th>Filename</th>"
                                                                      "<th>Size</th>"
                                                                    "</tr>"
                                                                  "</thead>"
                                                                  "<tbody>"
                                                                    "%0"
                                                                  "</tbody>"
                                                                "</table>"
                                                              "</div>"
                                                            "</body>"
                                                          "</html>").arg(content));
    }
    else if(fileInfo.isFile())
    {
        std::unique_ptr<QFileDevice> file = std::make_unique<QFile>(fileInfo.filePath());
        if(!file->open(QIODevice::ReadOnly))
            throw HttpException(request, tr("Could not open requested file \"%0\" because %1").arg(request.path, file->errorString()).toStdString());

        HttpResponse response;
        response.protocol = request.protocol;
        response.statusCode = HttpResponse::StatusCode::OK;
        response.headers.insert(HttpResponse::HEADER_SERVER, SERVER_NAME);
        connection->sendResponse(response, std::move(file));
    }
}

QString FileserverApplication::formatSize(qint64 size)
{
    static const QStringList suffixes { QStringLiteral("B"), QStringLiteral("kB"), QStringLiteral("MB"), QStringLiteral("GB"), QStringLiteral("TB") };
    static const int stepSize = 1024;

    for(const auto &suffix : suffixes)
    {
        if(size < stepSize)
            return QStringLiteral("%0 %1").arg(size).arg(suffix);
        size /= stepSize;
    }

    return QStringLiteral("%0 %1").arg(size).arg(suffixes.last());
}
