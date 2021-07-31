/*
 *  _                                   ____                       
 * | |    __ _  ___ _   _ _ __   __ _  / ___| _ __   __ _  ___ ___ 
 * | |   / _` |/ __| | | | '_ \ / _` | \___ \| '_ \ / _` |/ __/ _ \
 * | |__| (_| | (__| |_| | | | | (_| |  ___) | |_) | (_| | (_|  __/
 * |_____\__,_|\___|\__,_|_| |_|\__,_| |____/| .__/ \__,_|\___\___|
 *                                           |_|                   
 * Copyright (C) 2019 Lacuna Space Ltd.
 *
 * Description: Simple LoRaWAN example sketch
 * 
 * License: Revised BSD License, see LICENSE-LACUNA.TXT file included in the project
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef REGION
#define REGION R_EU868
#endif

// Interval between transmissions
#define TX_INTERVAL 20

#include <LibLacuna.h>
#include <SPI.h>

// Keys and device address are MSB
static byte networkKey[] = { 
    // Replace with your network key
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
static byte appKey[] = {
    // Replace with your application key
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
// Replace with your device address
static byte deviceAddress[] = { 0x01, 0x02, 0x03, 0x04 };

static char payload[255];
const String mytext = "test message";

static lsLoraWANParams loraWANParams;
static lsLoraSatTxParams SattxParams;
static lsLoraTxParams txParams;

void setup() {
  Serial.begin(9600);

  pinMode(LS_LED_BLUE, OUTPUT);

  pinMode(LS_GPS_ENABLE, OUTPUT);
  digitalWrite(LS_GPS_ENABLE, LOW);
  pinMode(LS_GPS_V_BCKP, OUTPUT);
  digitalWrite(LS_GPS_V_BCKP, LOW);

  while (!Serial && millis() < 3000);
  
  Serial.println("Initializing");

  Serial.print("Configured Region: ");
  #if REGION == R_EU868
    Serial.println("Europe 862-870 Mhz");
  #elif REGION  == R_US915
    Serial.println("US 902-928 Mhz");
  #elif REGION == R_AS923
    Serial.println("Asia 923 Mhz");
  #elif REGION == R_IN865
    Serial.println("India 865-867 Mhz");
  #else 
    Serial.println("Undefined");  
  #endif

  // SX1262 configuration for lacuna LS200 board
  lsSX126xConfig cfg;
  lsCreateDefaultSX126xConfig(&cfg, BOARD_VERSION);

  // Initialize SX1262
  int result = lsInitSX126x(&cfg, REGION);
  Serial.print("E22/SX1262: ");
  Serial.println(lsErrorToString(result));

  // LoRaWAN session parameters
  lsCreateDefaultLoraWANParams(&loraWANParams, networkKey, appKey, deviceAddress);
  loraWANParams.txPort = 1;
  loraWANParams.rxEnable = true;
 
  // transmission parameters for Lacuna satellites
  lsCreateDefaultLoraSatTxParams(&SattxParams, REGION);

  // transmission parameters for terrestrial LoRa
  lsCreateDefaultLoraTxParams(&txParams, REGION);
  txParams.spreadingFactor = lsLoraSpreadingFactor_7;

  txParams.frequency = 922000000; //922MHz

  Serial.print("Terrestrial Uplink Frequency: ");
  Serial.println(txParams.frequency/1e6);
 
}

void loop() {

  digitalWrite(LS_LED_BLUE, HIGH);
  delay(50);
  digitalWrite(LS_LED_BLUE, LOW);
  delay(100);
  digitalWrite(LS_LED_BLUE, HIGH);
  delay(50);
  digitalWrite(LS_LED_BLUE, LOW);

  Serial.println("Sending LoRa message"); 
  mytext.toCharArray(payload, 255); 
  int lora_result  = lsSendLoraWAN(&loraWANParams, &txParams, (byte *)payload, mytext.length());

  if (loraWANParams.rxpayloadLength) {
    Serial.println("Downlink received");
    Serial.print("Length: ");
    Serial.println(loraWANParams.rxpayloadLength);
    Serial.print("Port: ");
    Serial.println(loraWANParams.rxPort);
    Serial.print("Frame Counter: ");
    Serial.println(loraWANParams.framecounter_down);
    Serial.print("SNR: ");
    Serial.println(txParams.snr);
    Serial.print("RSSI: ");
    Serial.println(txParams.rssi);
    Serial.print("SignalRSSI: ");
    Serial.println(txParams.signalrssi);
    Serial.print("Payload: ");
    
    for (char n = 0; n < loraWANParams.rxpayloadLength; n++)
        {
          Serial.print (payload[n],HEX);
          Serial.write (" ");
          
        }
    Serial.println();
  }
  Serial.print("Result: ");
  Serial.println(lsErrorToString(lora_result));
 
  // Uncomment to send satellite message
  //
  // delay(1000);
  // Serial.println("Sending LoraSat message");
  // mytext.toCharArray(payload, 255); 
  // int sat_result = lsSendLoraSatWAN(&loraWANParams, &SattxParams, (byte *)payload, mytext.length());
  // Serial.print("Result: ");
  // Serial.println(lsErrorToString(sat_result));

  // wait TX_INTERVAL seconds
  delay(TX_INTERVAL*1000);

}
