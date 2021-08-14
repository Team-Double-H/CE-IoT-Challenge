/*
    _                                   ____
   | |    __ _  ___ _   _ _ __   __ _  / ___| _ __   __ _  ___ ___
   | |   / _` |/ __| | | | '_ \ / _` | \___ \| '_ \ / _` |/ __/ _ \
   | |__| (_| | (__| |_| | | | | (_| |  ___) | |_) | (_| | (_|  __/
   |_____\__,_|\___|\__,_|_| |_|\__,_| |____/| .__/ \__,_|\___\___|
                                             |_|
   Copyright (C) 2019 Lacuna Space Ltd.

   Description: Simple LoRaWAN example sketch

   License: Revised BSD License, see LICENSE-LACUNA.TXT file included in the project

   SPDX-License-Identifier: BSD-3-Clause

*/

#ifndef REGION
#define REGION R_EU868
#endif

// Interval between transmissions
#define TX_INTERVAL 40

#include <LibLacuna.h>
#include <SPI.h>


static char payload[255];
String mytext;
int Count = 0;

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

  // transmission parameters for terrestrial LoRa
  lsCreateDefaultLoraTxParams(&txParams, REGION);
  txParams.frequency = 922000000;
  txParams.spreadingFactor = lsLoraSpreadingFactor_7;
  txParams.codingRate = lsLoraCodingRate_4_6;
  txParams.bandwidth = lsLoraBandwidth_500_khz;

  Serial.print("Terrestrial Uplink Frequency: ");
  Serial.println(txParams.frequency / 1e6);
}

void loop() {

  digitalWrite(LS_LED_BLUE, HIGH);
  delay(50);
  digitalWrite(LS_LED_BLUE, LOW);
  delay(100);
  digitalWrite(LS_LED_BLUE, HIGH);
  delay(50);
  digitalWrite(LS_LED_BLUE, LOW);
  Count++;

  Serial.println("Sending LoRa message");
  if (Count < 4) {
    mytext = "LED 1: Available";
  } else if (Count == 4) {
    mytext = "LED 1: Not Available";
    Count = 0;
  }
  mytext.toCharArray(payload, 255);
  int lora_result  = lsSendLora(&txParams, (byte *)payload, mytext.length(), false);
  Serial.print("Result: ");
  Serial.println(lsErrorToString(lora_result));

  // wait TX_INTERVAL seconds
  delay(TX_INTERVAL * 1000);
}
