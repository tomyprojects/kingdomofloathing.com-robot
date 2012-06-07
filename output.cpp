#include "output.h"

Output::Output()
{
}

void Output::attach(QTextEdit * textEdit)
{
    _textEdit = textEdit;
}

void Output::write(const QString & qs)
{
    if (_textEdit)
        _textEdit->append(qs);
}

void Output::emptyLine()
{
    if (_textEdit)
        _textEdit->append(" ");
}

void Output::dumpHeader(QNetworkReply* reply)
{
    typedef const QNetworkReply::RawHeaderPair raw_header_pair;

    if (_textEdit)
    {
    for (int i=0; i<reply->rawHeaderList().count(); i++)
    {
        raw_header_pair p = reply->rawHeaderPairs().at(i);
        _textEdit->append(QString("%1 : %2").arg(QString(p.first)).arg(QString(p.second)));
    }
    emptyLine();
    }
}

void Output::dumpHeader(QNetworkRequest* request)
{
    if (_textEdit)
    {
    for (int i=0; i<request->rawHeaderList().count(); i++)
    {
        QString p = QString(request->rawHeaderList().at(i));
        _textEdit->append(p);
    }
    emptyLine();
    }
}

