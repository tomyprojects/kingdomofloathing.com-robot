#include "domtraversal.h"

DomTraversal::DomTraversal()
{
}

QWebElement DomTraversal::findPlainText(QWebElement parent, const QString & text)
{
    QWebElement we = parent.firstChild();
    while (!we.isNull())
    {
        if (we.toPlainText() == text)
            return we;

        QWebElement we2 = findPlainText(we, text);

        if (!we2.isNull())
            return we2;

        we = we.nextSibling();
    }

    return we;
}

QWebElement DomTraversal::findPlainText(QWebElement parent, StringSet text)
{
    QWebElement we = parent.firstChild();
    while (!we.isNull())
    {
        QString plainText = we.toPlainText();

        if (text.contains(plainText))
            return we;

        QWebElement we2 = findPlainText(we, text);

        if (!we2.isNull())
            return we2;

        we = we.nextSibling();
    }

    return we;
}

QWebElement DomTraversal::findName(QWebElement parent, const QString & name)
{
    QWebElement we = parent.firstChild();
    while (!we.isNull())
    {
        if (we.attribute("name") == name)
            return we;

        QWebElement we2 = findName(we, name);

        if (!we2.isNull())
            return we2;

        we = we.nextSibling();
    }

    return we;
}
