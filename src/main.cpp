/*******************************************************************************
 * @file main.cpp
 *
 * @author Clemens Haffner
 *
 * @brief Main for eISCP bridge
 * 
 * This implements a simple Bridge from Serial to WiFi.
 * 
 * However not a usual one, but a bridge that is compatible with Onkyos own eISCP
 * protocol. That way, an application that is designed to control Onkyo Network
 * Equipment should also work with this bridge. This includes auto-Discovery.
 * 
 * It can also be used to enable WiFi on Units that only have ethernet.
 *
 * @version 0.1
 * @date 2023-11-05
 *
 ******************************************************************************/

#include <Arduino.h>
#include <WiFiManager.h>

#include "tcp_server.h"
#include "udp_server.h"
#include "serial.h"

static WiFiManager wifiManager;

void setup() 
{
  serial_init();

  // TODO: Check if an AVR is connected and query Model Number
  Serial.begin(115200);
  Serial.println("--------- APP BOOTED ---------");

  // Connect to Wi-Fi
  wifiManager.autoConnect("eISCP_bridge");

  tcp_init();
  udp_init();
}

void loop()
{
  udp_task();
  tcp_task();
  serial_task();
}