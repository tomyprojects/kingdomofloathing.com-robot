#ifndef OUTPUT_H
#define OUTPUT_H

#include <QTextEdit>
#include <QtNetwork>

class Output
{
public:

    static Output& instance()
    {
        static Output output;
        return output;
    }

    void attach(QTextEdit * textEdit);

    void write(const QString & qs);

    void emptyLine();

    void dumpHeader(QNetworkReply*);

    void dumpHeader(QNetworkRequest*);

private:
    Output();

    QTextEdit * _textEdit;
};

#endif // OUTPUT_H
