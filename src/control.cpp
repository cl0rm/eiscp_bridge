/*******************************************************************************
 * @file control.cpp
 *
 * @author Clemens Haffner
 *
 * @brief Main for eISCP bridge
 * 
 * This combines the different modules in this project.
 *
 * @version 0.1
 * @date 2023-11-05
 *
 ******************************************************************************/

#include "iscp.h"
#include "eiscp.h"
#include "tcp_server.h"
#include "serial.h"
#include "control.h"


/*******************************************************************************
 * @brief Ethernet -> Serial
 * 
 * @param pPacket Structure with the ISCP data + Header
 ******************************************************************************/
void eiscp_on_packet(tOnkyoPacket * pPacket)
{
    iscp_send_message(pPacket->data);
}

/*******************************************************************************
 * @brief Serial -> Ethernet
 * 
 * @param pMessage ISCP message
 ******************************************************************************/
void iscp_on_message(char pMessage[])
{
    eiscp_send_packet(pMessage);
}

// Transmission Glue Code

int iscp_send_data(uint8_t *pData, uint16_t u16Len)
{
    return serial_send(pData, u16Len);
}

int eiscp_send_data(uint8_t *pData, uint16_t u16Len)
{
    return tcp_send(pData, u16Len);
}

// Reception Glue Code

void control_serial_rx(uint8_t *pData, uint16_t u16Len)
{
    for (uint16_t i = 0; i < u16Len; i++)
    {
        iscp_consume_byte(pData[i]);
    }
}

void control_tcp_rx(uint8_t *pData, uint16_t u16Len)
{
    for (uint16_t i = 0; i < u16Len; i++)
    {
        eiscp_consume_byte(pData[i]);
    }
}