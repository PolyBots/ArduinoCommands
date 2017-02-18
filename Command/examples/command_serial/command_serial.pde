#include <Command.h>

// Command Serial
// by Kenan Millet

// Demonstrates how to create and use commands with the serial monitor 

// Created 18 February 2017

//Creating the command
Command myCommand("test",
	//functionality of command goes here
	[](){
		Serial.println("Hello World!");
	}
	);

void setup()
{
	//allows us to use serial monitor
	Serial.begin(9600);
}

void loop()
{
	//get input from monitor
	String input = Serial.readString();

	//see if command with input as name exists
	//if it does, it will run the command
	//if it doesn't, this if statement will print an error
	if(!Command::exec(input) & input != "")
	{
		Serial.print("[ERROR] '");
		Serial.print(input);
		Serial.println("' is not a recognized command.");
	}
}