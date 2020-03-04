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


QTipCANDevice::QTipCANDevice(QObject *parent) : QCanBusDevice(parent)
{
    // TDOO - Initialization
}


QTipCANDevice::~QTipCANDevice()
{
    // TODO
}


QCanBusDeviceInfo QTipCANDevice::getDeviceInfo(const QString name)
{
    // TODO

    return QCanBusDevice::createDeviceInfo(name, false, true);
}


bool QTipCANDevice::open()
{
    // TODO

    return false;
}


void QTipCANDevice::close()
{
    // TODO

    setState(QCanBusDevice::UnconnectedState);
}


bool QTipCANDevice::writeFrame(const QCanBusFrame &frame)
{
    // TODO

    Q_UNUSED(frame);
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
