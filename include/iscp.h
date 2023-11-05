/*******************************************************************************
 * @file iscp.h
 *
 * @author Clemens Haffner
 *
 * @brief ISCP Serial Reception
 * 
 * Reception / Transmission of Serial ISCP Commands via UART.
 *
 * @version 0.1
 * @date 2023-11-05
 *
 ******************************************************************************/

#ifndef ISCP_H
#define ISCP_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

// Higher Layer Interface
int iscp_send_message(char pCommand[]);
void iscp_on_message(char pMessage[]); // Callback.

// LL Interface.
void iscp_consume_byte(uint8_t u8Byte); // Call from Rx Interrupt
int iscp_send_data(uint8_t *pData, uint16_t u16Len); // Callback. Needs to implement Tx

#ifdef __cplusplus
}
#endif
#endif /* ISCP_H */
