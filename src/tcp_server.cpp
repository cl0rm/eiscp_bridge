// TCP Server
// Listen on eISCP Port (Port 60128/TCP)

#include "tcp_server.h"
#include <stdint.h>

#include <ESP8266WiFi.h>
#include <Arduino.h>
#include <WiFiManager.h>   

#include "control.h"

static const uint16_t port = 60128;  //Port number
WiFiServer Server(port);
WiFiClient Connection;
WiFiManager wifiManager;

static uint8_t RxBuffer[256];

int tcp_init(void)
{
    // Connect to Wi-Fi
    wifiManager.autoConnect("eISCP_bridge");

    Server.begin();
    return 0;
}

void tcp_task(void)
{
    uint16_t u16NewBytes;
    
    Connection = Server.available();

    if(Connection)
    {
        if(Connection.connected())
        {
            // Get Number of Serial Bytes Available on Serial
            u16NewBytes = Connection.available();

            if(u16NewBytes > 0)
            {
                Connection.readBytes(RxBuffer, u16NewBytes);
                control_serial_rx(RxBuffer, u16NewBytes);
            }
        }
    }
}

int tcp_send(uint8_t *pData, uint16_t u16Len)
{
    if(Connection)
    {
        if(Connection.connected())
        {
            return Connection.write(pData, u16Len);
        }
        return -2;
    }
    return -1;
}