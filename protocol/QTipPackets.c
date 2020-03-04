// QTipPackets.c was generated by ProtoGen version 2.18.c

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

#include "QTipPackets.h"
#include "fielddecode.h"
#include "fieldencode.h"
#include "scaleddecode.h"
#include "scaledencode.h"

/*!
 * \brief Create the QTIP_CANFrame packet
 *
 * Serialized CAN data
 * \param _pg_pkt points to the packet which will be created by this function
 * \param _pg_user points to the user data that will be encoded in _pg_pkt
 */
void encodeQTIP_CANFramePacketStructure(QTIP_Packet_t* _pg_pkt, const QTIP_CANFrame_t* _pg_user)
{
    uint8_t* _pg_data = getQTipPacketData(_pg_pkt);
    int _pg_byteindex = 0;
    unsigned _pg_i = 0;

    // 0 = Std Id, 1 = Ext Id
    _pg_data[_pg_byteindex] = (uint8_t)_pg_user->ext << 7;

    // CAN frame type
    _pg_data[_pg_byteindex] |= (uint8_t)_pg_user->frameType << 5;

    // 1 if the packet contains timestamp information
    _pg_data[_pg_byteindex] |= (uint8_t)_pg_user->hasTimestamp << 4;
    _pg_byteindex += 1; // close bit field

    // CAN frame ID (upper bytes)
    if(_pg_user->ext)
    {
        uint16ToBeBytes(_pg_user->idHi, _pg_data, &_pg_byteindex);
    }

    // CAN frame ID (lower bytes)
    uint16ToBeBytes(_pg_user->idLo, _pg_data, &_pg_byteindex);

    // CAN data length code
    uint8ToBytes(_pg_user->dlc, _pg_data, &_pg_byteindex);

    // CAN frame timestamp
    if(_pg_user->hasTimestamp)
    {
        uint32ToBeBytes(_pg_user->timestamp, _pg_data, &_pg_byteindex);
    }

    for(_pg_i = 0; _pg_i < (unsigned)_pg_user->dlc && _pg_i < 64; _pg_i++)
        uint8ToBytes(_pg_user->data[_pg_i], _pg_data, &_pg_byteindex);

    // complete the process of creating the packet
    finishQTipPacket(_pg_pkt, _pg_byteindex, getQTIP_CANFramePacketID());
}

/*!
 * \brief Decode the QTIP_CANFrame packet
 *
 * Serialized CAN data
 * \param _pg_pkt points to the packet being decoded by this function
 * \param _pg_user receives the data decoded from the packet
 * \return 0 is returned if the packet ID or size is wrong, else 1
 */
int decodeQTIP_CANFramePacketStructure(const QTIP_Packet_t* _pg_pkt, QTIP_CANFrame_t* _pg_user)
{
    int _pg_numbytes;
    int _pg_byteindex = 0;
    const uint8_t* _pg_data;
    unsigned _pg_i = 0;

    // Verify the packet identifier
    if(getQTipPacketID(_pg_pkt) != getQTIP_CANFramePacketID())
        return 0;

    // Verify the packet size
    _pg_numbytes = getQTipPacketSize(_pg_pkt);
    if(_pg_numbytes < getQTIP_CANFrameMinDataLength())
        return 0;

    // The raw data from the packet
    _pg_data = getQTipPacketDataConst(_pg_pkt);

    // 0 = Std Id, 1 = Ext Id
    _pg_user->ext = (_pg_data[_pg_byteindex] >> 7);

    // CAN frame type
    _pg_user->frameType = ((_pg_data[_pg_byteindex] >> 5) & 0x3);

    // 1 if the packet contains timestamp information
    _pg_user->hasTimestamp = ((_pg_data[_pg_byteindex] >> 4) & 0x1);
    _pg_byteindex += 1; // close bit field

    // CAN frame ID (upper bytes)
    if(_pg_user->ext)
    {
    _pg_user->idHi = uint16FromBeBytes(_pg_data, &_pg_byteindex);
    }

    // CAN frame ID (lower bytes)
    _pg_user->idLo = uint16FromBeBytes(_pg_data, &_pg_byteindex);

    // CAN data length code
    _pg_user->dlc = uint8FromBytes(_pg_data, &_pg_byteindex);

    // CAN frame timestamp
    if(_pg_user->hasTimestamp)
    {
    _pg_user->timestamp = uint32FromBeBytes(_pg_data, &_pg_byteindex);
    }

    for(_pg_i = 0; _pg_i < (unsigned)_pg_user->dlc && _pg_i < 64; _pg_i++)
        _pg_user->data[_pg_i] = uint8FromBytes(_pg_data, &_pg_byteindex);

    // Used variable length arrays or dependent fields, check actual length
    if(_pg_numbytes < _pg_byteindex)
        return 0;

    return 1;
}
// end of QTipPackets.c
