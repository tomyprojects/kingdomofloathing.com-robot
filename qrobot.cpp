#include "qrobot.h"
#include "statuscode.h"
#include "httprequest.h"

QRobot::QRobot(QObject *parent) :
    QObject(parent)
{
    _nam = new QNetworkAccessManager(this);

    _postRequest.setRawHeader("Content-Type", "application/x-www-form-urlencoded");

    _ready = true;
}

QRobot::~QRobot()
{
    _nam->deleteLater();
}

void QRobot::attach(QWebView * webview)
{
    _webview = webview;

    QObject::connect(_webview, SIGNAL(loadFinished(bool)), this, SLOT(webviewLoadFinished()));
}

void QRobot::httpConnect(const QString & link)
{
    setUrl(QUrl(link));
    QNetworkReply* reply = _nam->get(_getRequest);
    connect(reply, SIGNAL(finished()), this, SLOT(httpReplyFinished()));
}

void QRobot::executeState()
{
}

QWebView* QRobot::getWebView()
{
    return _webview;
}

QNetworkAccessManager* QRobot::getNetworkAccessManager()
{
    return _nam;
}

QUrl QRobot::getUrl()
{
    return _url;
}

QWebElement QRobot::getWebElement(const QString & key)
{   
    QWebElement e = _webview->page()->mainFrame()->findFirstElement(_csspaths.value(key));
    if (e.isNull())
        qFatal("getWebElement: bad key '%s'", qPrintable(key));
    return e;
}

QWebElement QRobot::getWebElement(const QString & key, const QString & frameName)
{
    QWebFrame* frame = findFrame(frameName);
    QWebElement e = frame->findFirstElement(_csspaths.value(key));
    if (e.isNull())
        qFatal("getWebElement: bad key '%s' '%s'", qPrintable(key), qPrintable(frameName));
    return e;
}

QString QRobot::getCssPath(const QString & key)
{
    if (!_csspaths.contains(key))
        qFatal("getCssPath: no key '%s'", qPrintable(key));

    return _csspaths.value(key);
}

void QRobot::addCssPath(const QString & key, const QString & path, KeyAttribute attribute)
{
    switch (attribute)
    {
    case NONE:
        _csspaths.insert(key, QString("%1").arg(path));
        break;
    case NAME:
        _csspaths.insert(key, QString("%1[name=\"%2\"]").arg(path).arg(key));
        break;

    case TITLE:
        _csspaths.insert(key, QString("%1[title=\"%2\"]").arg(path).arg(key));
        break;

    default:
        break;

    }
}

void QRobot::post(const QUrl & relative, StringQueue & keys)
{
    QByteArray query = http_request::createPostData(_webview, _csspaths, keys);

    QUrl url = _postRequest.url().resolved(relative);
    _postRequest.setUrl(url);
    QNetworkReply * reply = _nam->post(_postRequest, query);
    connect(reply, SIGNAL(finished()), this, SLOT(httpReplyFinished()));
}

void QRobot::post(const QUrl & relative, StringMap & keys, const QString & frameName)
{
    QByteArray query = http_request::createPostData(keys);

    QUrl url = _postRequest.url().resolved(relative);
    _postRequest.setUrl(url);
    QNetworkReply * reply = _nam->post(_postRequest, query);
    reply->setProperty("frameName", QVariant(frameName));
    connect(reply, SIGNAL(finished()), this, SLOT(replyFinished()));
}

void QRobot::get(const QUrl & relative, const QString & frameName)
{
    QUrl url = _getRequest.url().resolved(relative);
    _getRequest.setUrl(url);
    QNetworkReply* reply = _nam->get(_getRequest);
    reply->setProperty("frameName", QVariant(frameName));
    connect(reply, SIGNAL(finished()), this, SLOT(replyFinished()));
}

void QRobot::loadContents(QNetworkReply* reply)
{
    if (_framequeue.empty())
    {
        _webview->setContent(reply->readAll());
        loadFrames();
    }
    else
    {
        QWebFrame* frame = _framequeue.dequeue();
        frame->setContent(reply->readAll());

        if (!_framequeue.empty())
        {
            getFrame(_framequeue.first());
        }
    }
}

void QRobot::loadFrames()
{
    typedef QList<QWebFrame *> WebFrameList;
    typedef QListIterator<QWebFrame *> Iterator;

    QWebFrame* mainframe = _webview->page()->mainFrame();
    WebFrameList webframes = mainframe->childFrames();

    Iterator i(webframes);
    while (i.hasNext())
    {
        QWebFrame* frame = i.next();
        _framequeue.enqueue(frame);
    }

    if (!_framequeue.empty())
    {
        getFrame(_framequeue.first());
    }
}

void QRobot::getFrame(QWebFrame * frame)
{
    _getRequest.setUrl(_getRequest.url().resolved(frame->requestedUrl()));
    QNetworkReply * reply = _nam->get(_getRequest);
    connect(reply, SIGNAL(finished()), this, SLOT(httpReplyFinished()));
}

void QRobot::httpReplyFinished()
{
    QNetworkReply * reply = qobject_cast<QNetworkReply*>(sender());

    int statusCode = status_code::httpStatusCode(reply);

    switch (statusCode)
    {
    case 200:
        qDebug() << "initial content load";
        loadContents(reply);
        break;

    case 302:
    {
        qDebug() << "intial redirection";
        QUrl url = status_code::redirect(getUrl(), reply);
        httpConnect(url.toString());
        break;
    }

    default:
        break;
    }

    reply->deleteLater();
}

void QRobot::replyFinished()
{
    QNetworkReply * reply = qobject_cast<QNetworkReply*>(sender());

    int statusCode = status_code::httpStatusCode(reply);

    if (reply)
    {
        if (reply->error() == QNetworkReply::NoError)
        {
            switch (statusCode)
            {
            case 200:
            {
                QString frameName = reply->property("frameName").toString();
                QWebFrame* frame = findFrame(frameName);
                qDebug() << "update frame: " << frame->frameName();
                frame->setContent(reply->readAll());
                break;
            }

            case 302:
            {
                qDebug() << "302 redirection";
                QUrl url = status_code::redirect(getUrl(), reply);
                get(url, reply->property("frameName").toString());
                break;
            }

            default:
                break;
            }
        }
    }

    reply->deleteLater();
}

void QRobot::setUrl(const QUrl & url)
{
    _getRequest.setUrl(QUrl(url));
    _postRequest.setUrl(QUrl(url));
    _url = QUrl(url);
}


void QRobot::findFrame(const QString & frameName, QWebFrame* view)
{
    typedef QList<QWebFrame*> FrameList;
    typedef QListIterator<QWebFrame*> FrameIterator;

    FrameList frames = view->childFrames();

    if (frames.empty())
        return;

    for (int i=0; i<frames.count(); i++)
    {
        QWebFrame * frame = frames.at(i);

        if (! _framemap.contains(frame->frameName()) )
        {
            qDebug() << frame->frameName();
            _framemap.insert(frame->frameName(), frame);
        }
    }
}

QWebFrame* QRobot::findFrame(const QString & frameName)
{
    if (!_framemap.contains(frameName))
    {
        qDebug() << "Initializing path to all child frames...child count: " << _webview->page()->mainFrame()->childFrames().count();
        findFrame(frameName, _webview->page()->mainFrame());
    }

    if (! _framemap.contains(frameName))
    {
        qFatal("findFrame: can't find '%s'", qPrintable(frameName));
    }

    return _framemap.value(frameName);
}

void QRobot::execute()
{
    executeState();
    _ready = true;
}

void QRobot::webviewLoadFinished()
{
    if (_framequeue.empty())
    {
        if (_ready)
        {
            _ready = false;
            QTimer::singleShot(500, this, SLOT(execute()));
        }
    }
}
