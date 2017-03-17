#ifndef CommandEssentials_h
#define CommandEssentials_h

#include "Command.h"

extern Command<int, int> cmd_pinMode;

extern Command<int, int> cmd_digitalWrite;
extern Command<int, int> cmd_analogWrite;

extern Command<int> cmd_digitalRead;
extern Command<int> cmd_analogRead;

extern Command<> cmd_restart;

extern Command<int> cmd_echo;
extern Command<int> cmd_verbose;
extern Command<> cmd_clearScreen;

#endif