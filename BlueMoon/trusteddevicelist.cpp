#include "trusteddevicelist.h"
#include <QDataStream>
#include <QFile>
#include <QIODevice>
#include <QBluetoothServiceInfo>

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
    QVector<QString> tdlist=trusteddevicelist::getTrustedDevices();
    QDataStream in(&file);
    QString deviceInfo;
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
    if (!file.open(QIODevice::WriteOnly))
            return;
    QDataStream out(&file);
    out<<QString("HTC DESIRE 616");
    out<<QString("HP PAVILION g1349");
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

QVector<QString> trusteddevicelist::getTrustedDevices()
{
    return trustedDevices;
}

void trusteddevicelist::setTrusteddevices(QVector<QString> tD)
{
    trustedDevices=tD;
}

//TODO: Elina
bool trusteddevicelist::isTrusted(QString macAddress)
{
    return false;
}

/*void trusteddevicelist::addToTrustList(const QBluetoothServiceInfo& serviceInfo)
{
    //QString device = serviceInfo.device().name().toString();
    //device+=serviceInfo.serviceName().toString();
    //device+=serviceInfo.address().toString();
    //trustedDevices.prepend(deviceInfo);
}
void trusteddevicelist::deleteFromTrustList(const QBluetoothServiceInfo& serviceInfo)
{}*/
