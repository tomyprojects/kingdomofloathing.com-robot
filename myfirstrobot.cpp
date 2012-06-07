#include "myfirstrobot.h"

MyFirstRobot::MyFirstRobot()
{
}

 void MyFirstRobot::start(QWebView * webview)
 {
     KolQRobot::start(webview, "myfirstrobot", "flip167pick83");
 }

void MyFirstRobot::game()
{
    static bool useBuffs = true;
    static bool useActionCounter = false;
    static int actionCounter = 10;
    static int buffCounter = 0;
    static const int manaCost = 100;


    if ((actionCounter--) < 0 && useActionCounter)
    {
        qDebug() << "actionCounter is zero";
        return;
    }

    int hp = getHitPoints();
    int mp = getMojoPoints();
    int adventures = getAdventures();

    Output::instance().write(QString("hp: %1  mp: %2  adventures: %3").arg(hp).arg(mp).arg(adventures));

    if (adventures > 0)
    {
        qDebug() << "Hp: " << hp << " Adventures: " << adventures;

        if (hp == 0)
        {
            //runStep(Step::MEDICINE);
            qDebug() << "No hp";
            exit(0);
        }
        else
        {
            if (isCombat())
            {
                if (canSteal())
                {
                    runStep(Step::STEAL);
                }
                else if (isWin())
                {
                    if (mp > manaCost && useBuffs && buffCounter <= 0)
                    {
                        runStep(Step::BUFF);
                        buffCounter = 20;
                    }
                    else
                    {
                        runStep(Step::ADVENTURE_AGAIN);

                        buffCounter--;
                    }
                }
                else
                {
                    runStep(Step::ATTACK);
                }
            }
            else if (isRandomLackofanEncounter())
            {
                runStep(Step::CHOICE2);

                buffCounter--;
            }
            else
            {
                if (mp > manaCost && useBuffs && buffCounter <= 0)
                {
                    runStep(Step::BUFF);
                    buffCounter = 20;
                }
                else
                {
                    runStep(Step::ADVENTURE_AGAIN);

                    buffCounter--;
                }
            }
        }
    }
    else
    {
        qDebug() << "Exit";
        exit(0);
    }
}
