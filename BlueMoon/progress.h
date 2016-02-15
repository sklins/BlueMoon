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
