#include "remoteselector.h"

#include "progress.h"

RemoteSelector::RemoteSelector(QWidget* parent)
    : QWidget(parent)
    , pairingErrorFlag_(false)
{
    ui.reset(new Ui::RemoteSelector);
    ui->setupUi(this);

    //Stuff with local trusted deviceList
    trustedDevicelist =  trusteddevicelist();
    //TODO:check path
    trustedDevicelist.setFileName(QString("list.dat"));
    QString dir=QDir::currentPath();
    //dir.lastIndexOf('/');
    trustedDevicelist.setFileDirectory(dir);// /home/linke/Desktop/BlueMoon/"));

    //trustedDevicelist.writeToTrustedDeviceList();
    trustedDevicelist.readTrustedDeviceList();

    localDevice_.reset(new QBluetoothLocalDevice);
    QBluetoothAddress adapterAddress = localDevice_->address();
    discoveryAgent_.reset(new QBluetoothServiceDiscoveryAgent(adapterAddress));

    connect(discoveryAgent_.data(), &QBluetoothServiceDiscoveryAgent::serviceDiscovered, this, &RemoteSelector::serviceDiscovered);
    connect(discoveryAgent_.data(), &QBluetoothServiceDiscoveryAgent::serviceDiscovered, this, &RemoteSelector::discoveryFinished);
    connect(discoveryAgent_.data(), &QBluetoothServiceDiscoveryAgent::canceled, this, &RemoteSelector::discoveryFinished);

    ui->remoteDevices->setColumnWidth(3, 75);
    ui->remoteDevices->setColumnWidth(4, 100);

    connect(localDevice_.data(), &QBluetoothLocalDevice::pairingDisplayPinCode, this, &RemoteSelector::displayPin);
    connect(localDevice_.data(), &QBluetoothLocalDevice::pairingDisplayConfirmation, this, &RemoteSelector::displayConfirmation);
    connect(localDevice_.data(), &QBluetoothLocalDevice::pairingFinished, this, &RemoteSelector::pairingFinished);
    connect(localDevice_.data(), &QBluetoothLocalDevice::error, this, &RemoteSelector::pairingError);

    ui->busyWidget->setMovie(new QMovie(":/icons/busy.gif"));
    ui->busyWidget->movie()->start();

    ui->pairingBusy->setMovie(new QMovie(":/icons/pairing.gif"));
    ui->pairingBusy->hide();

    ui->remoteDevices->clearContents();
    ui->remoteDevices->setRowCount(0);

    //QPushButton* pButton = new QPushutton("click_on",MyTable);

    //MyTable->setCellWidget(Rownum,ColNum,pButton);
    //connect(pButton, SIGNAL(clicked()), &ButtonSignalMapper, SLOT(map()));
    //ButtonSignalMapper.setMapping(pButton, RowNum);
    //connect(&ButtonSignalMapper, SIGNAL(mapped(int)), this, SLOT(CellButtonClicked(int)));

    showtrustedDeviceList(trustedDevicelist);

    connect(ui->sendFilesButton, &QPushButton::clicked, this, &RemoteSelector::sendFileButton_clicked);

    createActions();
    createTrayIcon();
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
    trayIcon->show();
}

void RemoteSelector::startDiscovery(const QBluetoothUuid& uuid) {
    ui->stopButton->setDisabled(false);
    if (discoveryAgent_->isActive())
        discoveryAgent_->stop();

    discoveryAgent_->setUuidFilter(uuid);
    discoveryAgent_->start();

    if (!discoveryAgent_->isActive() ||
            discoveryAgent_->error() != QBluetoothServiceDiscoveryAgent::NoError)
    {
        ui->status->setText(tr("Cannot find remote services."));
    } else {
        ui->status->setText(tr("Scanning..."));
        ui->busyWidget->show();
        ui->busyWidget->movie()->start();
    }
}

QBluetoothServiceInfo RemoteSelector::service() const {
    return service_;
}

void RemoteSelector::serviceDiscovered(const QBluetoothServiceInfo& serviceInfo) {
#if DEBUG_OUTPUT
    qDebug() << "Discovered service on"
             << serviceInfo.device().name() << serviceInfo.device().address().toString();
    qDebug() << "\tService name:" << serviceInfo.serviceName();
    qDebug() << "\tDescription:"
             << serviceInfo.attribute(QBluetoothServiceInfo::ServiceDescription).toString();
    qDebug() << "\tProvider:"
             << serviceInfo.attribute(QBluetoothServiceInfo::ServiceProvider).toString();
    qDebug() << "\tL2CAP protocol service multiplexer:"
             << serviceInfo.protocolServiceMultiplexer();
    qDebug() << "\tRFCOMM server channel:" << serviceInfo.serverChannel();
#endif

    QString remoteName;
    if (serviceInfo.device().name().isEmpty())
        remoteName = serviceInfo.device().address().toString();
    else
        remoteName = serviceInfo.device().name();

    QMutableMapIterator<int, QBluetoothServiceInfo> i(discoveredServices_);
    while (i.hasNext()){
        i.next();
        if (serviceInfo.device().address() == i.value().device().address()){
            i.setValue(serviceInfo);
            return;
        }
    }

    int row = ui->remoteDevices->rowCount();
    ui->remoteDevices->insertRow(row);
    QTableWidgetItem *item = new QTableWidgetItem(serviceInfo.device().address().toString());
    ui->remoteDevices->setItem(row, 0, item);
    item = new QTableWidgetItem(serviceInfo.device().name());
    ui->remoteDevices->setItem(row, 1, item);
    item = new QTableWidgetItem(serviceInfo.serviceName());

    ui->remoteDevices->setItem(row, 2, item);

    QBluetoothLocalDevice::Pairing p;

    p = localDevice_->pairingStatus(serviceInfo.device().address());

    ui->remoteDevices->blockSignals(true);

    item = new QTableWidgetItem();
    if ((p&QBluetoothLocalDevice::Paired) || (p&QBluetoothLocalDevice::AuthorizedPaired))
        item->setCheckState(Qt::Checked);
    else
        item->setCheckState(Qt::Unchecked);
    ui->remoteDevices->setItem(row, 3, item);

    item = new QTableWidgetItem();
    if (p&QBluetoothLocalDevice::AuthorizedPaired)
        item->setCheckState(Qt::Checked);
    else
        item->setCheckState(Qt::Unchecked);

    ui->remoteDevices->setItem(row, 4, item);

    ui->remoteDevices->blockSignals(false);


    discoveredServices_.insert(row, serviceInfo);
}

void RemoteSelector::discoveryFinished() {
    ui->status->setText(tr("Select the device to send to."));
    ui->stopButton->setDisabled(true);
    ui->busyWidget->movie()->stop();
    ui->busyWidget->hide();
}

void RemoteSelector::startDiscovery() {
    startDiscovery(QBluetoothUuid(QBluetoothUuid::ObexObjectPush));
}

void RemoteSelector::on_refreshPB_clicked() {
    startDiscovery();
    ui->stopButton->setDisabled(false);
}

void RemoteSelector::on_stopButton_clicked() {
    discoveryAgent_->stop();
}

QString RemoteSelector::addressToName(const QBluetoothAddress &address) {
    QMapIterator<int, QBluetoothServiceInfo> i(discoveredServices_);
    while (i.hasNext()){
        i.next();
        if (i.value().device().address() == address)
            return i.value().device().name();
    }
    return address.toString();
}

void RemoteSelector::displayPin(const QBluetoothAddress &address, QString pin) {
    pinDialog_.reset(new PinDialog(this, QString("Enter pairing pin on: %1").arg(addressToName(address)), pin));
    pinDialog_->show();
}

void RemoteSelector::displayConfirmation(const QBluetoothAddress &address, QString pin) {
    Q_UNUSED(address);

    pinDialog_.reset(new PinDialog(this, QString("Confirm this pin is the same"), pin));
    connect(pinDialog_.data(), &PinDialog::accepted, this, &RemoteSelector::displayConfAccepted);
    connect(pinDialog_.data(), &PinDialog::rejected, this, &RemoteSelector::displayConfReject);
    pinDialog_->initializeComponent();
    pinDialog_->show();
}

void RemoteSelector::displayConfAccepted() {
    localDevice_->pairingConfirmation(true);
}

void RemoteSelector::displayConfReject() {
    localDevice_->pairingConfirmation(false);
}

void RemoteSelector::pairingFinished(const QBluetoothAddress &address, QBluetoothLocalDevice::Pairing status) {
    QBluetoothServiceInfo service;
    int row = 0;

    ui->pairingBusy->hide();
    ui->pairingBusy->movie()->stop();

    ui->remoteDevices->blockSignals(true);

    for (int i = 0; i < discoveredServices_.count(); i++) {
        if (discoveredServices_.value(i).device().address() == address) {
            service = discoveredServices_.value(i);
            row = i;
            break;
        }
    }

    pinDialog_.reset();

    QMessageBox msgBox;
    if (pairingErrorFlag_) {
        msgBox.setText("Pairing failed with " + address.toString());
    } else if (status == QBluetoothLocalDevice::Paired
               || status == QBluetoothLocalDevice::AuthorizedPaired) {
        msgBox.setText("Paired successfully with " + address.toString());
    } else {
        msgBox.setText("Pairing released with " + address.toString());
    }

    if (service.isValid()) {
        if (status == QBluetoothLocalDevice::AuthorizedPaired) {
            ui->remoteDevices->item(row, 3)->setCheckState(Qt::Checked);
            ui->remoteDevices->item(row, 4)->setCheckState(Qt::Checked);
        }
        else if (status == QBluetoothLocalDevice::Paired) {
            ui->remoteDevices->item(row, 3)->setCheckState(Qt::Checked);
            ui->remoteDevices->item(row, 4)->setCheckState(Qt::Unchecked);
        }
        else {
            ui->remoteDevices->item(row, 3)->setCheckState(Qt::Unchecked);
            ui->remoteDevices->item(row, 4)->setCheckState(Qt::Unchecked);
        }
    }

    pairingErrorFlag_ = false;
    msgBox.exec();

    ui->remoteDevices->blockSignals(false);
}

void RemoteSelector::pairingError(QBluetoothLocalDevice::Error error) {
    if (error != QBluetoothLocalDevice::PairingError)
        return;

    pairingErrorFlag_ = true;
    pairingFinished(service_.device().address(), QBluetoothLocalDevice::Unpaired);
}

void RemoteSelector::on_remoteDevices_cellClicked(int row, int column) {
    Q_UNUSED(column);
    service_ = discoveredServices_.value(row);
}

void RemoteSelector::on_remoteDevices_itemChanged(QTableWidgetItem* item) {
    int row = item->row();
    int column = item->column();
    service_ = discoveredServices_.value(row);

    if (column < 3)
        return;

    if (item->checkState() == Qt::Unchecked && column == 3){
        localDevice_->requestPairing(service_.device().address(), QBluetoothLocalDevice::Unpaired);
        return; // don't continue and start movie
    }
    else if ((item->checkState() == Qt::Checked && column == 3) ||
            (item->checkState() == Qt::Unchecked && column == 4)){
        localDevice_->requestPairing(service_.device().address(), QBluetoothLocalDevice::Paired);
        ui->remoteDevices->blockSignals(true);
        ui->remoteDevices->item(row, column)->setCheckState(Qt::PartiallyChecked);
        ui->remoteDevices->blockSignals(false);
    }
    else if (item->checkState() == Qt::Checked && column == 4){
        localDevice_->requestPairing(service_.device().address(), QBluetoothLocalDevice::AuthorizedPaired);
        ui->remoteDevices->blockSignals(true);
        ui->remoteDevices->item(row, column)->setCheckState(Qt::PartiallyChecked);
        ui->remoteDevices->blockSignals(false);
    }
    ui->pairingBusy->show();
    ui->pairingBusy->movie()->start();
}

void RemoteSelector::sendFileButton_clicked() {
    Progress* p = new Progress(0, service_);
    p->show();
}

void RemoteSelector::showtrustedDeviceList(trusteddevicelist tdl)
{

    QVector<QString> trustedList=tdl.getTrustedDevices();
    QTableWidgetItem *item;

    for (int i = 0; i < trustedList.size(); ++i)
    {
        int row = ui->remoteDevices->rowCount();
        ui->remoteDevices->insertRow(row);
        item=new QTableWidgetItem(trustedList[i]);
        ui->remoteDevices->setItem(i,1, item);
    }
}

// Delete after?!
void RemoteSelector::addToTrustList(trusteddevicelist tdl,const QBluetoothServiceInfo& serviceInfo)
{
    //tdl.addToTrustList(serviceInfo);

}
void RemoteSelector::deleteFromTrustList(trusteddevicelist tdl,const QBluetoothServiceInfo& serviceInfo)
{
    //tdl.deleteFromTrustList(serviceInfo);
}



// Tray Icon creating.
// Overriding the virtual function
void RemoteSelector::changeEvent(QEvent *event)
{
    QWidget::changeEvent(event);
    if(event->type() == QEvent::WindowStateChange)
        if(isMinimized())
            this->hide();
}

void RemoteSelector::createActions()
{
    minimizeAction = new QAction(tr("Minimize"), this);
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

    maximizeAction = new QAction(tr("Maximize"), this);
    connect(maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));

    bluetoothOnAction = new QAction(tr("Bluetooth ON"), this);
    connect(bluetoothOnAction, SIGNAL(triggered()), this, SLOT(bluetoothOn()));

    bluetoothOffAction = new QAction(tr("Bluetooth OFF"), this);
    connect(bluetoothOffAction, SIGNAL(triggered()), this, SLOT(bluetoothOff()));

    quitAction = new QAction(tr("Quit"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void RemoteSelector::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(bluetoothOnAction);
    trayIconMenu->addAction(bluetoothOffAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(maximizeAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
    // Or :/icons/bluetooth.svg
    trayIcon->setIcon(QIcon("bluetooth.svg"));
}

void RemoteSelector::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
    case QSystemTrayIcon::MiddleClick:
        showNormal();
    default:
        ;
    }
}

void RemoteSelector::bluetoothOn()
{
    this->showNormal();
    //localDevice_.data()->powerOn();

}

void RemoteSelector::bluetoothOff()
{
    this->hide();
    //localDevice_.data()->setHostMode(QBluetoothLocalDevice::HostPoweredOff);
}
