/*******************************************************************************
 * @file eiscp.h
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

#ifndef EISCP_H
#define EISCP_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef struct __attribute__ ((packed)) tOnkyoPacket_s
{
    uint8_t     arPacketHeader[4];
    uint32_t    u32HeaderSize;
    uint32_t    u32DataSize;
    uint8_t     u8Version;
    uint8_t     reserved[3];
    char        data[];
}tOnkyoPacket;

// Higher Layer Interface
int eiscp_send_packet(char pCommand[]);
int eiscp_build_packet(char pCommand[], uint8_t *pBuffer, uint16_t u16LenBuffer);
void eiscp_on_packet(tOnkyoPacket * pPacket); // Gets called back if packet is detected from TCP stream

// LL Interface.
void eiscp_consume_byte(uint8_t u8Byte); // Call from Rx Interrupt
int eiscp_send_data(uint8_t *pData, uint16_t u16Len); // Implement Tx here.

#ifdef __cplusplus
}
#endif
#endif /* EISCP_H */
