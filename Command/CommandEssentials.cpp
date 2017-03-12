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
		command.println(digitalRead(pin));
	}
);

Command<int> cmd_analogRead(
	"analogRead", [](int pin)
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

Command<int> cmd_echo(
	"echo", [](int state)
	{
		command.echo = state;
	}
);

Command<int> cmd_verbose(
	"verbose", [](int state)
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