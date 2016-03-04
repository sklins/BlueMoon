/* trusteddevicelist.h
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
    void deleteFromTrustList(QString macAddress);//const QBluetoothServiceInfo& serviceInfo);

    bool isTrusted(QString macAddress);

public:
    void readTrustedDeviceList();
    void writeToTrustedDeviceList();

};

#endif // TRUSTEDDEVICELIST_H
