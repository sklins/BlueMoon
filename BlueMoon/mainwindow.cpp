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

    //test
    const char *v = "pwd";
    statusBar()->showMessage(QString::fromStdString(exec(v)));
}

MainWindow::~MainWindow()
{
    delete ui;
}
