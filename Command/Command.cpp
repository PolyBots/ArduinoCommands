#include "Command.h"
#include <stdint.h>
#include <math.h>
#include <Arduino.h>

//Alternate to String-functionality constructor (uses function pointer
//instead of lambda)
Command<>::Command(const char* signature, const char* description, void(*func)())
	: Command(signature, description, [&](){ func(); })
{

}

Command<>::Command(const char* signature, void(*func)())
	: Command(signature, nullptr, func)
{

}

//deletes the stored lambda and unregisters the command
Command<>::~Command()
{
	deleteLambda(lambda);
	
	Command::Node* n = registry;
	if(registry == entry)
	{
		registry = registry->next;
	}
	else
	{
		while(n->next != entry) n = n->next;
		n->next = n->next->next;
	}
	delete entry;
}

//pretty straight forward...
Command<>::Node::Node(Command<>* cmd, Command<>::Node* next)
	: cmd(cmd), next(next)
{
	
}

Command<>::Node* Command<>::registry = nullptr;
Command<>::Node* Command<>::dynReg = nullptr;
Command<>::Node** Command<>::back = nullptr;
Command<>::Node** Command<>::dynBack = nullptr;
const size_t Command<>::bufferSize = 64;
bool Command<>::echo = true;
bool Command<>::verbose = true;

// void* Command<>::operator new(size_t s)
// {
// 	void* result = ::operator new(s);
// 	Command<>::Node* e = ((Command<>*)result)->entry;
// 	if(dynReg == nullptr) dynReg = e;
// 	else *dynBack = e;

// 	dynBack = &(e->next);

// 	return result;
// }

// void Command<>::operator delete(void* p)
// {
// 	Command<>* c = p;
// 	Command<>::Node* n = dynReg;
// 	if(!dynReg) return;
// 	if(dynReg = c->entry)
// 	{
// 		dynReg = dynReg->next;
// 		return;
// 	}
// 	while(n->next != c->entry && n->next->next != nullptr) n = n->next;
// 	n->next = n->next->next;
// 	::operator delete(p);
// }

//Searches through the registry for a signature that matches "cmdName".
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

	if(!strcmp(name, "help"))
	{
		char* argCmd = strtok(NULL, " (),\r\n");
		Command<>::help(argCmd);
		return true;
	}

	Command<>* cmd = Command<>::find(name);
	if(cmd)
	{
		cmd->runLambda(cmd->lambda);
		return true;
	}

	if(verbose) Command<>::errorHandler(name);
	return false;
}

Command<>* Command<>::find(const char* name)
{
	for(Command<>::Node* n = registry; n != nullptr; n = n->next)
	{
		if(!strncmp(name, n->cmd->getSignature(), n->cmd->nameLength)) return n->cmd;
	}
	return nullptr;
}

void Command<>::help(const char* name)
{
	if(!name)
	{
		Command<>::println("Available Commands (type \"help\" followed by a command name for more info on the command):");
		for(Command<>::Node* n = registry; n != nullptr; n = n->next)
		{
			char sig[n->cmd->nameLength+1];
			strncpy(sig, n->cmd->getSignature(), n->cmd->nameLength);
			sig[n->cmd->nameLength] = '\0';
			Command<>::println(sig);
		}
		return;
	}

	Command<>* cmd = Command<>::find(name);
	if(cmd)
	{
		Command<>::println(cmd->getSignature());
		if(cmd->getDesc()) Command<>::println(cmd->getDesc());
		else Command<>::println("No description available.");
		return;
	}
	Command<>::errorHandler(name);
}

size_t Command<>::print(const char* str)
{
	return Command<>::streamWrite(str);
}

size_t Command<>::print(char c)
{
	const char buf[2] = {c, '\0'};
	return Command<>::streamWrite(buf);
}

size_t Command<>::print(int num, unsigned char base)
{
	unsigned char bufSize = (unsigned char)(log(double(INT_MAX))/log(double(base))) + 1U;
	char buf[bufSize];
	return Command<>::streamWrite(itoa(num, buf, base));
}

size_t Command<>::print(long num, unsigned char base)
{
	unsigned char bufSize = (unsigned char)(log(double(LONG_MAX))/log(double(base))) + 1U;
	char buf[bufSize];
	return Command<>::streamWrite(ltoa(num, buf, base));
}

size_t Command<>::print(double num, unsigned char precision)
{
	if (isnan(num)) return Command<>::print("nan");
	if (isinf(num)) return Command<>::print("inf");

	size_t n = 0;

	//negative checking
	if (num < 0.0)
	{
		n += Command<>::print("-");
		num = -num; //we invert number here so that we can print the rest of the number correctly
	}

	//rounds number to nearest decimal at precision
	num += 0.5 * pow(10, double(-precision));

	//prints the integer part
	n += Command<>::print(int(num));
	//reduces num to just decimal part
	num -= int(num);

	//prints the decimal point
	n += Command<>::print(".");

	//makes the decimal part into an integer part for purposes of printing
	num *= pow(10, double(precision));
	//prints the decimal part
	n += Command<>::print(int(num));

	return n;
}

size_t Command<>::print(float num, unsigned char precision)
{
	return Command<>::print(double(num), precision);
}

size_t Command<>::println(const char* str)
{
	return Command<>::print(str) + Command<>::print("\r\n");
}

size_t Command<>::println(char* str)
{
	return Command<>::print(str) + Command<>::print("\r\n");
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
size_t (*Command<>::streamWrite)(const char*) = [](const char*) -> size_t { return 0; };

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
void Command<>::setStreamWrite(size_t(*stream_write)(const char*))
{
	Command<>::streamWrite = stream_write;
}

void Command<>::hook()
{
	static char buf[bufferSize] = "";

	if(streamAvail())
	{
		char ch = streamRead();

		if(ch == '\n' || ch == '\r') //if the delimeter has been reached
		{
			//Concatenate the read byte to the string
			strcat(buf, &ch);

			//Execute command if input string isn't empty
			if(strcmp(buf, "\r") && strcmp(buf, "\n")) exec(buf);

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

const char* Command<>::getSignature() const
{
	return signature;
}

const char* Command<>::getDesc() const
{
	return description;
}

Command<int> command("echo [ON/OFF]",
	"Turns echo ON or OFF. When echo is ON, commands invoked will be printed back to the user.",
	[](int state)
	{
		Command<>::echo = state;
	}
);

Command<int> cmd_verbose("verbose [ON/OFF]",
	"Turns verbose mode ON or OFF. When verbose mode is ON, the user will be informed when a command does not exist.",
	[](int state)
	{
		Command<>::verbose = state;
	}
);