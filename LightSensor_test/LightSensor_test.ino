/*
 * LightSensor_test.ino
 *
 * Example implementation to test the light sensor.
 *
 * Using a 47k resistor, light level goes from 0 to 1024, staying at 50 on a dark room, and around 200 on a dim room
 */
#include "Arduino.h"

enum {
    // The analog data I/O pin connected to the light sensor
    LIGHT_DATA_PIN = 0,

    // The baud rate of the serial interface
    SERIAL_BAUD = 9600,

    // The delay between sensor polls.
    POLL_DELAY = 500,
};

void setup() {
  Serial.begin(SERIAL_BAUD);
  Serial.print("Starting light sensor");
}

void loop() {
  int lightRead = analogRead(LIGHT_DATA_PIN);
  Serial.print("Light level: ");
  Serial.println(lightRead);
  delay(POLL_DELAY);
}
