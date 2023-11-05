// UDP Server for Auto-Discovery
// Listen on eISCP Port (Port 60128/UDP)

#include "udp_server.h"
#include <stdint.h>

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include <Arduino.h>
#include <WiFiManager.h>   

#define MODEL_NAME              "TX-SR705" // VLA max 64 letters.
#define REGIONAL_ID             "XX" // XX European/Asian, DX American, JJ Japanese -> 2 CHAR
#define IDENTIFIER              "133713371337" // Unique id or MAC, -> VLA max 12 CHAR

WiFiUDP UdpServer;

static uint8_t RxBuffer[128];

int udp_init(void)
{
    // Connect to Wi-Fi

    UdpServer.begin(60128);
    return 0;
}

void udp_task(void)
{
    int iPacketSize = UdpServer.parsePacket();
    if (iPacketSize)
    {
        int iLen = UdpServer.read(RxBuffer, sizeof(RxBuffer) - 1);
        if (iLen > 9) // Length of !xECNQSTN
        {
            // Terminate the received String
            RxBuffer[iLen] = '\0';

            // Check if this is the Status Question
            if(RxBuffer[0] == '!')
            {
                if(strnstr((char *)&RxBuffer[2],"ECNQSTN",16))
                {
                    // Form response
                    char sResponse[64];
                    snprintf(sResponse, 64, "!1ECN%s/%05d/%s/%s", MODEL_NAME, 60128, REGIONAL_ID, IDENTIFIER);

                    UdpServer.beginPacket(UdpServer.remoteIP(), UdpServer.remotePort());
                    UdpServer.write(sResponse);
                    UdpServer.endPacket();
                }
            }
        }
    }
}