#include "commands.h"
#include "sockets.h"
#include "trie.h"
#include "player.h"
#include "room.h"
#include "timestamp.h"

#include <map>

#include "chess.h"

using namespace std;
using namespace v8;

typedef struct CommandPair
{
	const unsigned char* key;
	void* func;
	int abbreviates;
}	CommandPair;

static map<char, int> DirectionMap = { {'n', 0}, {'e', 1}, {'s', 2}, {'w', 3}, {'u', 4}, {'d', 5} };
static map<char, int> InverseDirectionMap = { {'n', 2}, {'e', 3}, {'s', 0}, {'w', 1}, {'u', 5}, {'d', 4} };

string cmdNorth(string sockuid, string args);
string cmdEast(string sockuid, string args);
string cmdSouth(string sockuid, string args);
string cmdWest(string sockuid, string args);
string cmdUp(string sockuid, string args);
string cmdDown(string sockuid, string args);
string cmdTunnelOneWay(string sockuid, string args);
string cmdTunnelBothWays(string sockuid, string args);
string cmdGet(string sockuid, string args);
string cmdGossip(string sockuid, string args);
string cmdResetBoard(string sockuid, string args);
string cmdLook(string sockuid, string args);
string cmdSay(string sockuid, string args);
string cmdQuit(string sockuid, string args);

void moveDir(string sockuid, string dir)
{
	Player* pl = playerForSocket(sockuid);
	Room* fromRm = pl->room();

	int d = DirectionMap[dir[0]];

	if (!fromRm->exits()[d]){
		Write(sockuid, "You can't go that way.\n");
		return;
	}

	EchoAround(sockuid, pl->name() + " leaves " + dir + ".\n");

	Room* toRm = roomAt(fromRm->exits()[d]);
	fromRm->removePlayerWithName(pl->name());
	toRm->addPlayer(pl);
}

string cmdNorth(string sockuid, string args)
{
	moveDir(sockuid, "north");
	return cmdLook(sockuid, args);
}

string cmdEast(string sockuid, string args)
{
	moveDir(sockuid, "east");
	return cmdLook(sockuid, args);
}

string cmdSouth(string sockuid, string args)
{
	moveDir(sockuid, "south");
	return cmdLook(sockuid, args);
}

string cmdWest(string sockuid, string args)
{
	moveDir(sockuid, "west");
	return cmdLook(sockuid, args);
}

string cmdUp(string sockuid, string args)
{
	moveDir(sockuid, "up");
	return cmdLook(sockuid, args);
}

string cmdDown(string sockuid, string args)
{
	moveDir(sockuid, "down");
	return cmdLook(sockuid, args);
}

string cmdTunnelOneWay(string sockuid, string args)
{
	Room* fromRm = playerForSocket(sockuid)->room();

	int dirend = args.find(' ');

	string res = string(1, (char)INGAME);
	if (dirend < 0){
		res += "Invalid arguments. (tunnel <direction> <destination rnum>)\n";
		return res;
	}

	string dirkey = args.substr(0, dirend);
	int dir;

	if (!valueForKey(Commands(), (unsigned char*)dirkey.c_str())){
		res += "That's not a valid direction. (tunnel <direction> <destination rnum>)\n";
		return res;
	} else {
		dir = DirectionMap[dirkey[0]];
	}

	if (dirend + 1 >= args.size()){
		res += "That's not a valid destination. (tunnel <direction> <destination rnum>)\n";
		return res;
	}

	int rnum = stol(args.substr(dirend+1));
	fromRm->setExit(dir, rnum);

	Write(sockuid, "You construct an exit to the " + dirkey + ".\n");
	EchoAround(sockuid, playerForSocket(sockuid)->name() + " constructs an exit to the " + dirkey + ".\n");

	return res;
}

string cmdTunnelBothWays(string sockuid, string args)
{
	Room* fromRm = playerForSocket(sockuid)->room();

	int dirend = args.find(' ');

	string res = string(1, (char)INGAME);
	if (dirend < 0){
		res += "Invalid arguments. (tunnel <direction> <destination rnum>)\n";
		return res;
	}

	string dirkey = args.substr(0, dirend);
	int dir;

	if (!valueForKey(Commands(), (unsigned char*)dirkey.c_str())){
		res += "That's not a valid direction. (tunnel <direction> <destination rnum>)\n";
		return res;
	} else {
		dir = DirectionMap[dirkey[0]];
	}

	if (dirend + 1 >= args.size()){
		res += "That's not a valid destination. (tunnel <direction> <destination rnum>)\n";
		return res;
	}

	int rnum = stol(args.substr(dirend+1));
	if (!roomAt(rnum)){
		res += "That destination is out of bounds.\n";
		return res;
	}
	Room* toRm = roomAt(rnum);

	fromRm->setExit(dir, rnum);

	Write(sockuid, "You construct an exit to the " + dirkey + ".\n");
	EchoAround(sockuid, playerForSocket(sockuid)->name() + " constructs an exit to the " + dirkey + ".\n");

	int invdir = InverseDirectionMap[dirkey[0]];
	if (toRm->exits()[invdir]){
		res += "Destination already has an exit in this direction. Could not construct return exit.\n";
		return res;
	}

	toRm->setExit(invdir, fromRm->rnum());

	return res;
}

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

string cmdResetBoard(string sockuid, string args)
{
	resetBoard(WHITE);
	return string(1, (char)INGAME) + "Board reset.\n";
}

string cmdLook(string sockuid, string args)
{
	if (args == "board") {
		Board* board = GetSharedBoard();
		string boardStr = string(1, (char)INGAME) + "\n  a b c d e f g h \n";
		// printf("\n");
		for (int row = 0; row < GetBoardDimen(); ++row) {
			boardStr += to_string(GetBoardDimen()-row);
			boardStr += " ";
			for (int col = 0; col < GetBoardDimen(); ++col) {
				Square* square = board[row][col];
				boardStr += strForSquare(square);
				boardStr += " ";
			}
			boardStr += " ";
			boardStr += to_string(GetBoardDimen()-row);
			boardStr += "\n";
			// printf("\n");
		}

		boardStr += "  a b c d e f g h \n\n";
		return boardStr;
	}

	Player* player = playerForSocket(sockuid);
	string rmname = "\033[96m" + player->room()->name() + "\033[39m\n";
	string rmexits = "\033[92m" + player->room()->exitstr() + "\033[39m\n";
	string rmdesc = "\033[32m" + player->room()->desc() + "\033[39m\n";
	string players = "\033[36m";
	Player* curr = player->roomPlayersNext();
	while (curr != player){
		players += curr->name() + " is here.\n";
		curr = curr->roomPlayersNext();
	}
	players += "\033[39m";
	string msg = string(1, (char)INGAME) + rmname + rmexits + rmdesc + players;
	return msg;
}

string cmdSay(string sockuid, string args)
{
	Write(sockuid, "\033[97mYou say, \"" + args + "\"\033[39m\n");

	string playerName = playerForSocket(sockuid)->name();
	string msg = "\033[97m" + playerName + " says, \"" + args + "\"\033[39m\n";
	
	EchoAround(sockuid, msg);

	msg = string(1, (char)INGAME);
	return msg;
}

string cmdQuit(string sockuid, string args)
{
	disconnectPlayerOnSocket(sockuid);
	return string(1, (char)QUITTING);
}

static const size_t ncmd = 14; // remember to increment this when adding commands
static CommandPair commandPairs[ncmd] = // precedence is top down
{
	{ (const unsigned char*)"north\0", (void*)&cmdNorth, 1 },
	{ (const unsigned char*)"east\0", (void*)&cmdEast, 1 },
	{ (const unsigned char*)"south\0", (void*)&cmdSouth, 1 },
	{ (const unsigned char*)"west\0", (void*)&cmdWest, 1 },
	{ (const unsigned char*)"up\0", (void*)&cmdUp, 1 },
	{ (const unsigned char*)"down\0", (void*)&cmdDown, 1 },

	{ (const unsigned char*)"get\0", (void*)&cmdGet, 1 },
	{ (const unsigned char*)"gossip\0", (void*)&cmdGossip, 1 },

	{ (const unsigned char*)"reset\0", (void*)&cmdResetBoard, 0 },
	{ (const unsigned char*)"look\0", (void*)&cmdLook, 1 },

	{ (const unsigned char*)"say\0", (void*)&cmdSay, 1 },

	{ (const unsigned char*)"tunnel\0", (void*)&cmdTunnelOneWay, 0 },
	{ (const unsigned char*)"tunnel~\0", (void*)&cmdTunnelBothWays, 0 },
	
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