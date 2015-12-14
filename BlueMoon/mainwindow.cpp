#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utils.h"

void MainWindow::setStatusBar()
{

#ifndef QT_NO_STATUSBAR
    (void)statusBar();
#endif

}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setStatusBar();

    //test of an old method
    //const char *v = "pwd";
    //statusBar()->showMessage(QString::fromStdString(exec(v)));

    //test of a new method running scripts
    QString script = QString("#! /usr/bin/env bash\n") +
                         QString("pwd\n") +
                         QString("EOF\n");
    statusBar()->showMessage(runScr(script));
}

MainWindow::~MainWindow()
{
    delete ui;
}
