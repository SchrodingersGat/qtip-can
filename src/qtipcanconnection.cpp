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


#include "qtipcanconnection.h"
#include "qtipcan.h"
#include "qtipdebug.h"


/**
 * @brief QTipCANConnection::QTipCANConnection - Create a new QTipCANConnection object
 * @param d - pointer to the QTipCANDevice object
 * @param s - pointer to the QTcpSocket object
 */
QTipCANConnection::QTipCANConnection(QTipCANDevice *d, QTcpSocket *s) : QObject(), device(d), socket(s)
{
    connect(socket, SIGNAL(readyRead()), this, SLOT(onBytesAvailable()));
    connect(socket, SIGNAL(disconnected()), device, SLOT(flushConnections()));

    connect(this, SIGNAL(packetReceived(QTIP_Packet_t)), device, SLOT(onNewPacket(QTIP_Packet_t)));

    // Open the socket
    socket->open(QTcpSocket::OpenModeFlag::ReadWrite);

    QTipDebug() << "Creating new QTipCANConnection";
}


QTipCANConnection::~QTipCANConnection()
{
    QTipDebug() << "Deleting QTipCANConnection";

    if (socket->isOpen()) socket->close();
}


/**
 * @brief QTipCANConnection::sendPacket - Serialize and transmit packet over this socket
 * @param pkt - packet to transmit
 * @return - true if the packet was successfully transmitted
 */
bool QTipCANConnection::sendPacket(const QTIP_Packet_t &pkt)
{
    QByteArray bytes;

    // TODO - Encode the packet into the bytes

    return socket->write(bytes) == bytes.count();
}


/**
 * @brief QTipCANConnection::onBytesAvailable - Called when new data are available
 */
void QTipCANConnection::onBytesAvailable()
{
    auto bytes = socket->readAll();

    parseData(bytes);
}


/**
 * @brief QTipCANConnection::parseData - Parse recently received data
 * @param bytes
 */
void QTipCANConnection::parseData(QByteArray &bytes)
{
    QTipDebug() << bytes;

    QTIP_Packet_t pkt;

    bool result = false;

    // TODO - Check the bytes and see if a packet has been received

    if (result)
    {
        emit packetReceived(pkt);
    }
}

