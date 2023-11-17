/*!
 * @file turbo.cpp
 *
 * @mainpage Pfeiffer Turbo Library
 *
 * @section intro_sec Introduction
 *
 * This is a library for working with Pfeiffer TC110
 * turbopump controllers
 *
 * @section author Author
 *
 * Written by Brett Longworth
 *
 * @section license License
 *
 * BSD license, all text above must be included in any redistribution
 */

#include <Arduino.h>
#include "turbo.h"

Turbo::Turbo()
{
}

void Turbo::begin() {
}

void Turbo::start() {
  userial.write("001 10 023 06 111111 019\r");
  delay(250);
  const int BUFFER_SIZE_T = 30;
  char VarT[BUFFER_SIZE_T];
  char VarTOut[6];
  digitalWrite(13, HIGH);
  rlen = userial.readBytesUntil(13, VarT, BUFFER_SIZE_T);
  for (int i = 10; i < 16; ++i) {
    VarTOut[i - 10] = VarT[i];
  }
  String s = String(VarTOut);
  //  Serial.println(s);

  userial.write("0011001006111111015\r");
  delay(250);
  // for loop from a to b
  rlen = userial.readBytesUntil(13, VarT, BUFFER_SIZE_T);
  for (int i = 10; i < 16; ++i) {
    VarTOut[i - 10] = VarT[i];
  }
  s = String(VarTOut);
  //  Serial.println(s);
  digitalWrite(13, LOW);
}

void Turbo::stop(){
  userial.write("0011001006000000009\r");
  delay(250);
  // for loop from a to b
  const int BUFFER_SIZE_T = 30;
  char VarT[BUFFER_SIZE_T];
  char VarTOut[6];
  int rlen = userial.readBytesUntil(13, VarT, BUFFER_SIZE_T);
  for ( int i = 10; i < 16; ++i )
    VarTOut[i - 10] = VarT[ i ];
  String s = String(VarTOut);
}
  
void USB_serial_stuff(){
    myusb.Task();
  // Print out information about different devices.
  for (uint8_t i = 0; i < CNT_DEVICES; i++) {
    if (*drivers[i] != driver_active[i]) {
      if (driver_active[i]) {
        Serial.printf("*** Device %s - disconnected ***\n", driver_names[i]);
        driver_active[i] = false;
      } else {
        Serial.printf("*** Device %s %x:%x - connected ***\n", driver_names[i], drivers[i]->idVendor(), drivers[i]->idProduct());
        driver_active[i] = true;

        const uint8_t *psz = drivers[i]->manufacturer();
        if (psz && *psz) Serial.printf("  manufacturer: %s\n", psz);
        psz = drivers[i]->product();
        if (psz && *psz) Serial.printf("  product: %s\n", psz);
        psz = drivers[i]->serialNumber();
        if (psz && *psz) Serial.printf("  Serial: %s\n", psz);

        // If this is a new Serial device.
        if (drivers[i] == &userial) {
          // Lets try first outputting something to our USerial to see if it will go out...
          userial.begin(baud);

        }
      }
    }
  }
}

void Turbo::setSpeed(int TB_Spd4) {
  Serial.print("Change Speed ...");
  Serial.print(TB_Spd4);
  Serial.println("Hz");
  float m = TB_Spd4;
  int sp = (m / 1500) * 10000;
  char spC[13];
  char spdMsg[34];
  sprintf(spC, "%06d", sp);
  int sum = 22;
  for (int i = 0; i < 6; i++) {
    sum += spC[i] - '0';
  };
  sprintf(spdMsg, "0011070706%06d%03d", sp, sum);
  Serial.println(spdMsg);
  userial.write(spdMsg);
  userial.write("\r");
  // acknowledge change
  const int BUFFER_SIZE = 30;
  char Var1[BUFFER_SIZE];
  char VarOut[6];
  int rlen = userial.readBytesUntil(13, Var1, BUFFER_SIZE);
  for (int i = 10; i < 16; ++i) {
    VarOut[i - 10] = Var1[i];
  }
  String s = String(VarOut);
  Serial.println(s);

  Serial.println("Rotation speed setting mode ...");
  userial.write("0011002607001018\r");
  delay(250);
  // for loop from a to b
  rlen = userial.readBytesUntil(13, Var1, BUFFER_SIZE);
  for (int i = 10; i < 16; ++i) {
    VarOut[i - 10] = Var1[i];
  }
  s = String(VarOut);
  Serial.println(s);
  Serial.println("Rotation speed updated");
}

int Turbo::readStatus (char *x, int a, int b) { // function declaration {
  userial.write(x);
  delay(50);
  // for loop from a to b
  const int BUFFER_SIZE = 30;
  char Var1[BUFFER_SIZE];
  char VarOut[6];
  int rlen = userial.readBytesUntil(13, Var1, BUFFER_SIZE);
  for ( int i = a; i < b; ++i ) {
    VarOut[i - a] = Var1[ i ];
  }
  String s = String(VarOut);
  int VarNum = s.toFloat();
  //  Serial.println(s);
  return VarNum; // return the value
}

void Turbo::getStatusTurboA(int ST[]) {
  // ErrorCode
  ST[0] = readStatus("0010030302=?101\r", 10, 16);
  //  SetRotSpd
  //  ST[1] = readStatus("0010030802=?106\r", 10, 16);
  //  ActualSpd
  ST[2] = readStatus("0010030902=?107\r", 10, 16);
  //  NominalSpd
  //  ST[3] = readStatus("0010031502=?104\r", 10, 16);
  //  DrvPower
  ST[4] = readStatus("0010031602=?105\r", 10, 16);
  //  //  DrvCurrent
  //  ST[5] = readStatus("0010031002=?099\r", 10, 16);
  //  DrvVoltage
  ST[6] = readStatus("0010031302=?102\r", 10, 16);
  //  //  TempElec
  ST[7] = readStatus("0010032602=?106\r", 10, 16);
  //  TempPmpBot
  ST[8] = readStatus("0010033002=?101\r", 10, 16);
  //  //  TempMotor
  ST[9] = readStatus("0010034602=?108\r", 10, 16);
}

void Turbo::getStatusTurboB(int ST[]) {
  // ErrorCode
  ST[0] = readStatus("0010030302=?101\r", 10, 16);
  //  ActualSpd
  ST[1] = readStatus("0010030902=?107\r", 10, 16);
  //  DrvPower
  ST[2] = readStatus("0010031602=?105\r", 10, 16);
}

int Turbo::check(int TB_Spd1) {
  int m;
  m=TB_Spd1-50;
  getStatusTurboB(Status_Turbo_B);
  
  Serial.print("Error:");
  Serial.print(Status_Turbo_B[0]);
  Serial.println(" ");

  Serial.print("NominalSpd (");
  Serial.print(m);
  Serial.print("Hz):");
  Serial.print(Status_Turbo_B[1]);
  Serial.println("Hz");

  Serial.print("DrvPower:");
  Serial.print(Status_Turbo_B[2]);
  Serial.println("W");

  int T = 0;
  if (Status_Turbo_B[0] == 0 && Status_Turbo_B[1] > m && Status_Turbo_B[2] < 15) {
    T = 1;
    //    Serial.println("Set turbo check value to one");
  };
  //  Serial.print("T:");
  //  Serial.println(T);
  return T;
}
