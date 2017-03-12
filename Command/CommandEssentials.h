#ifndef CommandEssentials_h
#define CommandEssentials_h

#include "Command.h"

extern Command<unsigned char, unsigned char> cmd_pinMode;

extern Command<unsigned char, unsigned char> cmd_digitalWrite;
extern Command<unsigned char, unsigned char> cmd_analogWrite;

extern Command<unsigned char> cmd_digitalRead;
extern Command<unsigned char> cmd_analogRead;

extern Command<> cmd_restart;

extern Command<unsigned char> cmd_echo;
extern Command<unsigned char> cmd_verbose;
extern Command<> cmd_clearScreen;

#endif