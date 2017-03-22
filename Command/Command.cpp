#include "Command.h"
#include <stdint.h>
#include <Arduino.h>

//deletes the stored lambda and unregisters the command
Command<>::~Command()
{
	deleteLambda(lambda);
	
	Command::Node* n = registry;
	while(n->next != entry) n = n->next;
	n->next = n->next->next;
}

//pretty straight forward...
Command<>::Node::Node(Command<>* cmd, Command<>::Node* next)
	: cmd(cmd), next(next)
{
	
}

Command<>::Node* Command<>::registry = nullptr;
Command<>::Node** Command<>::back = nullptr;
const size_t Command<>::bufferSize = 64;
bool Command<>::echo = true;
bool Command<>::verbose = true;

//Searches through the registry for a nametag that matches "cmdName".
//If found, runs the command and returns true. If no match is found,
//returns false.
bool Command<>::exec(const char* cmdstr)
{
	char cmdbuf[bufferSize];
	strcpy(cmdbuf, cmdstr);
	//print back command if echo is on
	if(echo)
	{
		streamWrite("> ");
		streamWrite(cmdbuf);
	}

	char* name = strtok(cmdbuf, " (),\r\n");

	for(Command<>::Node* n = registry; n != nullptr; n = n->next)
	{
		if(!strcmp(name, n->cmd->nametag))
		{
			n->cmd->runLambda(n->cmd->lambda);
			return true;
		}
	}
	if(verbose) Command<>::errorHandler(name);
	return false;
}


//NOTE: INTENDED TO BE SET UP SO THAT USERS CAN ADD KEYWORDS
//WITHOUT MODIFYING HEADER FILE
int Command<>::convertArgKeyword(const char* s)
{
	if(!strcmp(s, "OFF")) return false;
	if(!strcmp(s, "LOW")) return LOW;
	if(!strcmp(s, "false")) return false;

	if(!strcmp(s, "ON")) return true;
	if(!strcmp(s, "HIGH")) return HIGH;
	if(!strcmp(s, "true")) return true;

	if(!strcmp(s, "INPUT")) return INPUT;
	if(!strcmp(s, "OUTPUT")) return OUTPUT;
	if(!strcmp(s, "INPUT_PULLUP")) return INPUT_PULLUP;
	if(!strcmp(s, "LED_BUILTIN")) return LED_BUILTIN;
	return 0;
}


void (*Command<>::errorHandler)(const char*) = [](const char*){};
int (*Command<>::streamRead)() = [](){ return -1; };
int (*Command<>::streamAvail)() = [](){ return 0; };
void (*Command<>::streamWrite)(const char*) = [](const char*){};

void Command<>::setErrorHandler(void (*err_handler)(const char*))
{
	Command<>::errorHandler = err_handler;
}
void Command<>::setStreamAvail(int(*stream_avail)())
{
	Command<>::streamAvail = stream_avail;
}
void Command<>::setStreamRead(int(*stream_read)())
{
	Command<>::streamRead = stream_read;
}
void Command<>::setStreamWrite(void(*stream_write)(const char*))
{
	Command<>::streamWrite = stream_write;
}

void Command<>::hook()
{
	static char buf[bufferSize] = "";

	if(streamAvail())
	{
		char ch = streamRead();

		if(ch == '\n') //if the delimeter has not yet been reached
		{
			//Concatenate the read byte to the string
			strcat(buf, &ch);
			//Execute command
			exec(buf);

			//Clear the buffer
			strcpy(buf, "");
		}
		else
		{
			//Concatenate the read byte to the string
			strcat(buf, &ch);
		}
	}
}

Command<> command("", [](){});