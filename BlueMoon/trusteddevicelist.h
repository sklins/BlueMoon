#ifndef TRUSTEDDEVICELIST_H
#define TRUSTEDDEVICELIST_H
#include <QString>
#include <QVector>
#include <QBluetoothServiceInfo>

class trusteddevicelist
{
public:
    trusteddevicelist();

private:
    QString fileName;
    QString fileDirectory;
    QVector<QVector<QString> > trustedDevices;

public:
    void setFileName(QString filename);
    QString getFileName();

    void setFileDirectory(QString filedirectory);
    QString getFileDirectory();

    void setTrusteddevices(QVector<QVector<QString> > tD);
    QVector<QVector<QString> > getTrustedDevices();

    void addToTrustList(const QBluetoothServiceInfo& serviceInfo);
    void deleteFromTrustList(QString macAddress);

    bool isTrusted(QString macAddress);

public:
    void readTrustedDeviceList();
    void writeToTrustedDeviceList();

};

#endif // TRUSTEDDEVICELIST_H
