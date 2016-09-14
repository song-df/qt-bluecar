#ifndef QARDUINO_H
#define QARDUINO_H

#include <QObject>
#include <QtBluetooth>
#include <QTimer>

class QArduino : public QObject
{
    Q_OBJECT

    typedef struct{
        QString name;
        QVariant value;
    } arduinoAttribute;
public:
    explicit QArduino( QString bluetoothName, QObject *parent = 0);

    QList<arduinoAttribute> attributeList();
    arduinoAttribute attribute(QString attributeName);
    void sendMsg(QString);

signals:
    void connectionStatus(bool status);
    void connectionInfo(QString info);

    void attributeReceived(QString name, QVariant value);
    void attributeReceived(QString name, int value);
    void attributeReceived(QString name, QString value);
    void attributeReceived(arduinoAttribute attribute);
public slots:

private slots:
    void handleSocketConnected();
    void handleSocketDisconnected();
    void handleSocketError(QBluetoothSocket::SocketError error);
    void handleSocketReadyRead();

    void handleServiceDiscovered(QBluetoothServiceInfo service);
    void handleServiceDiscoveryError(QBluetoothServiceDiscoveryAgent::Error error);

    void slotTimeout();
    void addDevice(const QBluetoothDeviceInfo &info);
    void displayPin(QBluetoothAddress address, QString pin);
    void displayConfirmation(QBluetoothAddress address, QString pin);
    void pairingFinished(QBluetoothAddress address, QBluetoothLocalDevice::Pairing status);
    void slotError(QBluetoothServiceDiscoveryAgent::Error);
private:
    QBluetoothSocket *socket;

    QBluetoothLocalDevice *localDevice;

    QBluetoothDeviceDiscoveryAgent *discovery;
    QBluetoothServiceDiscoveryAgent *discoveryServiceAgent;
    QString deviceNameToConnect;
    QByteArray readBuffer;
    QTimer *timer;
};

#endif // QARDUINO_H
