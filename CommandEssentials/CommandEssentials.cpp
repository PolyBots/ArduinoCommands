#include "CommandEssentials.h"
#include "Arduino.h"

Command<int, int> cmd_pinMode(
	"pinMode", [](int pin, int mode)
	{
		pinMode(pin, mode);
	}
);

Command<int, int> cmd_digitalWrite(
	"digitalWrite", [](int pin, int value)
	{
		digitalWrite(pin, value);
	}
);

Command<int, int> cmd_analogWrite(
	"analogWrite", [](int pin, int value)
	{
		analogWrite(pin, value);
	}
);

Command<int> cmd_digitalRead(
	"digitalRead", [](int pin)
	{
		if(digitalRead(pin)) command.println("HIGH");
		else command.println("LOW");
	}
);

Command<int> cmd_analogRead(
	"analogRead", [](int pin)
	{
		command.println(analogRead(pin));
	}
);

Command<> cmd_reset(
	"reset", []()
	{
		void(*reset)(void) = 0;
		cmd_clearScreen();
		reset();
	}
);

Command<> cmd_clearScreen(
	"clearScreen", []()
	{
		//place a bunch of new lines in case the monitor in question does not support the form feed character (e.g. Arduino Serial Monitor)
		for(int i = 0; i < 16; ++i) command.print("\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n");

		command.print(12);	// form feed ASCII character
							// not supported on Arduino Serial Monitor
							// but is supported on others like PuTTY
	}
);