#include "CommandSerial.h"
#include "Arduino.h"

void commandSerialSetup(unsigned int baud)
{
	// Set up UART (Serial) monitor at set baud rate
	Serial.begin(baud);

	// Register error handler with the Command interface
	command.setErrorHandler(
		[](const char* s){
			command.streamWrite("ERROR: ");
			command.streamWrite("\"");
			command.streamWrite(s);
			command.streamWrite("\" is not a valid command!\n");
		}
	);

	// Register stream availability function with the Command interface
	command.setStreamAvail([](){return Serial.available();});

	// Register stream read function with the Command interface
	command.setStreamRead([](){return Serial.read();});

	// Register stream write function with the Command interface
	command.setStreamWrite([](const char* s){Serial.write(s);});
}