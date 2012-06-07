#ifndef POSTREQUEST_H
#define POSTREQUEST_H

#include <QMainWindow>
#include <QtNetwork>
#include <QtWebKit>

class PostRequest
{
public:
    typedef QString CSSPath;
    typedef QMap<QString, CSSPath> PathMap;

    PostRequest();
    PostRequest(const QNetworkRequest & request);

    void setUrl(const QString &);
    void setData(QWebView * view, const PathMap & pmap, QQueue & keys);
    void post(QNetworkAccessManager *);

private:

    QUrl _data;
    QNetworkRequest _request;
};

#endif // POSTREQUEST_H
