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


#include "qtipcan.h"

#include "QTipProtocol.h"
#include "QTipPackets.h"

#include "qtipdebug.h"

/**
 * @brief QTipCANDevice::QTipCANDevice - Create a new QTipCANDevice object
 * @param parent
 */
QTipCANDevice::QTipCANDevice(QObject *parent) : QCanBusDevice(parent)
{
    portNum = DEFAULT_PORT_NUM;

    server.setMaxPendingConnections(10);

    QTipDebug() << "QTipCANDevice()";

    // TDOO - Initialization
}


QTipCANDevice::~QTipCANDevice()
{
    close();

    flushConnections();

    for (auto *connection : connections)
    {
        if (connection) connection->close();
    }

    QTipDebug() << "~QTipCANDevice()";
}


QCanBusDeviceInfo QTipCANDevice::getDeviceInfo(const QString name)
{
    // TODO

    return QCanBusDevice::createDeviceInfo(name, false, true);
}


/**
 * @brief QTipCANDevice::open - Open the CAN connection.
 * This starts a TCP server listening on the configured port.
 * @return true if the connection was successfully opened
 */
bool QTipCANDevice::open()
{
    if (state() != QCanBusDevice::UnconnectedState)
    {
        close();
    }

    server.listen(QHostAddress::Any, portNum);

    connect(&server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));

    setState(QCanBusDevice::ConnectedState);

    return true;
}


/**
 * @brief QTipCANDevice::close - Shut down the TCP server and disconnect any open connections
 */
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
}


/**
 * @brief QTipCANDevice::onNewConnection - Callback when a new TCP socket is opened
 */
void QTipCANDevice::onNewConnection()
{
    auto* socket= server.nextPendingConnection();

    QTipDebug() << "QTip - New connection recevied";

    if (socket != nullptr)
    {
        QTipCANConnection* connection = new QTipCANConnection(this, socket);

        connections.append(connection);
    }

    flushConnections();
}


/**
 * @brief QTipCANDevice::flushConnections - Delete any disconnected or invalid connections.
 * Call periodically to ensure that any dead connections are removed.
 */
void QTipCANDevice::flushConnections()
{
    int idx = 0;

    QTipDebug() << "flushing connections";

    while (idx < connections.count())
    {
        auto* connection = connections.at(idx);

        if (!connection || !connection->isOpen())
        {
            QTipDebug() << "Removing old connection";

            if (connection) delete connection;

            connections.removeAt(idx);
            continue;
        }

        idx++;
    }
}


/**
 * @brief QTipCANDevice::writeFrame - Send a QCanBusFrame to all connected sockets
 * @param frame - QCanBusFrame to write
 * @return
 */
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


/**
 * @brief QTipCANDevice::sendPacketToConnection - Transmit packet to a single connected socket
 * @param pkt - reference to the packet being transmitted
 * @param connection - pointer to the socket connection
 */
void QTipCANDevice::sendPacketToConnection(const QTIP_Packet_t &pkt, QTcpSocket *connection)
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
