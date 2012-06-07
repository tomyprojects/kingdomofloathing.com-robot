#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QtWebKit>
#include "myfirstrobot.h"
#include "output.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    
private:
    Ui::MainWindow *ui;

    MyFirstRobot robot;
};

#endif // MAINWINDOW_H
