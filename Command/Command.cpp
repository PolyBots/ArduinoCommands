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

//Searches through the registry for a nametag that matches "cmdName".
//If found, runs the command and returns true. If no match is found,
//returns false.
bool Command::exec(const String& cmdName)
{
  for(Command::Node* n = registry; n != nullptr; n = n->next)
  {
    if(cmdName == n->cmd->nametag)
    {
      n->cmd->runLambda(n->cmd->lambda);
      return true;
    }
  }
  return false;
}