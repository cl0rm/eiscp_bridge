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
static uint8_t TxBuf[256];
static int iLenDiscoveryResponse;

int udp_init(void)
{
    // Get the MAC Address
    String sMAC = WiFi.macAddress();
    char sResponse[80];
    // Remove colons
    sMAC.remove(2, 1); // xx:xx:xx:xx:xx:xx
    sMAC.remove(4, 1);
    sMAC.remove(6, 1);
    sMAC.remove(8, 1);
    sMAC.remove(10, 1);

    // Form response in advance!
    snprintf(sResponse, 80, "!1ECN%s/%05d/%s/%s\x19\r\n", MODEL_NAME, 60128, REGIONAL_ID, sMAC.c_str());
    iLenDiscoveryResponse = eiscp_build_packet(sResponse, TxBuf, sizeof(TxBuf));

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
                Serial.print("[UDP] received bcast from ");
                Serial.print(UdpServer.remoteIP());
                Serial.print(" at port ");
                Serial.println(UdpServer.remotePort());
                if(RxBuffer[16] == '!')
                {
                    if(strnstr((char *)&RxBuffer[18],"ECNQSTN",16))
                    {                       
                        if(UdpServer.beginPacket(UdpServer.remoteIP(), UdpServer.remotePort()) == 1)
                        {
                            UdpServer.write(TxBuf, iLenDiscoveryResponse);
                            if(UdpServer.endPacket() == 1)
                            {
                                Serial.println("[UDP] response sent!");
                            }
                            else
                            {
                                Serial.println("[UDP] error sending response");
                            }

                        }
                        else
                        {
                            Serial.println("[UDP] error beginPacket()");
                        }
                    }
                }
            }
        }
    }
}