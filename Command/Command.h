/*
	Command.h - Command Library for Arduino - Allows user to bind functions to strings and run them
*/

// ensure this library is only included once
#ifndef Command_h
#define Command_h

#include <string.h>
#include <stdlib.h>
#include <ctype.h>

template<class... Args>
class Command;

template<>
class Command<>
{
public:

	static bool echo;
	static bool verbose;

	template<class T>
	static void print(const T&);
	template<class T>
	static void println(const T&);

	static void setErrorHandler(void(*)(const char*));
	static void setStreamAvail(int(*)());
	static void setStreamRead(int(*)());
	static void setStreamWrite(void(*)(const char*));

	static void (*errorHandler)(const char*);
	static int (*streamAvail)();
	static int (*streamRead)();
	static void (*streamWrite)(const char*);

	static bool exec(const char*);
	static void hook();

	//Used for converting c-strings to arguments
	template<class T>
	static T convertArg(const char*);

	static int convertArgKeyword(const char*);

	//String-Functionality Constructor
	template<class L> Command(const char*, const L&);
	~Command();

	//Copying and re-assignment of Commands is disabled
	Command(const Command&) = delete;
	Command(Command&&) = delete;
	Command& operator=(const Command&) = delete;
	Command& operator=(Command&&) = delete;

protected:
	//Child-RunLambda-Overload Constructor
	template<class L> Command(const char*, const L&, void (*)(void*));

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

class Command<>::Node
{
public:
	Node(Command<>*, Command<>::Node*);
		
	Command<>* cmd;
	Command<>::Node* next;
};

template<class... Args>
class Command : public Command<>
{
public:
	//String-Functionality Constructor
	template<class L> Command(const char*, const L&);

	//Copying and re-assignment of Commands is disabled
	Command(const Command&) = delete;
	Command(Command&&) = delete;
	Command& operator=(const Command&) = delete;
	Command& operator=(Command&&) = delete;
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
template<class... Args>
template<class L>
inline Command<Args...>::Command(const char* nametag, const L& lambda)
	: Command<>(nametag, lambda, [](void* l){
		char* args[sizeof...(Args)];
		for(unsigned char i = 0; i < sizeof...(Args); ++i)
		{
			char* arg = strtok(NULL, " (),\r\n");
			if(strcmp(arg, ""))
			{
				--i;
				continue;
			}
			args[i] = arg;
		}
		unsigned char index = sizeof...(Args);
		((L*)l)->operator()(Command<>::convertArg<Args>(args[--index])...);
	})
{

}

//Child-RunLambda-Overload constructor for Command<>
template<class L>
inline Command<>::Command(const char* nametag, const L& lambda, void(*runLambda)(void*))
	: entry(new Command::Node(this, nullptr)),
		nametag(nametag), lambda(new L(lambda)),
		deleteLambda([](void* l){ delete ((L*)l); }),
		runLambda(runLambda)
{
	if(registry == nullptr) registry = entry;
	else *back = entry;

	back = &(entry->next);
}

//Specialization of String-Functionality constructor for Command<>
template<class L>
inline Command<>::Command(const char* nametag, const L& lambda)
	: Command(nametag, lambda, [](void*l){ ((L*)l)->operator()(); })
{

}



template<class T>
inline void Command<>::print(const T& obj)
{
	(void(*)(const T&))Command<>::streamWrite(obj);
}

template<class T>
inline void Command<>::println(const T& obj)
{
	Command<>::print(obj);
	Command<>::print("\r\n");
}



//NOTE: INTENDED TO BE SET UP SO THAT USERS CAN ADD KEYWORDS
//WITHOUT MODIFYING HEADER FILE
inline int Command<>::convertArgKeyword(const char* s)
{
	if(!strcmp(s, "OFF")) return 0;
	if(!strcmp(s, "LOW")) return 0;
	if(!strcmp(s, "false")) return 0;

	if(!strcmp(s, "ON")) return 1;
	if(!strcmp(s, "HIGH")) return 1;
	if(!strcmp(s, "true")) return 1;
}

template<class T>
inline T Command<>::convertArg(const char* s)
{
	return T(s);
}

template<>
inline int Command<>::convertArg<int>(const char* s)
{
	if(!isdigit(s[0])) return Command<>::convertArgKeyword(s);
	return atoi(s);
}

template<>
inline long Command<>::convertArg<long>(const char* s)
{
	if(!isdigit(s[0])) return Command<>::convertArgKeyword(s);
	return atol(s);
}

template<>
inline float Command<>::convertArg<float>(const char* s)
{
	return atof(s);
}

template<>
inline double Command<>::convertArg<double>(const char* s)
{
	return atof(s);
}

template<>
inline const char* Command<>::convertArg<const char*>(const char* s)
{
	return s;
}

extern Command<> command;

#endif