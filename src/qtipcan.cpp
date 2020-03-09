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


#include <qdebug.h>

#include "qtipcan.h"

#include "QTipProtocol.h"
#include "QTipPackets.h"


QTipCANDevice::QTipCANDevice(QObject *parent) : QCanBusDevice(parent)
{
    server.setMaxPendingConnections(10);

    // TDOO - Initialization

    qDebug() << "QTipCANDevice()";
}


QTipCANDevice::~QTipCANDevice()
{
    close();

    qDebug() << "~QTipCANDevice()";
}


QCanBusDeviceInfo QTipCANDevice::getDeviceInfo(const QString name)
{
    // TODO

    return QCanBusDevice::createDeviceInfo(name, false, true);
}


bool QTipCANDevice::open()
{
    if (state() != QCanBusDevice::UnconnectedState)
    {
        close();
    }

    server.listen(QHostAddress::Any, portNum);

    connect(&server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));

    qDebug() << "connection opened";

    return true;
}


void QTipCANDevice::close()
{
    for (auto* connection : connections)
    {
        if (connection && connection->isOpen())
        {
            connection->close();
        }
    }

    qDeleteAll(connections);

    server.close();

    setState(QCanBusDevice::UnconnectedState);

    qDebug() << "connection closed";
}


void QTipCANDevice::onNewConnection()
{
    qDebug() << "new connection";
}


bool QTipCANDevice::writeFrame(const QCanBusFrame &frame)
{
    QTIP_CANFrame_t qtipFrame;

    memset(&qtipFrame, 0, sizeof(qtipFrame));

    auto payload = frame.payload();

    int n = payload.size();

    // Restrict maximum CAN frame size
    if (n > 64) n = 64;

    qtipFrame.dlc = n;

    // Copy packet data
    for (int ii = 0; ii < n; ii++)
    {
        qtipFrame.data[ii] = payload.at(ii);
    }

    qtipFrame.ext = frame.hasExtendedFrameFormat();

    // Ignore timestamp for now...
    qtipFrame.hasTimestamp = 0;

    qtipFrame.frameType = frame.frameType();

    qtipFrame.idHi = frame.frameId() >> 16;
    qtipFrame.idLo = frame.frameId() && 0xFFFF;

    QTIP_Packet_t pkt;

    encodeQTIP_CANFramePacketStructure(&pkt, &qtipFrame);

    for (auto* connection : connections)
    {
        sendPacketToConnection(pkt, connection);
    }

    // TODO - Return success code
    return true;
}


void QTipCANDevice::sendPacketToConnection(QTIP_Packet_t &pkt, QTcpSocket *connection)
{
    if (!connection || !connection->isOpen())
    {
        return;
    }

    // TODO - Encode and send the packet...
}


void QTipCANDevice::setConfigurationParameter(int key, const QVariant &value)
{
    // TODO

    QCanBusDevice::setConfigurationParameter(key, value);
}


QString QTipCANDevice::interpretErrorFrame(const QCanBusFrame &errorFrame)
{
    // TODO

    return QString();
}


//! \return the packet data pointer from the packet
uint8_t* getQTipPacketData(QTIP_Packet_t* pkt)
{
    return pkt->data;
}


//! \return the packet data pointer from the packet, const
const uint8_t* getQTipPacketDataConst(const QTIP_Packet_t* pkt)
{
    return (const uint8_t*) pkt->data;
}


//! Complete a packet after the data have been encoded
void finishQTipPacket(QTIP_Packet_t* pkt, int size, uint32_t packetID)
{
    pkt->length = size;
    pkt->id = packetID;
}


//! \return the size of a packet from the packet header
int getQTipPacketSize(const QTIP_Packet_t* pkt)
{
    return pkt->length;
}


//! \return the ID of a packet from the packet header
uint32_t getQTipPacketID(const QTIP_Packet_t* pkt)
{
    return pkt->id;
}
