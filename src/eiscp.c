/*******************************************************************************
 * @file eiscp.c
 *
 * @author Clemens Haffner
 *
 * @brief eISCP TCP Packets
 * 
 * Reception / Transmission of Ethernet eISCP Commands via TCP.
 *
 * @version 0.1
 * @date 2023-11-05
 *
 ******************************************************************************/

#include <stdint.h>
#include <string.h>
#include "endian.h"
#include "eiscp.h"

typedef enum tState_e
{
    S_SEARCH_I,
    S_SEARCH_S,
    S_SEARCH_C,
    S_SEARCH_P,
    S_GET_HDRSIZE,
    S_GET_DATASIZE,
    S_GET_VERSION_RESERVED,
    S_IN_PACK,
} tState;

/* Reception Variables */
static tState state;
static uint8_t RxBuffer[257]; //+1 for null termination
static uint8_t RxBufferPos;
static uint32_t u32DataSize;

/* Transmission Variables */
static uint8_t TxBuffer[258]; // two more. CR-LF quirk.


int eiscp_send_packet(char pCommand[])
{
    tOnkyoPacket *pPack = (tOnkyoPacket *)TxBuffer;

    pPack->arPacketHeader[0] = 'I';
    pPack->arPacketHeader[1] = 'S';
    pPack->arPacketHeader[2] = 'C';
    pPack->arPacketHeader[3] = 'P';

    pPack->u32HeaderSize = swap_uint32(16);
    pPack->u8Version = 1;
    pPack->reserved[0] = 0;
    pPack->reserved[1] = 0;
    pPack->reserved[2] = 0;

    strncpy(pPack->data, pCommand, sizeof(TxBuffer) - 16);
    pPack->u32DataSize = strlen(pPack->data);

    // Copy our data. But there is a quirk: EOF (0x1A) at the end is not enough for eISCP
    if(pCommand[pPack->u32DataSize - 1] == 0x1A)
    {
        pPack->data[pPack->u32DataSize++] = '\r';
        pPack->data[pPack->u32DataSize++] = '\n';
    }
    return eiscp_send_data(TxBuffer, u32DataSize + 16);
}

void eiscp_consume_byte(uint8_t u8Byte)
{
    switch (state)
    {
    case S_SEARCH_I:
        RxBufferPos = 0;
        RxBuffer[RxBufferPos++] = u8Byte;
        state = (u8Byte == 'I') ? S_SEARCH_S : S_SEARCH_I;
        return;
    case S_SEARCH_S:
        RxBuffer[RxBufferPos++] = u8Byte;
        state = (u8Byte == 'S') ? S_SEARCH_C : S_SEARCH_I;
        return;
    case S_SEARCH_C:
        RxBuffer[RxBufferPos++] = u8Byte;
        state = (u8Byte == 'C') ? S_SEARCH_P : S_SEARCH_I;
        return;
    case S_SEARCH_P:
        RxBuffer[RxBufferPos++] = u8Byte;
        state = (u8Byte == 'P') ? S_IN_PACK : S_SEARCH_I;
        // Seems good, we likely have a packet!
        return;
    case S_GET_HDRSIZE:
        RxBuffer[RxBufferPos++] = u8Byte;
        if(RxBufferPos == 8)
        {
            uint32_t *pBigEndian = (uint32_t *)&RxBuffer[4];
            state = (16 == swap_uint32(*pBigEndian)) ? S_GET_DATASIZE : S_SEARCH_I; // Only Header Size == 16 is OK
        }
        return;
    case S_GET_DATASIZE:
        RxBuffer[RxBufferPos++] = u8Byte;
        if(RxBufferPos == 12)
        {
            uint32_t *pBigEndian = (uint32_t *)&RxBuffer[4];
            u32DataSize = swap_uint32(*pBigEndian);
            state = (u32DataSize > (sizeof(RxBuffer) - 17)) ? S_SEARCH_I : S_GET_VERSION_RESERVED; // - Heaader, - null termination
        }
        return;
    case S_GET_VERSION_RESERVED:
        RxBuffer[RxBufferPos++] = u8Byte;
        if(RxBufferPos == 16)
        {
            // we don't care about the protocol version.
            // As long as the header size fits, everything should be fine.
            state = S_IN_PACK;
        }
        return;
    case S_IN_PACK:
        // finally... HERE COME THE DATA!
        if(RxBufferPos < (sizeof(RxBuffer) - 1))
        {
            RxBuffer[RxBufferPos++] = u8Byte;
            if((u8Byte == '\r') || (u8Byte == '\n') || (u8Byte == 0x1A)) // 0x1A is ACII <EOF>
            {
                // here we could check the size... be careful <CR> and <CR><LF> are valid

                // One Last (important) thing. We Null Terminate our Message
                // Not needed for serial <-> ETH bridge but if we want to parse the data.
                RxBuffer[RxBufferPos] = '\0';
                eiscp_on_packet((tOnkyoPacket *)RxBuffer);
                state = S_SEARCH_I;
            }
        }
        else
        {
            // Too many Data! Can not be a valid packet
            state = S_SEARCH_I;
        }
        return;
    default:
        // Should never happen ... State machine corrupted!
        state = S_SEARCH_I;
        break;
    }
}