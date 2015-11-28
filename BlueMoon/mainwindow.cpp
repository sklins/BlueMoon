#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setStatusBar();
    statusBar()->showMessage("test status");
}

void MainWindow::setStatusBar()
{

#ifndef QT_NO_STATUSBAR
    ( void )statusBar();
#endif

}

MainWindow::~MainWindow()
{
    delete ui;
}
