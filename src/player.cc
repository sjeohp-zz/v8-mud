#include "player.h"

#include <unordered_map>

using namespace std;

Player::Player() {};

static unordered_map<string, Player> PlayersInGame;
static unordered_map<string, Player> PlayersAll;

Player* playerForSocket(string sockuid)
{
	return &PlayersInGame[sockuid];
}

void setPlayerForSocket(string sockuid, Player player)
{
	PlayersInGame[sockuid] = player;
}

void removePlayerFromGame(string name)
{
	PlayersAll[name] = PlayersInGame[name];
	PlayersInGame.erase(name);
}

bool checkPlayerExists(string name)
{
	return PlayersAll.count(name) > 0;
}

Player playerForName(string name)
{
	return PlayersAll[name];
}