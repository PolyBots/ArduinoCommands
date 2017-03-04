#include <Command.h>

/*
 Command Basics
 by Kenan Millet, Abhimanyu Ghosh
 NYU PolyBOTS, 2017

 Demonstrates how to create and use commands with the serial monitor
*/

//Creating a test command to print back to us
Command myCommand("test",
	//functionality of command goes here
	[](){
		Command::streamWrite("Hello World!\n");
	}
);

//LED 13 On command
Command on("on",
 	//functionality of command goes here
 	[](){
		digitalWrite(13, HIGH);
	}
);

//LED 13 Off command
Command off("off",
	//functionality of command goes here
	[](){
		digitalWrite(13, LOW);
	}
);

void setup()
{
	// Set up UART (Serial) monitor at 9600 baud, 8n1:
	Serial.begin(9600);

	// Register our error handler with the Command interface
	Command::setErrorHandler(
		[](const char* s){
			Command::streamWrite("ERROR: ");
			Command::streamWrite("\"");
			Command::streamWrite(s);
			Command::streamWrite("\" is not a valid command!\n");
		}
	);

	// Register our stream availability function with the Command interface
	Command::setStreamAvail([](){return Serial.available();});

	// Register our stream read function with the Command interface
	Command::setStreamRead([](){return Serial.read();});

	// Register our stream write function with the Command interface
	Command::setStreamWrite([](const char* s){Serial.print(s);});

	// Perform some digital pin setup to use Pin 13 (Port B, Pin 5)
	// as an OUTPUT:
	pinMode(13, OUTPUT);
	digitalWrite(13, LOW); // Initialize the output to set LED off by default
}

void loop()
{
	//intercepts commands and executes them
	Command::hook();
}