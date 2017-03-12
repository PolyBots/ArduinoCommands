#include "CommandEssentials.h"
#include "Arduino.h"

Command<unsigned char, unsigned char> cmd_pinMode(
	"pinMode", [](unsigned char pin, unsigned char mode)
	{
		pinMode(pin, mode);
	}
);

Command<unsigned char, unsigned char> cmd_digitalWrite(
	"digitalWrite", [](unsigned char pin, unsigned char value)
	{
		digitalWrite(pin, value);
	}
);

Command<unsigned char, unsigned char> cmd_analogWrite(
	"analogWrite", [](unsigned char pin, unsigned char value)
	{
		analogWrite(pin, value);
	}
);

Command<unsigned char> cmd_digitalRead(
	"digitalRead", [](unsigned char pin)
	{
		command.println(digitalRead(pin));
	}
);

Command<unsigned char> cmd_analogRead(
	"analogRead", [](unsigned char pin)
	{
		command.println(analogRead(pin));
	}
);

Command<> cmd_restart(
	"restart", []()
	{
		// best implementation not yet decided
		// asm volatile("jmp 0");
	}
);

Command<unsigned char> cmd_echo(
	"echo", [](unsigned char state)
	{
		command.echo = state;
	}
);

Command<unsigned char> cmd_verbose(
	"verbose", [](unsigned char state)
	{
		command.verbose = state;
	}
);

Command<> cmd_clearScreen(
	"clearScreen", []()
	{
		// best implementation not yet decided

		// command.print(12);	// form feed ASCII character
								// not supported on Arduino Serial Monitor
								// but is supported on others like PuTTY

		// supported on all serial terminals, but doesn't look as nice
		// for(int i = 0; i < 64; ++i) command.print("\r\n");
	}
);