#ifndef KOLQROBOT_H
#define KOLQROBOT_H

#include "qrobot.h"
#include "httprequest.h"
#include "domtraversal.h"

class KolQRobot : private QRobot
{
public:

    KolQRobot();
    void start( QWebView * webview,
             const QString & name,
             const QString & password);

private:
    QString _loginname;
    QString _password;
    QNetworkRequest _request;
    Step _step;



protected slots:
    void login();
    virtual void game();

    int getHitPoints();
    int getMojoPoints();
    int getAdventures();

    bool isCombat();
    bool isWin();
    bool canSteal();
    bool isWheelsInTheSky();
    bool isRandomLackofanEncounter();

    void attack();
    void steal();

    void selectChoice2();

    void gotoMainMap();
    void gotoCamp();
    void gotoMedicineShow();
    void gotoSkillsPage();

    void buyMedicine(const QString & quantity);
    void useSkill(const QString & skill, const QString & quantity);

    void lastAdventure();
    void restInTent();
    void updateCharpane();

    void runStep(Step::STEP s);

private slots:
    void executeState();
};

#endif // KOLQROBOT_H
