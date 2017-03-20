#include <CommandSerial.h>

/*
 Command Basics
 by Kenan Millet
 NYU PolyBOTS, 2017

 Demonstrates how to use Quick Command Setup for the Serial
 Monitor with the Command Library
*/

void setup() {
  //Setting up Serial Monitor/Command interface
  //with Serial Monitor Baud Rate set to 9600
  //bits per second
  commandSerialSetup();
}

void loop() {
  //intercepts commands and executes them
  command.hook();
}