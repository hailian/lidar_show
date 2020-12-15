#include "udpreceiver.h"

UdpReceiver::UdpReceiver(QObject *parent) : QObject(parent)
{
    uSocket = new QUdpSocket;
    //uSocket->bind(QHostAddress("192.168.11.101"), 9998);
    uSocket->bind(QHostAddress("127.0.0.1"), 9998);
    connect(uSocket, SIGNAL(readyRead()), this, SLOT(receive()));

}


void UdpReceiver::receive()
{
    QByteArray ba;
    while(uSocket->hasPendingDatagrams())
    {
        ba.resize(uSocket->pendingDatagramSize());
        uSocket->readDatagram(ba.data(), ba.size());
        emit readyRead(ba);
    }
}
