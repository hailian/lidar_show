#ifndef UDPRECEIVER_H
#define UDPRECEIVER_H

#include <QObject>
#include <QUdpSocket>


class UdpReceiver : public QObject
{
    Q_OBJECT
public:
    explicit UdpReceiver(QObject *parent = nullptr);

signals:
    void readyRead(QByteArray qba);

public slots:
    void receive();

private:
    QUdpSocket *uSocket;
};

#endif // UDPRECEIVER_H
