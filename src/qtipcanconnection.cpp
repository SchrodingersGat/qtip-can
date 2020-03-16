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


QTipCANConnection::QTipCANConnection(QTipCANDevice *d, QTcpSocket *s) : QTcpSocket(), device(d), socket(s)
{
    connect(socket, SIGNAL(readyRead()), this, SLOT(onBytesAvailable()));

    connect(this, SIGNAL(disconnected()), device, SLOT(flushConnections()));

    // Open the socket
    open(OpenModeFlag::ReadWrite);

    QTipDebug() << "Creating new QTipCANConnection";
}


QTipCANConnection::~QTipCANConnection()
{
    QTipDebug() << "Deleting QTipCANConnection";

    if (isOpen()) close();
}


void QTipCANConnection::onBytesAvailable()
{
    auto bytes = socket->readAll();

    parseData(bytes);
}


void QTipCANConnection::parseData(QByteArray &bytes)
{
    QTipDebug() << bytes;
}

