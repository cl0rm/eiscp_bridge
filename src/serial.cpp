#include "serial.h"

#include <stdint.h>
#include <Arduino.h>
#include "control.h"


static uint8_t RxBuffer[256];

int serial_init(void)
{
    Serial.begin(9600);
    return 0;
}

void serial_task(void)
{
    // Get Number of Serial Bytes Available on Serial
    uint16_t u16NewBytes = Serial.available();

    if(u16NewBytes > 0)
    {
        Serial.readBytes(RxBuffer, u16NewBytes);
        control_serial_rx(RxBuffer, u16NewBytes);
    }
}

int serial_send(uint8_t *pData, uint16_t u16Len)
{
    return Serial.write(pData, u16Len);
}