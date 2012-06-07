#include "kolqrobot.h"

KolQRobot::KolQRobot()
{
    addCssPath("Login", "html body center table tbody tr td table tbody tr td center table tbody tr td table form", NAME);
    addCssPath("loggingin", "body center table tbody tr td table tbody tr td center table tbody tr td table input", NAME);
    addCssPath("loginname", "body center table tbody tr td table tbody tr td center table tbody tr td table tbody tr td input", NAME);
    addCssPath("password", "body center table tbody tr td table tbody tr td center table tbody tr td table tbody tr td input", NAME);
    addCssPath("secure","body center table tbody tr td table tbody tr td center table tbody tr td table tbody input", NAME);
    addCssPath("challenge", "html body center table tbody tr td table tbody tr td center table tbody tr td table tbody input", NAME);
    addCssPath("response","body center table tbody tr td table tbody tr td center table tbody tr td table tbody input", NAME);
    addCssPath("submitbutton", "body center table tbody tr td table tbody tr td center table tbody tr td table tbody tr td center input", NAME);

    addCssPath("Hit Points","body center table tbody tr td img",TITLE);
    addCssPath("Mojo Points","body center table tbody tr td img",TITLE);
    addCssPath("Adventures Remaining", "html body center table tbody tr td img", TITLE);

    addCssPath("Last Adventure", "body center center table tbody tr td font a[target=\"mainpane\"]", NONE);

    addCssPath("pwd", "body center table tbody tr td center table tbody tr td center table tbody tr td form input", NAME);

    addCssPath("body", "body", NONE);

    addCssPath("buffform","html body center center table tbody tr td center table tbody tr td center form", NAME);

    addCssPath("choiceform3", "html body center center table tbody tr td center table tbody tr td center form", NAME);
    addCssPath("choiceform2", "html body center center table tbody tr td center table tbody tr td center form", NAME);
}

void KolQRobot::start(QWebView * webview,
                    const QString & name,
                    const QString & password)
{
    _loginname = name;
    _password = password;
    attach(webview);
    httpConnect("http://www.kingdomofloathing.com");
}

void KolQRobot::login()
{
    QWebElement name = getWebElement("loginname");
    QWebElement password = getWebElement("password");
    QWebElement submit = getWebElement("submitbutton");
    QWebElement form = getWebElement("Login");

    name.setAttribute("value", _loginname);
    password.setAttribute("value", _password);
    submit.evaluateJavaScript("this.click()");

    StringQueue keys;
    keys.enqueue("loggingin");
    keys.enqueue("loginname");
    keys.enqueue("password");
    keys.enqueue("secure");
    keys.enqueue("challenge");
    keys.enqueue("response");

    post(QUrl(form.attribute("action")), keys);
 }

void KolQRobot::game()
{
}

int KolQRobot::getHitPoints()
{
    QWebElement img = getWebElement("Hit Points", "charpane");
    QWebElement span = img.nextSibling().nextSibling();
    QString str = span.toPlainText();
    QTextStream textStream(&str);

    int hp;
    textStream >> hp;
    return hp;
}

int KolQRobot::getMojoPoints()
{
    QWebElement img = getWebElement("Mojo Points", "charpane");
    QWebElement span = img.nextSibling().nextSibling();
    QString str = span.toPlainText();
    QTextStream textStream(&str);

    int mp;
    textStream >> mp;
    return mp;
}

int KolQRobot::getAdventures()
{
    QWebElement img = getWebElement("Adventures Remaining", "charpane");
    QWebElement span = img.nextSibling().nextSibling();
    QString str = span.toPlainText();
    QTextStream textStream(&str);

    int a;
    textStream >> a;
    return a;
}

bool KolQRobot::isCombat()
{
    DomTraversal dt;

    QWebElement we = dt.findPlainText(getWebElement("body", "mainpane"), "Combat!");

    return !we.isNull();
}

bool KolQRobot::isWin()
{
    DomTraversal dt;

    QWebElement we = dt.findPlainText(getWebElement("body", "mainpane"), "You win the fight!");

    return !we.isNull();
}

bool KolQRobot::isWheelsInTheSky()
{
    DomTraversal dt;

    QWebElement we = dt.findPlainText(getWebElement("body", "mainpane"), "Wheel in the Clouds in the Sky, Keep On Turning");

    return !we.isNull();
}

bool KolQRobot::isRandomLackofanEncounter()
{
    DomTraversal dt;

    QWebElement we = dt.findPlainText(getWebElement("body", "mainpane"), "Random Lack of an Encounter");

    return !we.isNull();
}

bool KolQRobot::canSteal()
{
    DomTraversal dt;

    DomTraversal::StringSet set;

    set.insert("You get the jump on him.");
    set.insert("You get the jump on them.");
    set.insert("You get the jump on her.");

    QWebElement e = dt.findPlainText(getWebElement("body", "mainpane"), set);

    return !e.isNull();
}

void KolQRobot::attack()
{
    StringMap keys;
    keys.insert("action", "attack");
    post(QUrl("/fight.php"), keys, "mainpane");
}

void KolQRobot::steal()
{
    StringMap keys;
    keys.insert("action", "steal");
    post(QUrl("/fight.php"), keys, "mainpane");
}

void KolQRobot::selectChoice2()
{
    StringMap keys;

    DomTraversal dt;

    QWebElement e = dt.findPlainText(getWebElement("body", "mainpane"), "Which area will it be?");

    e = e.nextSibling();

    QWebElement form = dt.findName(e, "choiceform2");

    e = form.firstChild();

    while (!e.isNull())
    {
        if (e.attribute("name") != "")
            keys.insert(e.attribute("name"), e.attribute("value"));

        e = e.nextSibling();
    }

    post(QUrl("/choice.php"), keys, "mainpane");
}

void KolQRobot::gotoMainMap()
{
    get(QUrl("/main.php"), "mainpane");
}

void KolQRobot::gotoCamp()
{
    get(QUrl("/campground.php"), "mainpane");
}

void KolQRobot::gotoSkillsPage()
{
    get(QUrl("/skills.php"), "mainpane");
}

void KolQRobot::buyMedicine(const QString & quantity)
{
    QWebElement e = getWebElement("pwd", "mainpane");

    StringMap keys;

    keys.insert("pwd", e.attribute("value"));
    keys.insert("action", "curehp");
    keys.insert("quantity", quantity);

    post(QUrl("/galaktik.php"), keys, "mainpane");
}

void KolQRobot::useSkill(const QString & skill, const QString & quantity)
{
    QWebElement form = getWebElement("buffform", "mainpane");

    StringMap keys;

    QWebElement e = form.firstChild();

    keys.insert(e.attribute("name"), e.attribute("value"));

    e = e.nextSibling();

    keys.insert(e.attribute("name"), e.attribute("value"));

    e = e.nextSibling();

    DomTraversal dt;

    QWebElement option = dt.findPlainText(e, skill);

    keys.insert(e.attribute("name"), option.attribute("value"));

    e = e.nextSibling();

    keys.insert("specificplayer", "");

    QWebElement yourself = dt.findPlainText(e, "(yourself)");

    keys.insert("targetplayer", yourself.attribute("value"));
    keys.insert("bufftimes", quantity);

    post(QUrl("/skills.php"), keys, "mainpane");
}

void KolQRobot::gotoMedicineShow()
{
    get(QUrl("/galaktik.php"), "mainpane");
}

void KolQRobot::lastAdventure()
{
    QWebElement a = getWebElement("Last Adventure", "charpane");
    get(QUrl(a.attribute("href")), "mainpane");
}

void KolQRobot::restInTent()
{
    get(QUrl("/campground.php?action=rest"), "mainpane");
}

void KolQRobot::updateCharpane()
{
    get(QUrl("/charpane.php"), "charpane");
}

void KolQRobot::runStep(Step::STEP s)
{
    _step.go(s);
    QTimer::singleShot(2000, this, SLOT(executeState()));

    switch (_step.state())
    {
    case Step::ADVENTURE_AGAIN:
        Output::instance().write("Adventure again");
        qDebug() << "Adventure again";
        break;

    case Step::ATTACK:
        Output::instance().write("Attack");
        qDebug() << "Attack";
        break;

    case Step::MEDICINE:
        Output::instance().write("Buying medicine");
        qDebug() << "Buying medicine";
        break;

    case Step::BUFF:
        Output::instance().write("Buffing");
        qDebug() << "Buffing";
        break;

    case Step::STEAL:
        Output::instance().write("Stealing");
        qDebug() << "Stealing";
        break;

    default:
        qDebug() << "running state " << _step.state();
        break;
    }
}

void KolQRobot::executeState()
{
    switch (_step.state())
    {
    case Step::WAIT:
        break;

    case Step::LOGIN:
        login();
        _step.next();
        break;

    case Step::SUBMIT:
        _step.next();
        break;

    case Step::IN_GAME: // 2
        game();
        break;

    case Step::UPDATE_CHARACTER_PANEL: // 3
        updateCharpane();
        _step.go(Step::IN_GAME);
        break;

    case Step::HEAL_1:
        gotoCamp();
        _step.next();
        break;

    case Step::HEAL_2:
        restInTent();
        _step.go(Step::UPDATE_CHARACTER_PANEL);
        break;

    case Step::ADVENTURE_AGAIN: // 6
        lastAdventure();
        _step.go(Step::UPDATE_CHARACTER_PANEL);
        break;

    case Step::ATTACK: // 7
        attack();
        _step.go(Step::UPDATE_CHARACTER_PANEL);
        break;

    case Step::MEDICINE: // 8
        gotoMedicineShow();
        _step.next();
        break;

    case Step::MEDICINE_2:
        buyMedicine("1");
        _step.go(Step::UPDATE_CHARACTER_PANEL);
        break;

    case Step::STEAL:
        steal();
        _step.go(Step::UPDATE_CHARACTER_PANEL);
        break;

    case Step::BUFF:
        gotoSkillsPage();
        _step.next();
        break;

    case Step::BUFF_1:
        useSkill("The Polka of Plenty (7 MP)", "1");
        _step.next();
        break;

    case Step::BUFF_2:
        useSkill("Fat Leon's Phat Loot Lyric (11 MP)", "1");
        _step.next();
        break;

    case Step::BUFF_3:
        useSkill("Aloysius' Antiphon of Aptitude (40 MP)", "1");
        _step.go(Step::UPDATE_CHARACTER_PANEL);
        break;

    case Step::BUFF_4:
        //useSkill("The Polka of Plenty (7 MP)", "1");
        //_step.go(Step::UPDATE_CHARACTER_PANEL);
        break;

    case Step::CHOICE2:
        selectChoice2();
        _step.go(Step::UPDATE_CHARACTER_PANEL);
        break;

    default:
        break;
    }
}

