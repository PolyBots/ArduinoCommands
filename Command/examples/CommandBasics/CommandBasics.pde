#include <Command.h>

/*
 Command Basics
 by Kenan Millet
 NYU PolyBOTS, 2017

 Demonstrates how to create and use commands with the serial monitor

 NOTE: This library is very small and easy to use, but not that easy
 to learn. For easier to use versions of this library, install:
 CommandSerial
 CommandEssentials
*/

/*	=======================================================
	|| COMMAND SYNTAX (outside of setup)                 ||
	=======================================================
	|| INST = name of object                             ||
	|| ARGS = types of all arguments used in command     ||
	|| NAME = string that you will type to call command  ||
	|| ARGSLIST = types/names of arguments in command    ||
	|| BODY = body of command (same as a function body)  ||
	=======================================================

	Command<ARGS> INST(NAME,
		[](ARGSLIST){
			BODY
		}
	);

	=======================================================
	|| COMMAND SYNTAX (inside setup)                     ||
	=======================================================
	|| ARGS = types of all arguments used in command     ||
	|| NAME = string that you will type to call command  ||
	|| ARGSLIST = types/names of arguments in command    ||
	|| BODY = body of command (same as a function body)  ||
	=======================================================
	|| NOTE: This alternate syntax for commands will     ||
	|| take up memory differently than the above syntax  ||
	|| and if not managed properly, can lead to issues   ||
	|| (if curious, the issues are called memory leaks). ||
	||                                                   ||  
	|| DO NOT put this syntax in void loop() or in a     ||
	|| for/while loop unless you know what you are       ||
	|| doing (i.e. you know how the new/delete           ||
	|| operators work and how to dynamic allocation      ||
	|| works in C/C++).                                  ||
	=======================================================

	new Command<ARGS> (NAME,
		[](ARGSLIST){
			BODY
		}
	);
*/

//This command takes one argument (HIGH/LOW) and turns the 
//builtin LED on the Arduino on or off when the user types
//"led HIGH" or "led LOW" (without quotes)
//This command uses the traditional command syntax
Command<int> writeLEDCommand("led",
  [](int state){
  	digitalWrite(LED_BUILTIN, state);
  }
);

void setup() {

	//This command takes no arguments and prints "Hello World!"
	//back to the user when they type "test" (without quotes) into
	//the serial monitor.
	//This command uses the alternate command syntax
	new Command<> ("test",
		[](int pin, int state){
			Command.println("Hello World!");
		}
	);

	// Set up UART (Serial) monitor at 9600 baud, 8n1:
	Serial.begin(9600);

	// Register our error handler with the Command interface
	command.setErrorHandler(
		[](const char* s){
			command.print("ERROR: \"");
			command.print(s);
			command.println("\" is not a valid command!");
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
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, LOW); // Initialize the output to set LED off by default
}

void loop() {
  //intercepts commands and executes them
  command.hook();
}