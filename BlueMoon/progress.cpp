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

