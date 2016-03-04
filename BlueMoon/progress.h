/* progress.h
* Bluemoon - a Bluetooth manager designed for LXQt desktop environment
* https://github.com/sklins/BlueMoon
*
* Copyright: 2016, Bluemoon Team
* Authors:
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

#include "ui_progress.h"

#include <QtBluetooth>

#include <QDialog>
#include <QTime>

class Progress
    : public QDialog
{
    Q_OBJECT

public:
    explicit Progress(QWidget*, const QBluetoothServiceInfo&);

public slots:
    void finished(QBluetoothTransferReply*);
    void uploadProgress(qint64 bytesSent, qint64 bytesTotal);

private slots:
    void chooseFile();
    void startTransfer();

private:
    QScopedPointer<Ui::Progress> ui;
    QTime start;
    QBluetoothServiceInfo service_;
    QScopedPointer<QBluetoothTransferManager> mgr_;
    QScopedPointer<QBluetoothTransferRequest> req_;
    QScopedPointer<QBluetoothTransferReply> reply_;
    QScopedPointer<QFile> file_;
};
