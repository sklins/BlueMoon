/* trusteddevicelist.cpp
* Bluemoon - a Bluetooth manager designed for LXQt desktop environment
* https://github.com/sklins/BlueMoon
*
* Copyright: 2016, Bluemoon Team
* Authors:
* Elina Gilmanova <linkegilmanova@gmail.com>
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
    if (!trusteddevicelist::isTrusted(deviceInfo.address().toString()) & deviceInfo.address().toString()!="00:00:00:00:00:00")
    {
        device.prepend(deviceInfo.address().toString());
        device.prepend(deviceInfo.name());
        device.prepend(serviceInfo.serviceName());

        trustedDevices.prepend(device);
    }
}
void trusteddevicelist::deleteFromTrustList(QString macAddress)//const QBluetoothServiceInfo& serviceInfo)
{
    //QString address=serviceInfo.device().address().toString();
    for(int i=0;i<trustedDevices.length();i++)
    {
        if(trustedDevices[i][2]==macAddress)
            trustedDevices.remove(i);
    }
}
