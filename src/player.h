#ifndef Mud_Player_h
#define Mud_Player_h

#include <string>

using namespace std;

typedef string PlayerName;
typedef string SocketUID;

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
	QUITTING			= 8
}	PLAYER_STATE;

class Player
{
private:
	PlayerName name_;
	string password_hash_;
	SocketUID sockuid_;
public:
	Player();
	Player(string name, string hash)
		:  name_(name), password_hash_(hash)
		{};
	string name() const { return name_; }
	void setName(string name)
	{
		name_ = name;
	}
	string passwordHash() const { return password_hash_; }
	void setPasswordHash(string password_hash)
	{
		password_hash_ = password_hash;
	}
	string socketUID() const { return sockuid_; }
	void setSocketUID(string sockuid)
	{
		sockuid_ = sockuid;
	}
};

Player* playerForSocket(SocketUID sockuid);
void connectSocketToPlayer(SocketUID sockuid, PlayerName name);
void connectSocketToNewPlayer(SocketUID sockuid, Player player);
void disconnectSocket(SocketUID sockuid);
bool checkPlayerExists(PlayerName name);
bool verifyPlayer(PlayerName name, string password);

#endif