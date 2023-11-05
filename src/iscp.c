/*******************************************************************************
 * @file iscp.c
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

#include <stdint.h>
#include <string.h>
#include "iscp.h"

typedef enum tState_e
{
    S_SEARCH_EXCLAMATION,
    S_IN_PACK,
} tState;

/* Reception Variables */
static tState state;

static char RxBuffer[257]; //+1 for null termination
static uint8_t RxBufferPos;

void iscp_consume_byte(uint8_t u8Byte)
{
    switch (state)
    {
    case S_SEARCH_EXCLAMATION:
        RxBufferPos = 0;
        RxBuffer[RxBufferPos++] = (char)u8Byte;
        state = (u8Byte == '!') ? S_IN_PACK : S_SEARCH_EXCLAMATION;
        return;
    case S_IN_PACK:
        // finally... HERE COME THE DATA!
        if(RxBufferPos < (sizeof(RxBuffer) - 1))
        {
            RxBuffer[RxBufferPos++] = (char)u8Byte;
            if((u8Byte == '\r') || (u8Byte == '\n') || (u8Byte == 0x1A)) // 0x1A is ACII <EOF>
            {
                // One Last (important) thing. We Null Terminate our Message
                // Not needed for serial <-> ETH bridge but if we want to parse the data.
                RxBuffer[RxBufferPos] = '\0';
                iscp_on_message(RxBuffer);
                state = S_SEARCH_EXCLAMATION;
            }
        }
        else
        {
            // Too many Data! Can not be a valid frame
            state = S_SEARCH_EXCLAMATION;
        }
        return;
    default:
        // Should never happen ... State machine corrupted!
        state = S_SEARCH_EXCLAMATION;
        break;
    }
}

int iscp_send_message(char pCommand[])
{
    return iscp_send_data((uint8_t *)pCommand, strlen(pCommand));
}
