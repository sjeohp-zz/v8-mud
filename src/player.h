#ifndef Mud_Player_h
#define Mud_Player_h

#include <string>
#include <v8.h>

using namespace std;
using namespace v8;

class Room;

typedef enum 
{
	ZILCH 				= 0,
	UNNAMED 			= 1,
	UNVERIFIED 			= 2,
	NAME_CHOSEN			= 3,
	NAME_CONFIRMED		= 4,
	PASSWORD_CHOSEN		= 5,
	PASSWORD_CONFIRMED	= 6,
	INGAME				= 7,
	QUITTING			= 9
}	PLAYER_STATE;

class Player
{
private:
	string name_;
	string password_hash_;
	string sockuid_;
	Room* room_;
	Player* room_players_next_;
	Player* room_players_prev_;
public:
	Player();
	Player(string name, string hash) :  
		name_(name), 
		password_hash_(hash)
		{};
	string Serialize();
	string name() const { return name_; }
	void setName(string name) { name_ = name; }
	string passwordHash() const { return password_hash_; }
	void setPasswordHash(string password_hash) { password_hash_ = password_hash; }
	string socketUID() const { return sockuid_; }
	void setSocketUID(string sockuid) { sockuid_ = sockuid; }
	Room* room() const { return room_; }
	Player* roomPlayersNext() const { return room_players_next_; }
	Player* roomPlayersPrev() const { return room_players_prev_; }
};

Player* playerForSocket(string sockuid);
bool checkPlayerExists(string name);
bool verifyPlayer(string name, string password);
void connectSocketToPlayer(string sockuid, string name);
void connectSocketToNewPlayer(string sockuid, Player player);
void disconnectPlayerOnSocket(string sockuid);
Handle<Value> SetDisconnect(const Arguments& args);
Handle<Value> RemovePlayerOnSocket(const Arguments& args);
Handle<Value> SavePlayers(const Arguments& args);
Handle<Value> LoadPlayers(const Arguments& args);

#endif