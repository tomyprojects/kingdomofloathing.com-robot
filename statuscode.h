#ifndef STATUSCODE_H
#define STATUSCODE_H

#include <QMainWindow>
#include <QtNetwork>
#include <QtWebKit>

namespace status_code {

inline QUrl redirect(QUrl org, QNetworkReply* reply)
{
    QUrl rel = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
    QUrl url = org.resolved(rel);
    return url;
}

inline int httpStatusCode(QNetworkReply* reply)
{
    return reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
}

}

#endif // STATUSCODE_H
