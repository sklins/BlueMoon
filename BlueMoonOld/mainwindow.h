#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore>
#include <QtBluetooth>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
    //createTransfer(DeviceInfo); - Sonya
    //createPassword
    
    //InfoAboutDevice?
    
    //TurnOnOffBluetooth
    //VisibleIbvisible 
    
    //CreateTrayIcon - Tina
    //TraylIconWindow OnOff
    
    //createPAN
    
    //loadLocalList - Elina
    //addtoLocalList
    //deleteFromLocalList

private slots:
    void startScan();
    void addDevice(const QBluetoothDeviceInfo&);
    void scanFinished();

private:
    QScopedPointer<QBluetoothDeviceDiscoveryAgent> discoveryAgent;
    QScopedPointer<QBluetoothLocalDevice> localDevice;
    Ui::MainWindow *ui;
    void setStatusBar();
};

#endif // MAINWINDOW_H
