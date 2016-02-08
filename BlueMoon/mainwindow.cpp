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
    //QString script = QString("#! /usr/bin/env bash\n") +
    //                   QString("pwd\n") +
    //                   QString("EOF\n");
    //statusBar()->showMessage(runScr(script));

    this->discoveryAgent.reset(new QBluetoothDeviceDiscoveryAgent);
    this->localDevice.reset(new QBluetoothLocalDevice);

    connect(this->ui->toolButton_2, SIGNAL(clicked()), this, SLOT(startScan()));
    connect(this->discoveryAgent.data(), SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)), this, SLOT(addDevice(QBluetoothDeviceInfo)));
    connect(this->discoveryAgent.data(), SIGNAL(finished()), this, SLOT(scanFinished()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startScan() {
    this->discoveryAgent->start();
    ui->toolButton_2->setEnabled(false);
}

void MainWindow::addDevice(const QBluetoothDeviceInfo &info) {
    QString label = QString("%1 %2").arg(info.address().toString(), info.name());
    QList<QListWidgetItem*> items = ui->listWidget->findItems(label, Qt::MatchExactly);
    if (items.empty()) {
        QListWidgetItem *item = new QListWidgetItem(label);

        QBluetoothLocalDevice::Pairing pairingStatus = localDevice->pairingStatus(info.address());
        if (pairingStatus == QBluetoothLocalDevice::Paired || pairingStatus == QBluetoothLocalDevice::AuthorizedPaired )
            item->setTextColor(QColor(Qt::green));
        else
            item->setTextColor(QColor(Qt::black));

        ui->listWidget->addItem(item);
    }
}

void MainWindow::scanFinished() {
    ui->toolButton_2->setEnabled(true);
}
