#include "player.h"
#include "scrypt.h"

#include <unordered_map>

using namespace std;

static unordered_map<SocketUID, Player> PlayersInGame;
static unordered_map<PlayerName, Player> PlayersAll;

Player::Player() {};

Player* playerForSocket(SocketUID sockuid)
{
	return &PlayersInGame[sockuid];
}

void connectSocketToPlayer(SocketUID sockuid, PlayerName name)
{
	Player player = PlayersAll[name];
	player.setSocketUID(sockuid);
	PlayersInGame[sockuid] = player;
}

void connectSocketToNewPlayer(SocketUID sockuid, Player player)
{
	player.setSocketUID(sockuid);
	PlayersInGame[sockuid] = player;
}

void disconnectSocket(SocketUID sockuid)
{
	Player player = PlayersInGame[sockuid];
	PlayersAll[player.name()] = player;
	PlayersInGame.erase(sockuid);
}

bool checkPlayerExists(PlayerName name)
{
	return PlayersAll.count(name) > 0;
}

bool verifyPlayer(PlayerName name, string password)
{
	Player player = PlayersAll[name];
	string hash = player.passwordHash();
	return VerifyPassword(hash, password);
}