#include "commands.h"
#include "sockets.h"
#include "trie.h"
#include "player.h"
#include "room.h"
#include "timestamp.h"

using namespace std;
using namespace v8;

typedef struct CommandPair
{
	const unsigned char* key;
	void* func;
	int abbreviates;
}	CommandPair;

string cmdGet(string sockuid, string args)
{
	return string(1, (char)INGAME) + "Just got something\n";
}

string cmdGossip(string sockuid, string args)
{
	Write(sockuid, "\033[33mYou gossip, \"" + args + "\"\033[39m\n");

	string playerName = playerForSocket(sockuid)->name();
	string msg = "\033[33m" + playerName + " gossips, \"" + args + "\"\033[39m\n";

	Broadcast(sockuid, msg);
	
	msg = string(1, (char)INGAME);
	return msg;
}

string cmdLook(string sockuid, string args)
{
	Player* player = playerForSocket(sockuid);
	string rmname = "\033[96m" + player->room()->name() + "\033[39m\n";
	string rmexits = "\033[92m" + player->room()->exits() + "\033[39m\n";
	string rmdesc = "\033[32m" + player->room()->desc() + "\033[39m\n";
	string msg = string(1, (char)INGAME) + rmname + rmexits + rmdesc;
	return msg;
}

string cmdSay(string sockuid, string args)
{
	Write(sockuid, "\033[37mYou say, \"" + args + "\"\033[39m\n");

	Player* pl = playerForSocket(sockuid);
	string playerName = pl->name();
	string msg = "\033[37m" + playerName + " says, \"" + args + "\"\033[39m\n";
	
	Player* curr = pl->roomPlayersNext();
	cout << "curr" << endl;
	while (curr != pl){
		Write(curr->socketUID(), msg);
		cout << "write" << endl;
		curr = curr->roomPlayersNext();
	}

	msg = string(1, (char)INGAME);
	return msg;
}

string cmdQuit(string sockuid, string args)
{
	disconnectPlayerOnSocket(sockuid);
	return string(1, (char)QUITTING);
}

static const size_t ncmd = 5; // remember to increment this when adding commands
static CommandPair commandPairs[ncmd] = // precedence is top down
{
	{ (const unsigned char*)"get\0", (void*)&cmdGet, 1 },
	{ (const unsigned char*)"gossip\0", (void*)&cmdGossip, 1 },

	{ (const unsigned char*)"look\0", (void*)&cmdLook, 1 },

	{ (const unsigned char*)"say\0", (void*)&cmdSay, 1 },
	
	{ (const unsigned char*)"quit\0", (void*)&cmdQuit, 0 },
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