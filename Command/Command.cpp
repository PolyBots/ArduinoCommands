#include "Command.h"
#include <stdint.h>

//deletes the stored lambda and unregisters the command
Command<>::~Command()
{
	deleteLambda(lambda);
	
	Command::Node* n = registry;
	while(n->next != &entry) n = n->next;
	n->next = n->next->next;
}

//pretty straight forward...
Command<>::Node::Node(Command<>* cmd, Command<>::Node* next)
	: cmd(cmd), next(next)
{
	
}

Command<>::Node* Command<>::registry = nullptr;
Command<>::Node** Command<>::back = nullptr;
bool Command<>::echo = true;
bool Command<>::verbose = true;

//Searches through the registry for a nametag that matches "cmdName".
//If found, runs the command and returns true. If no match is found,
//returns false.
bool Command<>::exec(const char* cmdstr)
{
	//print back command if echo is on
	if(echo)
	{
		streamWrite("> ");
		streamWrite(cmdstr);
	}

	char* name = strtok(cmdstr, " \n");

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
	static char buf[64] = "";

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