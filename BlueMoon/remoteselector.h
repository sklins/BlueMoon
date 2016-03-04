/* remoteselector.h
* Bluemoon - a Bluetooth manager designed for LXQt desktop environment
* https://github.com/sklins/BlueMoon
*
* Copyright: 2016, Bluemoon Team
* Authors:
* Sofiia Sandomirskaya <ssandomirskaya@gmail.com>
* Elina Gilmanova <linkegilmanova@gmail.com>
* Denis Pavlov <dspavlov_1@edu.hse.ru>
* Linh Chi Tran <lc.tinatran@gmail.com>
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published
* by the Free Software Foundation; either version 2.1 of the License,
* or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General
* Public License along with this library; if not, write to the
* Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
* Boston, MA 02110-1301 USA
*/

#pragma once

#include "ui_remoteselector.h"

#include <QtCore>
#include <QtBluetooth>
#include <QtWidgets>

#include "pindialog.h"
#include "trusteddevicelist.h"

class RemoteSelector
    : public QWidget
{
    Q_OBJECT

public:
    explicit RemoteSelector(QWidget* = 0);

    bool turnOnOff=false;
    bool visibility=false;

    void startDiscovery(const QBluetoothUuid& uuid);
    QBluetoothServiceInfo service() const;

    /*
     * Devicelist settings
     */
    void showtrustedDeviceList(trusteddevicelist tdl);

    void changeEvent(QEvent *event);

private:
    QScopedPointer<Ui::RemoteSelector> ui;
    QScopedPointer<QBluetoothLocalDevice> localDevice_;
    QScopedPointer<QBluetoothServiceDiscoveryAgent> discoveryAgent_;

    QBluetoothServiceInfo service_;
    QMap<int, QBluetoothServiceInfo> discoveredServices_;
    QScopedPointer<PinDialog> pinDialog_;
    bool pairingErrorFlag_;

    QString addressToName(const QBluetoothAddress& address);

    trusteddevicelist trustedDevicelist;

    // Tray Icon
    void createActions();
    void createTrayIcon();

    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *bluetoothOnAction;
    QAction *bluetoothOffAction;
    QAction *quitAction;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

public Q_SLOTS:
    void startDiscovery();
signals:
    //void tableSectionChecked();


private slots:
    void serviceDiscovered(const QBluetoothServiceInfo &serviceInfo);
    void discoveryFinished();
    void on_refreshPB_clicked();
    void on_stopButton_clicked();
    void sendFileButton_clicked();

    void pairingFinished(const QBluetoothAddress &address,QBluetoothLocalDevice::Pairing pairing);
    void pairingError(QBluetoothLocalDevice::Error error);
    void displayPin(const QBluetoothAddress &address, QString pin);
    void displayConfirmation(const QBluetoothAddress &address, QString pin);
    void displayConfReject();
    void displayConfAccepted();

    void on_remoteDevices_cellClicked(int row, int column);
    void on_remoteDevices_itemChanged(QTableWidgetItem* item);

    //void rowSelection
    void changeHostMode();
    void powerOnOff();
    void visibilityOnOff();
    //
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void bluetoothOn();
    void bluetoothOff();

};
