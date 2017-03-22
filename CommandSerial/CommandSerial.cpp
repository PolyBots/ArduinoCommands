#include "CommandSerial.h"
#include "Arduino.h"

void commandSerialSetup(unsigned int baud)
{
	// Set up UART (Serial) monitor at set baud rate
	Serial.begin(baud);

	// Register error handler with the Command interface
	command.setErrorHandler(
		[](const char* s){
			command.print("ERROR: ");
			command.print("\"");
			command.print(s);
			command.println("\" is not a valid command!");
		}
	);

	// Register stream availability function with the Command interface
	command.setStreamAvail([](){return Serial.available();});

	// Register stream read function with the Command interface
	command.setStreamRead([](){return Serial.read();});

	// Register stream write function with the Command interface
	command.setStreamWrite([](const char* s){return Serial.write(s);});
}