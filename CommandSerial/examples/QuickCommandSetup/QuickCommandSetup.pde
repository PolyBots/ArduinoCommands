#include <CommandSerial.h>

/*
 Command Serial
 by Kenan Millet
 NYU PolyBOTS, 2017

 Demonstrates how to use Quick Command Setup for the Serial
 Monitor with the Command Library

 NOTE: In order to use this library, you must have the Command Library installed.
*/

void setup() {
	//NOTE: Please ensure that you understand how commands work before using them.
	//For more details, look at the CommandBasics example in the Command Library.
	new Command<> ("Test"
	[](){
		command.println("Hello World!")
	});


	//Setting up Serial Monitor/Command interface
	//with Serial Monitor Baud Rate set to 9600
	//bits per second
	commandSerialSetup();
}

void loop() {
	//intercepts commands and executes them
	command.hook();
}