/* progress.cpp
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

#include "progress.h"
#include <QtWidgets>

Progress::Progress(QWidget* parent, const QBluetoothServiceInfo& service)
    : QDialog(parent)
    , service_(service)
{
    ui.reset(new Ui::Progress);

    ui->setupUi(this);
    ui->progressBar->setRange(0, 1);

    connect(ui->cancelButton, &QPushButton::clicked, this, &Progress::close);
    connect(ui->chooseFileButton, &QPushButton::clicked, this, &Progress::chooseFile);
    connect(ui->transferButton, &QPushButton::clicked, this, &Progress::startTransfer);
}

void Progress::finished(QBluetoothTransferReply *reply) {
    if (reply->error() != QBluetoothTransferReply::NoError) {
        ui->progressBar->setDisabled(true);
        ui->statusLabel->setText(tr("Failed: %1").arg(reply->errorString()));
    } else {
        ui->statusLabel->setText(tr("Transfer complete"));
    }

    ui->cancelButton->setText(tr("Dismiss"));
}

void Progress::uploadProgress(qint64 bytesSent, qint64 bytesTotal) {
    if (bytesSent == 0){
        start.start();
    }

    ui->progressBar->setMaximum(bytesTotal);
    ui->progressBar->setValue(bytesSent);

    if (bytesSent && bytesTotal &&
            (start.elapsed() > 1000) &&
            (bytesSent > start.elapsed()/1000)) {

        ui->statusLabel->setText(tr("Transferring... ETA: %1s")
                .arg(((bytesTotal-bytesSent)/(bytesSent/(start.elapsed()/1000)))));
    }
}

void Progress::chooseFile() {
    QString fileName = QFileDialog::getOpenFileName();
    ui->fileNameLineEdit->setText(fileName);
    if (service_.isValid()) {
        ui->transferButton->setDisabled(false);
    } else {
        ui->statusLabel->setText(QString("Invalid service: %1").arg(service_.device().name()));
    }
}

void Progress::startTransfer() {
    mgr_.reset(new QBluetoothTransferManager);
    req_.reset(new QBluetoothTransferRequest(service_.device().address()));

    file_.reset(new QFile(ui->fileNameLineEdit->text()));

    ui->statusLabel->setText("Sending to: " + service_.device().name() + ", waiting for start");

    reply_.reset(mgr_->put(*req_, file_.data()));
    reply_->setParent(this);
    if (reply_->error()){
        qDebug() << "Failed to send file";
        finished(reply_.data());
        return;
    }

    connect(reply_.data(), &QBluetoothTransferReply::transferProgress, this, &Progress::uploadProgress);
    connect(reply_.data(), &QBluetoothTransferReply::finished, this, &Progress::finished);
    connect(this, &Progress::rejected, reply_.data(), &QBluetoothTransferReply::abort);
}

