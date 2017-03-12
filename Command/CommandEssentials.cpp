#include "CommandEssentials.h"
#include "arduino.h"

extern Command<unsigned char, unsigned char> cmd_pinMode(
	"pinMode", [](unsigned char pin, unsigned char mode)
	{
		pinMode(pin, mode);
	}
);

extern Command<unsigned char, unsigned char> cmd_digitalWrite(
	"digitalWrite", [](unsigned char pin, unsigned char value)
	{
		digitalWrite(pin, value);
	}
);

extern Command<unsigned char, unsigned char> cmd_analogWrite(
	"analogWrite", [](unsigned char pin, unsigned char value)
	{
		analogWrite(pin, value);
	}
);

extern Command<unsigned char> cmd_digitalRead(
	"digitalRead", [](unsigned char pin)
	{
		command.println(digitalRead(pin));
	}
);

extern Command<unsigned char> cmd_analogRead(
	"analogRead", [](unsigned char pin)
	{
		command.println(analogRead(pin));
	}
);

extern Command<> cmd_restart(
	"restart", []()
	{
		// best implementation not yet decided
		// asm volatile("jmp 0");
	}
);

extern Command<unsigned char> cmd_echo(
	"echo", [](unsigned char state)
	{
		command.echo = state;
	}
);

extern Command<unsigned char> cmd_verbose(
	"verbose", [](unsigned char state)
	{
		// command.verbose not yet implemented
		// command.verbose = state;
	}
);

extern Command<> cmd_clearScreen(
	"clearScreen", []()
	{
		// best implementation not yet decided
		// command.print(12); // form feed ASCII character
	}
);