#include "commands.h"
#include "sockets.h"
#include "trie.h"
#include "player.h"
#include "timestamp.h"

using namespace std;
using namespace v8;

typedef struct CommandPair
{
	const unsigned char* key;
	void* func;
	int abbreviates;
}	CommandPair;

string getCmd(string sockuid, string args)
{
	return string(1, (char)INGAME) + "Just got something\n";
}

string gossipCmd(string sockuid, string args)
{
	Write(sockuid, "\033[33mYou gossip, \"" + args + "\"\033[39m\n");

	string playerName = playerForSocket(sockuid)->name();
	string msg = "\033[33m" + playerName + " gossips, \"" + args + "\"\033[39m\n";

	Broadcast(sockuid, msg);
	
	msg = string(1, (char)INGAME);
	return msg;
}

string quitCmd(string sockuid, string args)
{
	disconnectSocket(sockuid);
	return string(1, (char)QUITTING);
}

static const size_t ncmd = 3; // remember to increment this when adding commands
static CommandPair commandPairs[ncmd] = // precedence is top down
{
	{ (const unsigned char*)"get\0", (void*)&getCmd, 1 },
	{ (const unsigned char*)"gossip\0", (void*)&gossipCmd, 1 },

	{ (const unsigned char*)"quit\0", (void*)&quitCmd, 0 },
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

Handle<Value> run_command_benchmarks(const Arguments& args)
{
	return Null();
}