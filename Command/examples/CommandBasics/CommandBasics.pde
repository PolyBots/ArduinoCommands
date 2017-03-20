#include <Command.h>

/*
 Command Basics
 by Kenan Millet, Abhimanyu Ghosh
 NYU PolyBOTS, 2017

 Demonstrates how to create and use commands with the serial monitor
*/

Command<int, int> command_digitalWrite("digitalWrite",
   //functionality of command goes here
  [](int pin, int state){
  	digitalWrite(pin, state);
  }
);

Command<int> command_writeLED("led",
   //functionality of command goes here
  [](int state){
  	digitalWrite(13, state);
  }
);

void setup() {
  // Set up UART (Serial) monitor at 9600 baud, 8n1:
  Serial.begin(9600);

  // Register our error handler with the Command interface
  command.setErrorHandler(
    [](const char* s){
      command.streamWrite("ERROR: ");
      command.streamWrite("\"");
      command.streamWrite(s);
      command.streamWrite("\" is not a valid command!\n");
    }
  );

  // Register our stream availability function with the Command interface
  command.setStreamAvail([](){return Serial.available();});

  // Register our stream read function with the Command interface
  command.setStreamRead([](){return Serial.read();});

  // Register our stream write function with the Command interface
  command.setStreamWrite([](const char* s){Serial.write(s);});

  // Perform some digital pin setup to use Pin 13 (Port B, Pin 5)
  // as an OUTPUT:
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW); // Initialize the output to set LED off by default
}

void loop() {
  //intercepts commands and executes them
  command.hook();
}