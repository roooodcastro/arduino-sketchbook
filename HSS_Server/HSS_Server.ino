/*
 * Server Home Sentry System
 *
 * Implementation of the Home Sentry System main Arduino server.
 *
 * This will connect and communicate to all sensor units spread around the house, and also with the remote internet server to update sensor information and get commands to commandable units
 */

#include <SPI.h>
#include <Mirf.h>
#include <MirfSpiDriver.h>
#include <MirfHardwareSpiDriver.h>
#include <nRF24L01.h>
#include "HSS.h"


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
  Serial.begin(9600);
  Serial.println("Starting Server...");
  
  units = (Unit*) malloc(INITIAL_UNIT_ARRAY_SIZE * sizeof(Unit));
  
  // Configure the NRF24L01 chip
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  Mirf.setRADDR((byte *)"U_000"); // The server ID (always 0)
  Mirf.payload = sizeof(Command);
  Mirf.config();
}

void loop() {
  unsigned long timeNow = millis();
  // Read sensors if poll delay has passed
    
  if (Mirf.dataReady()) {
    Command *message = getMessage();
    executeCommand(*message);
  }
}
