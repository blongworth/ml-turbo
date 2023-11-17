/* Example usage of ml-turbo library */

#include <Arduino.h>
#include "turbo.h"

Turbo turbo;
char command;

void setup() {
  Serial.begin(9600);
  turbo.begin();
}

void loop() {
  // get and print turbo status every 5 seconds
  //turbo.status()

  // handle serial commands
  if (Serial.available())
  {
    Serial.read();
    if (command == 'S') //Start turbo
    {
      turbo.start();
    } else if (command == 'X') //Stop turbo
    {
      turbo.stop();
    } else if (command == '?') //Get status
    {
      turbo.status();
    }
  }
}