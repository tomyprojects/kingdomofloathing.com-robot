#ifndef QRobot_H
#define QRobot_H

#include <QMainWindow>
#include <QtNetwork>
#include <QtWebKit>
#include <QTimer>
#include <QTextEdit>
#include <QtDebug>
#include "step.h"
#include "output.h"

class QRobot : public QObject
{
    Q_OBJECT

public:
    typedef QQueue<QWebFrame *> WebFrameQueue;
    typedef QString CSSPath;
    typedef QMap<QString, CSSPath> PathMap;
    typedef QMap<QString, QString> StringMap;
    typedef QQueue<QString> StringQueue;
    typedef QMap<QString, QWebFrame*> FrameMap;

    explicit QRobot(QObject *parent = 0);
    ~QRobot();

    void attach(QWebView * webview);

    virtual void httpConnect(const QString &);

protected slots:
    virtual void executeState();

protected:

    enum KeyAttribute {NONE, NAME, CLASS, TITLE};

    QWebView* getWebView();
    QNetworkAccessManager* getNetworkAccessManager();
    QUrl getUrl();
    QWebElement getWebElement(const QString & key);
    QWebElement getWebElement(const QString & key, const QString & frameName);
    QString getCssPath(const QString & key);

    void addCssPath(const QString & key, const QString & path, KeyAttribute attribute = NONE);
    void post(const QUrl & relative, StringQueue & keys);
    void post(const QUrl & relative, StringMap & keys, const QString & frameName);
    void get(const QUrl & relative, const QString & frameName);

private:
    QWebView * _webview;
    QNetworkAccessManager * _nam;
    QNetworkRequest _postRequest;
    QNetworkRequest _getRequest;
    WebFrameQueue _framequeue;
    FrameMap _framemap;
    PathMap _csspaths;
    QUrl _url;

    bool _ready;

    void loadContents(QNetworkReply*);
    void loadFrames();
    void getFrame(QWebFrame *);
    void setUrl(const QUrl & url);
    QWebFrame * findFrame(const QString & frameName);
    void findFrame(const QString & frameName, QWebFrame* view);
private slots:
    void execute();
    void httpReplyFinished();
    void replyFinished();
    void webviewLoadFinished();
};

#endif // QRobot_H
