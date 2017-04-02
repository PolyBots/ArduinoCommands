/*
	Command.h - Command Library for Arduino - Allows user to bind functions to strings and run them
*/

// ensure this library is only included once
#ifndef Command_h
#define Command_h


#include <string.h>  // strcmp, strncmp, strtok, strcpy, strcat
#include <stdlib.h>  // atoi, atol, atof
#include <ctype.h>  // isdigit
#include <limits.h> // INT_MAX, LONG_MAX
#include <math.h> // log10, isnan, isinf

// empty class definition meant to prevent compilation when used incorrectly
template<class... Args>
class Command;

// top of Command class hierarchy, no template parameters
template<>
class Command<>
{
public:

	// enables commands to be printed back, arduino serial monitor does not do this
	// initially set to true
	static bool echo;
	// enables errorhandler, initially set to true
	static bool verbose;

	//size of input buffer (in bytes)
	static const size_t bufferSize;

	// interfaces to print and println using streamwrite
	static size_t print(const char*);
	static size_t print(char);
	static size_t print(int, unsigned char = 10);
	static size_t print(long, unsigned char = 10);
	static size_t print(float, unsigned char = 2);
	static size_t print(double, unsigned char = 2);

	static size_t println(const char* = "");
	static size_t println(char*);
	template<class T>
	static size_t println(const T&, unsigned char = 0);

	// setters for function pointers
	static void setErrorHandler(void(*)(const char*));
	static void setStreamAvail(int(*)());
	static void setStreamRead(int(*)());
	static void setStreamWrite(size_t(*)(const char*));

	// function pointers
	static void (*errorHandler)(const char*);
	static int (*streamAvail)();
	static int (*streamRead)();
	static size_t (*streamWrite)(const char*);

	// searches and executes commands/functions based on c-string
	static bool exec(const char*);

	static Command<>* find(const char*);

	// reads serial input and calls exec after newline character
	static void hook();

	//Used for converting c-strings to arguments
	template<class T>
	static T convertArg(const char*);
	// specifically for integers. converts special keywords to integers
	static int convertArgKeyword(const char*);

	// overload new/delete so commands will manage their own memory
	// static void* operator new(size_t);
	// static void operator delete(void*);

	// templated String-Functionality Constructor
	// both with and without description
	template<class L>
	Command(const char*, const L&);
	template<class L>
	Command(const char*, const char*, const L&);
	Command(const char*, void(*)());
	Command(const char*, const char*, void(*)());
	virtual ~Command();  // deconstructor

	//Copying and re-assignment of Commands is disabled
	Command(const Command<>&) = delete; // no copy constructor
	Command(Command<>&&) = delete; // no move copy constructor
	Command& operator=(const Command<>&) = delete; // no assignment operator. not allowed to redefine Command objects
	Command& operator=(Command<>&&) = delete; // no move assignment operator. not allowed to redefine Command objects

	// call the Command object like a function (functor)
	template<class... Args>
	void operator()(const Args&...);

	const char* getSignature() const;
	const char* getDesc() const;

	static void help(const char* = nullptr);

protected:
	//Child-RunLambda-Overload Constructor
	// for child classes to overload runLambda() and invLambda(), uses void pointers
	// to point to both the lambda and the parameters to be passed to the lambda function
	template<class L>
	Command(const char*, const char*, const L&, void (*)(void*), void (*)(void*, void*[])); // magic
		template<class L>
	Command(const char*, const L&, void (*)(void*), void (*)(void*, void*[])); // indescribable magic

	//Used for entries in a static registry
	//
	//NOTE: This Node is intended to be later
	//redesigned so that search time is faster
	//will likely use a prefix-tree or a radix-tree
	struct Node;

	//static registry
	static Node* registry;
	//static registry of nodes made with new operator
	static Node* dynReg;

	//back of registry
	static Node** back;
	//back of dynReg
	static Node** dynBack;

	//instance's entry in registry
	Node* entry;

	//name and functionality
	const char* signature;
	const char* description;
	size_t nameLength;
	void* lambda;
	void (*deleteLambda)(void*);
	void (*runLambda)(void*);
	void (*invLambda)(void*, void*[]);
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
	template<class L>
	Command(const char*, const char*, const L&);
	template<class L>
	Command(const char*, const L&);
	Command(const char*, const char*, void(*)(Args...));
	Command(const char*, void(*)(Args...));

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
inline Command<Args...>::Command(const char* signature, const char* description, const L& lambda)
	: Command<>(signature, description, lambda, [](void* l){
		char* args[sizeof...(Args)];
		for(unsigned char i = 0; i < sizeof...(Args); ++i) args[i] = nullptr;
		for(unsigned char i = 0; i < sizeof...(Args); ++i)
		{
			char* arg = strtok(NULL, " (),\r\n");
			if(!arg) break;
			if(!strcmp(arg, ""))
			{
				--i;
				continue;
			}
			args[i] = arg;
		}
		unsigned char index = sizeof...(Args);
		((L*)l)->operator()(Command<>::convertArg<Args>(args[--index])...);
	},
	[](void* l, void* a[]){
		unsigned char index = sizeof...(Args);
		((L*)l)->operator()((*(Args*)(a[--index]))...);
	})
{

}

template<class... Args>
template<class L>
inline Command<Args...>::Command(const char* signature, const L& lambda)
	: Command<Args...>(signature, nullptr, lambda)
{

}

//Alternate to String-functionality constructor (uses function pointer
//instead of lambda)
template<class... Args>
Command<Args...>::Command(const char* signature, const char* description, void(*func)(Args...))
	: Command<Args...>(signature, description, [&](Args... args) { func(args...); })
{

}

template<class... Args>
Command<Args...>::Command(const char* signature, void(*func)(Args...))
	: Command<Args...>(signature, nullptr, func)
{

}

//Child-RunLambda-Overload constructor for Command<>
template<class L>
inline Command<>::Command(const char* signature, const char* description, const L& lambda, void(*runLambda)(void*), void(*invLambda)(void*, void*[]))
	: entry(new Command::Node(this, nullptr)),
		signature(signature), description(description),
		lambda(new L(lambda)),
		deleteLambda([](void* l){ delete ((L*)l); }),
		runLambda(runLambda), invLambda(invLambda)
{
	char cmdbuf[bufferSize];
	strcpy(cmdbuf, signature);
	nameLength = strlen(strtok(cmdbuf, " (),\r\n"));

	if(registry == nullptr) registry = entry;
	else *back = entry;

	back = &(entry->next);
}

template<class L>
inline Command<>::Command(const char* signature, const L& lambda, void(*runLambda)(void*), void(*invLambda)(void*, void*[]))
	: Command<>(signature, nullptr, lambda, runLambda, invLambda)
{

}

//Specialization of String-Functionality constructor for Command<>
template<class L>
inline Command<>::Command(const char* signature, const char* description, const L& lambda)
	: Command<>(signature, description, lambda,
		[](void*l){ ((L*)l)->operator()(); },
		[](void* l, void* a[]){ ((L*)l)->operator()(); })
{

}

template<class L>
inline Command<>::Command(const char* signature, const L& lambda)
	: Command<>(signature, nullptr, lambda)
{

}



template<class... Args>
void Command<>::operator()(const Args&... args)
{
	void* argList[] = { &args... };
	invLambda(lambda, argList);
}

template<class T>
inline size_t Command<>::println(const T& obj, unsigned char baseOrPrecision)
{
	
	if(baseOrPrecision == 0) return Command<>::print(obj) + Command<>::print("\r\n");;
	
	return Command<>::print(obj, baseOrPrecision) + Command<>::print("\r\n");;
}



template<class T>
inline T Command<>::convertArg(const char* s)
{
	if(!s) return T();
	return T(s);
}

template<>
inline int Command<>::convertArg<int>(const char* s)
{
	if(!s) return 0;
	if(s[0] != '-' && !isdigit(s[0])) return Command<>::convertArgKeyword(s);
	return atoi(s);
}

template<>
inline long Command<>::convertArg<long>(const char* s)
{
	if(!s) return 0;
	if(s[0] != '-' && !isdigit(s[0])) return Command<>::convertArgKeyword(s);
	return atol(s);
}

template<>
inline double Command<>::convertArg<double>(const char* s)
{
	if(!s) return 0;
	return atof(s);
}

template<>
inline float Command<>::convertArg<float>(const char* s)
{
	if(!s) return 0;
	return Command<>::convertArg<double>(s);
}

template<>
inline const char* Command<>::convertArg<const char*>(const char* s)
{
	if(!s) return nullptr;
	return s;
}

//doubles as easy command static function syntax
//and as "echo" command when no command is specified
extern Command<int> command;

//command to turn verbose on/off
extern Command<int> cmd_verbose;

#endif