#include "trusteddevicelist.h"
#include <QDataStream>
#include <QFile>
#include <QIODevice>
#include <QBluetoothServiceInfo>
#include <QBluetoothDeviceInfo>

trusteddevicelist::trusteddevicelist()
{

}

//Read Write
void trusteddevicelist::readTrustedDeviceList()
{
    QString filename=trusteddevicelist::getFileName();
    QString filedir= trusteddevicelist::getFileDirectory();

    filename = filedir+filename;
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
            return;
    QVector<QVector<QString> > tdlist;
    QDataStream in(&file);
    QVector<QString> deviceInfo;
    while(!in.atEnd())
    {
        in>>deviceInfo;
        tdlist.prepend(deviceInfo);
    }
    file.close();
    trusteddevicelist::setTrusteddevices(tdlist);
}
void trusteddevicelist::writeToTrustedDeviceList()
{
    QString filename=trusteddevicelist::getFileName();
    QString filedir= trusteddevicelist::getFileDirectory();

    filename = filedir+filename;
    QFile file(filename);
    //QDataStream in(&file, QIODevice::Truncate);
    if (!file.open(QIODevice::WriteOnly | QFile::Truncate))
            return;
    QDataStream out(&file);
    for (int i=0;i<trusteddevicelist::trustedDevices.length();i++)
        out<<trusteddevicelist::trustedDevices[i];
    file.close();
}

// Getter Setter
QString trusteddevicelist::getFileDirectory(){
    return fileDirectory;
}
void trusteddevicelist::setFileDirectory(QString filedir){
    fileDirectory=filedir;
}

QString trusteddevicelist::getFileName(){
    return fileName;
}
void trusteddevicelist::setFileName(QString filename){
    fileName=filename;
}

QVector<QVector<QString> > trusteddevicelist::getTrustedDevices()
{
    return trustedDevices;
}

void trusteddevicelist::setTrusteddevices(QVector<QVector<QString> > tD)
{
    trustedDevices=tD;
}

//TODO: Elina
bool trusteddevicelist::isTrusted(QString macAddress)
{
    bool isTrusted=false;
    for(int i=0;i<trustedDevices.length();i++)
    {
        if(trustedDevices[i][2]==macAddress)
        {
            isTrusted=true;
            break;
        }
    }
    return isTrusted;
}

void trusteddevicelist::addToTrustList(const QBluetoothServiceInfo& serviceInfo)
{
    QVector<QString> device;
    QBluetoothDeviceInfo deviceInfo=serviceInfo.device();

    if (!trusteddevicelist::isTrusted(deviceInfo.address().toString()))
    {
        device.prepend(deviceInfo.address().toString());
        device.prepend(deviceInfo.name());
        device.prepend(serviceInfo.serviceDescription());

        trustedDevices.prepend(device);
    }

}
void trusteddevicelist::deleteFromTrustList(QString macAddress)
{
    for(int i=0;i<trustedDevices.length();i++)
    {
        if(trustedDevices[i][2]==macAddress)
            trustedDevices.remove(i);
    }

}
