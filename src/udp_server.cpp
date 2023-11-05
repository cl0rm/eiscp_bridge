/*******************************************************************************
 * @file udp_server.cpp
 *
 * @author Clemens Haffner
 *
 * @brief UDP Auto-Discovery for eISCP
 *
 * @version 0.1
 * @date 2023-11-05
 *
 ******************************************************************************/

#include "udp_server.h"
#include <stdint.h>

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include "eiscp.h"

#define MODEL_NAME              "TX-SR705" // VLA max 64 letters.
#define REGIONAL_ID             "XX" // XX European/Asian, DX American, JJ Japanese -> 2 CHAR
//#define UNIQUE_ID               "133713371337" // Unique id or MAC, -> VLA max 12 CHAR

static WiFiUDP UdpServer;

static uint8_t RxBuffer[128];
static String sMAC;

int udp_init(void)
{
    // Get the MAC Address
    sMAC = WiFi.macAddress();
    // Remove colons
    sMAC.remove(2, 1); // xx:xx:xx:xx:xx:xx
    sMAC.remove(4, 1);
    sMAC.remove(6, 1);
    sMAC.remove(8, 1);
    sMAC.remove(10, 1);

    UdpServer.begin(60128);
    return 0;
}

void udp_task(void)
{
    int iPacketSize = UdpServer.parsePacket();
    if (iPacketSize)
    {
        int iLen = UdpServer.read(RxBuffer, sizeof(RxBuffer) - 1);
        if (iLen >= 16 + 9) // Length of !xECNQSTN
        {
            // Terminate the received String
            RxBuffer[iLen] = '\0';

            if (strcmp((char *)RxBuffer, "ISCP") == 0)
            {
                if(RxBuffer[16] == '!')
                {
                    if(strnstr((char *)&RxBuffer[18],"ECNQSTN",16))
                    {                       
                        // Form response
                        char sResponse[80];
                        uint8_t TxBuf[256];
                        int iLen2;

                        snprintf(sResponse, 80, "!1ECN%s/%05d/%s/%s\x1A", MODEL_NAME, 60128, REGIONAL_ID, sMAC.c_str());
                        iLen2 = eiscp_build_packet(sResponse, TxBuf, sizeof(TxBuf));
                        if (iLen2 > 0)
                        {
                            UdpServer.beginPacket(UdpServer.remoteIP(), UdpServer.remotePort());
                            UdpServer.write(TxBuf, iLen2);
                            UdpServer.endPacket();
                        }
                    }
                }
            }
        }
    }
}