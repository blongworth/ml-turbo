/** Read Nortec Vector ADV serial packets */

#include <Arduino.h>
#include "turbo.h"

Turbo turbo;

void setup() {
  Serial.begin(9600);
  delay(3000);
  Serial.println("Teensy ready");
  Serial.println("Starting Turbo...");
  turbo.begin();
}

void loop() {
}