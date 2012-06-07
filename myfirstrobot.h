#ifndef MYFIRSTROBOT_H
#define MYFIRSTROBOT_H

#include "kolqrobot.h"

class MyFirstRobot : private KolQRobot
{
public:
    MyFirstRobot();

    void start(QWebView * webview);

protected slots:

    void game();
};

#endif // MYFIRSTROBOT_H
