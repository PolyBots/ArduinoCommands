#include "Command.h"

//deletes the stored lambda and unregisters the command
Command::~Command()
{
  deleteLambda(lambda);
  
  Command::Node* n = registry;
  while(n->next != &entry) n = n->next;
  n->next = n->next->next;
}

//pretty straight forward...
Command::Node::Node(Command* cmd, Command::Node* next)
  : cmd(cmd), next(next)
{
  
}

Command::Node* Command::registry = nullptr;
Command::Node** Command::back = nullptr;
bool Command::echo = true;

//Searches through the registry for a nametag that matches "cmdName".
//If found, runs the command and returns true. If no match is found,
//returns false.
bool Command::exec(const char* cmdName)
{
  //print back command if echo is on
  if(echo)
  {
    streamWrite("> ");
    streamWrite(cmdName);
    streamWrite("\n");
  }

  for(Command::Node* n = registry; n != nullptr; n = n->next)
  {
    if(!strcmp(cmdName, n->cmd->nametag))
    {
      n->cmd->runLambda(n->cmd->lambda);
      return true;
    }
  }
  Command::errorHandler(cmdName);
  return false;
}

void (*Command::errorHandler)(const char*) = [](const char*){};
int (*Command::streamRead)() = [](){ return -1; };
int (*Command::streamAvail)() = [](){ return 0; };
void (*Command::streamWrite)(const char*) = [](const char*){};

void Command::setErrorHandler(void (*err_handler)(const char*))
{
  Command::errorHandler = err_handler;
}
void Command::setStreamAvail(int(*stream_avail)())
{
  Command::streamAvail = stream_avail;
}
void Command::setStreamRead(int(*stream_read)())
{
  Command::streamRead = stream_read;
}
void Command::setStreamWrite(void(*stream_write)(const char*))
{
  Command::streamWrite = stream_write;
}

void Command::hook()
{
  static char buf[64] = "";

  if(streamAvail())
  {
    char ch = streamRead();

    if(ch == '\n') //if the delimeter has not yet been reached
    {
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