/*
 * Embedded Home Sentry System
 *
 * Implementation of the Home Sentry System to be embedded on the sensors.
 *
 * This is the basic implementation, with only the temperature, humidity, light sensors and an IR transmitter
 */

#include "Arduino.h"
#include "Dht11.h"
#include <SPI.h>
#include <Mirf.h>
#include <MirfSpiDriver.h>
#include <MirfHardwareSpiDriver.h>
#include <nRF24L01.h>
#include "EmbeddedHSS_Basic.h"


/*
 *
 * ==================== Functions and Methods ====================
 *
 */

void executeCommand(Command &message) {
  switch(message.command) {
    case CMD_TESTCOM:
      digitalWrite(3, HIGH); // Quick test, lights up an LED
      break;
    case CMD_REQ_SENSOR:
      digitalWrite(3, LOW); // Quick test, turns the LED off
      break;
    default:
    break;
  }
}

void setup() {
  Serial.begin(SERIAL_BAUD);
  Serial.print("Starting Sensor Unit");
  
  // Configure the NRF24L01 chip
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  Mirf.setRADDR((byte *)"U_001"); // This unit's ID
  Mirf.setTADDR((byte *)"U_000"); // The server ID (always 0)
  Mirf.payload = sizeof(Command);
  Mirf.config();
}

void loop() {
  unsigned long timeNow = millis();
  // Read sensors if poll delay has passed
  if (timeNow >= lastPoll + POLL_DELAY) {
    lastPoll = timeNow;
    int lightRead = analogRead(LIGHT_DATA_PIN);
    if (tempSensor.read() == Dht11::OK && lightRead < 1023) {
      lastSuccessfulPoll = timeNow;
      currentTemperature = tempSensor.getTemperature();
      currentHumidity = tempSensor.getHumidity();
      currentLight = lightRead;
    }
    
    if (Mirf.dataReady()) {
      Command *message = getData();
      executeCommand(*message);
      free(message);
    }
  }
}
