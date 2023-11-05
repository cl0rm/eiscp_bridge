/*******************************************************************************
 * @file tcp_server.cpp
 *
 * @author Clemens Haffner
 *
 * @brief TCP Server for eISCP, using Arduino ESP8266 Wifi API.
 * 
 * Should be easy to port for ESP32
 *
 * @version 0.1
 * @date 2023-11-05
 *
 ******************************************************************************/

#include "tcp_server.h"
#include <stdint.h>

#include <ESP8266WiFi.h>
#include <Arduino.h>

#include "control.h"

static WiFiServer Server(60128);
static WiFiClient Connection;

static uint8_t RxBuffer[256];
static int bConnected;

int tcp_init(void)
{
    Server.begin();
    return 0;
}

void tcp_task(void)
{
    int iNewBytes;
    
    if (bConnected == 0)
    {
        Connection = Server.available();
        if(Connection)
        {
            bConnected = 1;
        }
    }
    
    if(bConnected)
    {
        if(Connection.connected())
        {
            iNewBytes = Connection.available();
            if(iNewBytes > 0)
            {
                Connection.readBytes(RxBuffer, iNewBytes);
                control_tcp_rx(RxBuffer, iNewBytes);
            }
        }
        else
        {
            bConnected = 0;
            Connection.stop();
        }
    }
}

int tcp_send(uint8_t *pData, uint16_t u16Len)
{
    if(bConnected)
    {
        if(Connection.connected())
        {
            return Connection.write(pData, u16Len);
        }
        return -2;
    }
    return -1;
}