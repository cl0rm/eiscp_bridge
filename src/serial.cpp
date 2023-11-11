#include "serial.h"

#include <stdint.h>
#include <Arduino.h>
#include <SoftwareSerial.h>
#include "control.h"

#define MAX3232_TX 5
#define MAX3232_RX 4

static uint8_t RxBuffer[256];

static SoftwareSerial Uart(MAX3232_RX, MAX3232_TX);

int serial_init(void)
{
    Uart.begin(9600);
    delay(1000); // Wait for INIT
    return 0;
}

void serial_task(void)
{
    // Get Number of Serial Bytes Available on Serial
    uint16_t u16NewBytes = Uart.available();

    if(u16NewBytes > 0)
    {
        Uart.readBytes(RxBuffer, u16NewBytes);
        control_serial_rx(RxBuffer, u16NewBytes);
    }
}

int serial_send(uint8_t *pData, uint16_t u16Len)
{
    return Uart.write(pData, u16Len);
}