/**

Copyright (c) 2020 DroneCAN

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

**/


#ifndef QTIPCAN_H
#define QTIPCAN_H

#include <qcanbus.h>
#include <qtcpserver.h>
#include <qtcpsocket.h>

#include "qtippacket.h"
#include "qtipcanconnection.h"


class QTipCANDevice : public QCanBusDevice
{
    Q_OBJECT

public:
    QTipCANDevice(QObject *parent = nullptr);

    virtual ~QTipCANDevice();

    static QCanBusDeviceInfo getDeviceInfo(const QString name);

    virtual bool writeFrame(const QCanBusFrame &frame) override;
    virtual void setConfigurationParameter(int key, const QVariant &value) override;
    virtual QString interpretErrorFrame(const QCanBusFrame &errorFrame) override;

    int getPortNumber(void) const { return portNum; }
    void setPortNumber(int n) { portNum = n; }

    static const int DEFAULT_PORT_NUM = 13579;

protected slots:
    void onNewConnection();

protected:
    virtual bool open() override;
    virtual void close() override;

    void sendPacketToConnection(const QTIP_Packet_t &pkt, QTcpSocket* connection);
    void flushConnections(void);

    //! Default port number
    int portNum;

    //! TCP server for accepting remote connections
    QTcpServer server;

    //! List of connected socket objects
    QList<QTipCANConnection*> connections;
};

#endif // QTIPCAN_H
