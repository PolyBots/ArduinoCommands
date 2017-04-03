#include "CommandEssentials.h"
#include "Arduino.h"

Command<int, int> cmd_pinMode(
	"pinMode <P> [OUTPUT/INPUT/INPUT_PULLUP]",
	"Sets the pin P to the given mode.",
	[](int pin, int mode)
	{
		pinMode(pin, mode);
	}
);

Command<int, int> cmd_digitalWrite(
	"digitalWrite <P> [HIGH/LOW]",
	"Writes HIGH/LOW to the digital pin P.",
	[](int pin, int value)
	{
		digitalWrite(pin, value);
	}
);

Command<int, int> cmd_analogWrite(
	"analogWrite <P> <V>",
	"Writes the value V to the analog pin P.",
	[](int pin, int value)
	{
		analogWrite(pin, value);
	}
);

Command<int> cmd_digitalRead(
	"digitalRead <P>",
	"Prints the value (HIGH/LOW) of the digital pin P.",
	[](int pin)
	{
		if(digitalRead(pin)) command.println("HIGH");
		else command.println("LOW");
	}
);

Command<int> cmd_analogRead(
	"analogRead <P>",
	"Prints the value of the analog pin P.",
	[](int pin)
	{
		command.println(analogRead(pin));
	}
);

Command<> cmd_reset(
	"reset",
	"\"Soft\" resets the microcontroller. This will jump to the beginning of the program but will not reset internal registers.",
	[]()
	{
		void(*reset)(void) = 0;
		cmd_clearScreen();
		reset();
	}
);

Command<> cmd_clearScreen(
	"clearScreen",
	"Clears the screen that commands are written to.",
	[]()
	{
		//place a bunch of new lines in case the monitor in question does not support the form feed character (e.g. Arduino Serial Monitor)
		for(int i = 0; i < 16; ++i) command.print("\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n");

		command.print(12);	// form feed ASCII character
							// not supported on Arduino Serial Monitor
							// but is supported on others like PuTTY
	}
);