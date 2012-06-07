#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "statuscode.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Output::instance().attach(ui->textEdit);

    robot.start(ui->webView);
}

MainWindow::~MainWindow()
{
    delete ui;
}
