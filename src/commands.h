#ifndef Mud_Commands_h
#define Mud_Commands_h

#include <v8.h>

#include "sockets.h"
#include "trie.h"

using namespace std;
using namespace v8;

typedef string (*CommandFunc)(string, string);

string getCmd(string sockuid, string args)
{
	return string("Just got something\n");
}

string gossipCmd(string sockuid, string args)
{
	Local<Context> context = Context::GetCurrent();
	Handle<Object> global = context->Global();
	
	Persistent<Object> socket = ActiveSockets[sockuid];
	
	string playerName = PlayersInGame[sockuid].name();
	string msg = playerName + ": " + args + "\n";
	
	Handle<Value> broadcastArgv[] = { socket, String::New(msg.c_str()) };
	Broadcast->CallAsFunction(global, 2, broadcastArgv);
	
	msg = "You: " + args + "\n";
	
	return msg;
}

typedef struct CommandPair
{
	const unsigned char* key;
	void* func;
	int abbreviates;
}	CommandPair;

static const size_t ncmd = 2; // remember to increment this when adding commands
static CommandPair commandPairs[ncmd] = // precedence is top down
{
	{ (const unsigned char*)"get\0", (void*)&getCmd, 1 },
	{ (const unsigned char*)"gossip\0", (void*)&gossipCmd, 1 }
};

Trie* newTrieWithCommandList(size_t npairs, CommandPair cmdpairs[])
{
	Trie* node = (Trie*)calloc(1, sizeof(Trie));
	for (size_t i = 0; i < npairs; ++i){
		addKeyForValue(node, cmdpairs[i].key, cmdpairs[i].func, cmdpairs[i].abbreviates);
	}
	return node;
}

Trie* Commands()
{
	static Trie* commands;
	if (!commands){
		commands = newTrieWithCommandList(ncmd, commandPairs);
	}
	return commands;
}

#endif