/*
  Command.h - Command Library for Arduino - Allows user to bind functions to strings and run them
*/

// ensure this library is only included once
#ifndef Command_h
#define Command_h

//#include "WString.h" //for use of String class
#include <string.h>

class Command
{
public:

	static bool echo;

	static int (*streamAvail)();
	static int (*streamRead)();
	static void (*streamWrite)(const char*);

	static void setErrorHandler(void(*)(const char*));

	static void setStreamAvail(int(*)());
	static void setStreamRead(int(*)());
	static void setStreamWrite(void(*)(const char*));

	static bool exec(const char*);
	static void hook();



	//String-Functionality Constructor
	template<class L> Command(const char*, const L&);
	~Command();

	//Copying and re-assignment of Commands is disabled
	Command(const Command&) = delete;
	Command(Command&&) = delete;
	Command& operator=(const Command&) = delete;
	Command& operator=(Command&&) = delete;

private:

	static void (*errorHandler)(const char*);

	//Used for entries in a static registry
	//
	//NOTE: This Node is intended to be later
	//redesigned so that search time is faster
	//will likely use a prefix-tree or a radix-tree
	struct Node;

	//static registry
	static Node* registry;

	//back of registry
	static Node** back;

	//instance's entry in registry
	Node* entry;

	//name and functionality
	const char* nametag;
	void* lambda;
	void (*deleteLambda)(void*);
	void (*runLambda)(void*);
};

class Command::Node
{
public:
    Node(Command*, Command::Node*);
    
    Command* cmd;
	Command::Node* next;
};

//String-Functionality Constructor
//creates its entry in the registry and registers to it
//using that entry.
//Every lambda function (even identical ones) are unique types.
//This issue is circumvented by creating run and delete functions
//for the lambda on the fly during construction and storing the
//lambda in a void pointer
//
//NOTE: This method is used over simply storing the lambda
//in a function pointer because function pointers cannot store
//lambda functions if the lambda functions rely on outside sources
//(e.g. variables) that are not passed as a parameter (this is
//referred to as "capturing")
template<class L>
inline Command::Command(const char* nametag, const L& lambda)
  : entry(new Command::Node(this, nullptr)),
    nametag(nametag), lambda(new L(lambda)),
    deleteLambda([](void* l){delete ((L*)l);}),
    runLambda([](void* l){((L*)l)->operator()(); })
{
  if(registry == nullptr) registry = entry;
  else *back = entry;

  back = &(entry->next);
}

#endif