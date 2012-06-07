#include "postrequest.h"

PostRequest::PostRequest()
{
}

void PostRequest::setUrl(const QString & url)
{
    _request.setUrl(QUrl(url));
}

void PostRequest::setData(QWebView * view, PathMap & pmap, QQueue & keys);
void PostRequest::post(QNetworkAccessManager *);
