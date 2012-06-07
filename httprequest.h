#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <QMainWindow>
#include <QtNetwork>
#include <QtWebKit>

namespace http_request
{
typedef QString CSSPath;
typedef QMap<QString, CSSPath> PathMap;
typedef QMap<QString, QString> StringMap;
typedef QMapIterator<QString, QString> StringMapIterator;
typedef QQueue<QString> StringQueue;

inline QByteArray createPostData(QWebView * view, const PathMap & pmap, StringQueue & keys)
{
    QWebFrame * mframe = view->page()->mainFrame();

    QUrl url;

    while (!keys.empty())
    {
        QString name = keys.dequeue();
        QWebElement e = mframe->findFirstElement(pmap.value(name));
        url.addQueryItem(name, e.attribute("value"));
    }

    return url.encodedQuery();
}

inline QByteArray createPostData(StringMap & keys)
{
    StringMapIterator i(keys);

    QUrl url;

    while (i.hasNext())
    {
        i.next();
        url.addQueryItem(i.key(), i.value());
    }

    return url.encodedQuery();
}

}

#endif // HTTPREQUEST_H
