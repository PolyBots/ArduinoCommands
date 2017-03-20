#include "CommandSerial.h"
#include "Arduino.h"

void commandSetupSerial(usigned int baud)
{
	// Set up UART (Serial) monitor at 9600 baud, 8n1:
	Serial.begin(baud);

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
}