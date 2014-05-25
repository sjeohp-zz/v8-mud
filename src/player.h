#ifndef Mud_Player_h
#define Mud_Player_h

#include <string>

using namespace std;

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
}	PLAYER_STATE;

class Player
{
private:
	string name_;
	string password_hash_;
	string sockuid_;
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
	string sockuid() const { return sockuid_; }
	void setSockuid(string sockuid)
	{
		sockuid_ = sockuid;
	}
};

Player::Player() {};

static unordered_map<string, Player> PlayersInGame;
static unordered_map<string, Player> PlayersAll;

#endif