#ifndef DOMTRAVERSAL_H
#define DOMTRAVERSAL_H

#include <QMainWindow>
#include <QtNetwork>
#include <QtWebKit>

class DomTraversal
{
public:
    typedef QSet<QString> StringSet;

    DomTraversal();

    QWebElement findPlainText(QWebElement parent, const QString & text);

    QWebElement findPlainText(QWebElement parent, StringSet text);

    QWebElement findName(QWebElement parent, const QString & name);


};

#endif // DOMTRAVERSAL_H
