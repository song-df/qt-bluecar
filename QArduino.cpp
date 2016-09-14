#include "QArduino.h"
#include <QDebug>

QArduino::QArduino(QString bluetoothName, QObject *parent) : QObject(parent)
{

    deviceNameToConnect = bluetoothName;

    socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);
    connect(socket, SIGNAL(connected()), this, SLOT(handleSocketConnected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(handleSocketDisconnected()));
    connect(socket, SIGNAL(error(QBluetoothSocket::SocketError)),
            this, SLOT(handleSocketError(QBluetoothSocket::SocketError)));
    connect(socket, SIGNAL(readyRead()), this, SLOT(handleSocketReadyRead()));


    discovery = new QBluetoothDeviceDiscoveryAgent();
//    connect(discovery, SIGNAL(error(QBluetoothServiceDiscoveryAgent::Error)),
//            this, SLOT(handleServiceDiscoveryError(QBluetoothServiceDiscoveryAgent::Error)));
    connect(discovery, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)),
            this, SLOT(addDevice(QBluetoothDeviceInfo)));
//    connect(discovery, SIGNAL(serviceDiscovered(QBluetoothServiceInfo)),
//            this, SLOT(handleServiceDiscovered(QBluetoothServiceInfo)));
    discovery->start();

    //qDebug() << discovery->discoveredServices();
    qDebug() << "discovering...";
    emit connectionInfo("Discovering...");
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(slotTimeout()));

}

void QArduino::addDevice(const QBluetoothDeviceInfo &info)
{
    QString label = QString("%1 %2").arg(info.address().toString()).arg(info.name());
    qDebug() << label;

    if(info.name() == deviceNameToConnect){
        localDevice = new QBluetoothLocalDevice;
        QBluetoothAddress adapterAddress = localDevice->address();
        connect(localDevice, SIGNAL(pairingDisplayPinCode(QBluetoothAddress,QString)),
                this, SLOT(displayPin(QBluetoothAddress,QString)));
        connect(localDevice, SIGNAL(pairingDisplayConfirmation(QBluetoothAddress,QString)),
                this, SLOT(displayConfirmation(QBluetoothAddress,QString)));
        connect(localDevice, SIGNAL(pairingFinished(QBluetoothAddress,QBluetoothLocalDevice::Pairing)),
                this, SLOT(pairingFinished(QBluetoothAddress,QBluetoothLocalDevice::Pairing)));

        discoveryServiceAgent = new QBluetoothServiceDiscoveryAgent(adapterAddress);

        discoveryServiceAgent->setRemoteAddress(info.address());

        connect(discoveryServiceAgent, SIGNAL(serviceDiscovered(QBluetoothServiceInfo)),
                this, SLOT(handleServiceDiscovered(QBluetoothServiceInfo)));
        connect(discoveryServiceAgent,SIGNAL(error(QBluetoothServiceDiscoveryAgent::Error)),
                this,SLOT(slotError(QBluetoothServiceDiscoveryAgent::Error)));
        //connect(discoveryServiceAgent, SIGNAL(finished()), ui->status, SLOT(hide()));
        discoveryServiceAgent->setUuidFilter(QBluetoothUuid(QBluetoothUuid::SerialPort));
        discoveryServiceAgent->start();
    }
    //    QList<QListWidgetItem *> items = ui->list->findItems(label, Qt::MatchExactly);
//    if (items.empty()) {
//        QListWidgetItem *item = new QListWidgetItem(label);
//        QBluetoothLocalDevice::Pairing pairingStatus = localDevice->pairingStatus(info.address());
//        if (pairingStatus == QBluetoothLocalDevice::Paired || pairingStatus == QBluetoothLocalDevice::AuthorizedPaired )
//            item->setTextColor(QColor(Qt::green));
//        else
//            item->setTextColor(QColor(Qt::black));

//    }

}

void QArduino::slotError(QBluetoothServiceDiscoveryAgent::Error err)
{
    qDebug() << "service discover error" << err;
}

void QArduino::displayPin(QBluetoothAddress address,QString pin)
{
    Q_UNUSED(address);
    emit connectionInfo("pair pin: " + pin);
}

void QArduino::displayConfirmation(QBluetoothAddress address,QString pin)
{
    Q_UNUSED(address);

    emit connectionInfo("confirm pair pin: " + pin);
}

void QArduino::pairingFinished(QBluetoothAddress address,QBluetoothLocalDevice::Pairing status)
{
    Q_UNUSED(address)
    Q_UNUSED(status)
    emit connectionInfo("pairing finished");
}
void QArduino::slotTimeout()
{
    if(socket->isOpen()){
        socket->write(QString("hello").toUtf8().data(),6);
    }
}
void QArduino::sendMsg(QString msg)
{
    if(socket->isOpen())
        socket->write(msg.toUtf8().data(),msg.toUtf8().length());
}

void QArduino::handleSocketConnected()
{
    qDebug() << "QArduino::handleSocketConnected";
    //timer->start(1000);
    emit connectionInfo("Connected to " + deviceNameToConnect);
}

void QArduino::handleSocketDisconnected()
{
    qDebug() << "QArduino::handleSocketDisconnected";
    emit connectionInfo("Connection lost");
}

void QArduino::handleSocketError(QBluetoothSocket::SocketError error)
{
    qDebug() << "QArduino::handleSocketError error:" << error;
}

void QArduino::handleSocketReadyRead()
{
    qDebug() << "QArduino::handleSocketReadyRead";
    readBuffer = socket->readAll();
    emit connectionInfo(QString(readBuffer));
    socket->write(readBuffer,readBuffer.length());
#if 0
    if(readBuffer.count() >= 2)
    {
        bool ok;
        uint16_t fullLength = readBuffer.mid(0,2).toUInt(&ok);
        if( ok &&
            ((readBuffer.count() -2 ) >= fullLength))
        {
            //Remove the fullLength
            readBuffer.remove(0, 2);
            uint8_t nameLength = (uint8_t)readBuffer.at(0);
            if( (nameLength -1) > readBuffer.count())
            {
                qDebug() << "Error (nameLength -1) > readBuffer.count()";
                return;
            }
            QString name = QString::fromLocal8Bit(readBuffer.mid(1, nameLength));
            readBuffer.remove(0, 1+nameLength);
            if(readBuffer.count() < 2)
            {
                qDebug() << "Error readBuffer.count() < 2";
            }
            uint8_t dataType = (uint8_t)readBuffer.at(0);
            uint8_t dataLength = (uint8_t)readBuffer.at(1);


        }
        //else data for this packet haven't arrive in buffer
    }

    //uint16 fulllength
    //uint8 name length
    //QString name 1-256 char
    //uint8 dataType
    //0 bool
    //1 uint8
    //2 uint16
    //3 uint32
    //4 int8
    //5 int16
    //6 int32
    //7 float32
    //8 string
    //data length 1byte
    //data
#endif
}

void QArduino::handleServiceDiscovered(QBluetoothServiceInfo service)
{
    qDebug() << "QArduino::handleServiceDiscovered name:" << service.serviceName();

    emit connectionInfo("Discovered");
//    if( (service.device().name() == deviceNameToConnect) &&
//        !deviceNameToConnect.isEmpty() &&
//        (service.socketProtocol() == QBluetoothServiceInfo::RfcommProtocol))
    {
        qDebug() << "QArduino::handleServiceDiscovered connectToService";
        socket->connectToService(service);
        emit connectionInfo("connect to " + deviceNameToConnect);
    }
}

void QArduino::handleServiceDiscoveryError(QBluetoothServiceDiscoveryAgent::Error error)
{
    qDebug() << "QArduino::handleServiceDiscoveryError error:" << error;
}
