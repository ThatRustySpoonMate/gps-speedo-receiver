#include <Arduino.h>
#include "spi.h"
#include <RadioLib.h>

#define CS      18     // GPIO18 -- SX1278's CS
#define RST     14     // GPIO14 -- SX1278's RESET
#define DI0     26     // GPIO26 -- SX1278's IRQ(Interrupt Request)
#define BUTTON_PIN 38  // Pin between PWR and RST 
#define LED_PIN 4


/* Comms Protocol */

typedef struct {
  //int packet_id;

  float speed;
  float hdop;
  uint32_t sats;

} LoraMessage_t;

/* Global variable defs */
SX1276 radio = new Module(CS, DI0, RST);

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);

  // initialize SX1278 with default settings
  Serial.print(F("[SX1276] Initializing ... "));
  int state = radio.begin();
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true) { delay(10); }
  }

  // LoRaRate_t config;
  // config.spreadingFactor = 6; // 6-12
  // config.bandwidth = 250.0;
  // config.codingRate = 
  radio.setBandwidth(250.0);
  radio.setSpreadingFactor(6);
  radio.setFrequency(915.0);


}

void loop() {
  int receiveResult = RADIOLIB_ERR_NONE;

  LoraMessage_t packet;
  receiveResult = radio.receive((uint8_t *)&packet, sizeof(packet));
  


  if (receiveResult == RADIOLIB_ERR_NONE) {
    Serial.println("Success! Received:");
    Serial.println("SATS: " + String(packet.sats));
    Serial.println("SPEED: " + String(packet.speed));
    Serial.println("HDOP: " + String(packet.hdop));

    Serial.print(F("RSSI:"));
    Serial.print(radio.getRSSI());
    Serial.println(F(" dBm\n"));
    

  }
  // else if (receiveResult == RADIOLIB_ERR_RX_TIMEOUT) {
  //   // timeout occurred while waiting for a packet
  //   Serial.println(F("timeout!"));

  // } else if (receiveResult == RADIOLIB_ERR_CRC_MISMATCH) {
  //   // packet was received, but is malformed
  //   Serial.println(F("CRC error!"));

  // } else {
  //   // some other error occurred
  //   Serial.print(F("failed, code "));
  //   Serial.println(receiveResult);

  // }



}

