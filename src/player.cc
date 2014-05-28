#include "player.h"
#include "scrypt.h"

#include <unordered_map>
#include <fstream>
#include <sstream>
#include <ctime>
#include <v8.h>

using namespace std;
using namespace v8;

static unordered_map<string, Player> PlayersInGame;
static unordered_map<string, Player> PlayersAll;

static Persistent<Object> DisconnectObj;

Handle<Value> SetDisconnect(const Arguments& args)
{
	HandleScope handleScope;
	
	if (args.Length() < 1){
		ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
		return handleScope.Close(Undefined());
    }
    if (!args[0]->IsObject()) {
    	ThrowException(Exception::TypeError(String::New("Wrong arguments")));
    	return handleScope.Close(Undefined());
	}

	DisconnectObj = Persistent<Object>::New(args[0]->ToObject());
	
	return handleScope.Close(Null());
}

Player* playerForSocket(string sockuid)
{
	return &PlayersInGame[sockuid];
}

void connectSocketToPlayer(string sockuid, string name)
{
	Player player = PlayersAll[name];
	for (auto it = PlayersInGame.begin(); it != PlayersInGame.end(); ++it){
		if (it->second.name() == name){
			Handle<Context> context = Context::GetCurrent();
			Handle<Object> global = context->Global();
			Handle<Value> argv[] = { String::New(it->second.socketUID().c_str()) };
			DisconnectObj->CallAsFunction(global, 1, argv);
			disconnectSocket(it->second.socketUID());
		}
	}
	player.setSocketUID(sockuid);
	PlayersInGame[sockuid] = player;
}

void connectSocketToNewPlayer(string sockuid, Player player)
{
	player.setSocketUID(sockuid);
	PlayersInGame[sockuid] = player;
}

void savePlayer(Player player)
{
	PlayersAll[player.name()] = player;
}

void disconnectSocket(string sockuid)
{
	Player player = PlayersInGame[sockuid];
	savePlayer(player);
	PlayersInGame.erase(sockuid);
}

bool checkPlayerExists(string name)
{
	return PlayersAll.count(name) > 0;
}

bool verifyPlayer(string name, string password)
{
	Player player = PlayersAll[name];
	string hash = player.passwordHash();
	return VerifyPassword(hash, password);
}

Player::Player() {};

string Player::Serialize()
{
	string str = 
	"{" +
	name_ + "," +
	password_hash_ + "," +
	"}";
	return str;
}

Handle<Value> SavePlayers(const Arguments& args)
{
	for (auto it = PlayersInGame.begin(); it != PlayersInGame.end(); ++it){
		savePlayer(it->second);
	}

	time_t rawtime;
	struct tm * timeinfo;

	time (&rawtime);
	timeinfo = localtime(&rawtime);
	string timeStr = string(asctime(timeinfo));

	ofstream tf("./savefiles/LastSaveTime.txt", ios::out | ios::trunc);
	if (tf.is_open()){
		tf << timeStr;
	}

	ofstream file("./savefiles/Players " + timeStr + ".txt", ios::out | ios::app);
	if (file.is_open()){
		for (auto it = PlayersAll.begin(); it != PlayersAll.end(); ++it){
			file << it->second.Serialize() << ",";
		}
		file.close();
    }

    return Null();
}

Handle<Value> LoadPlayers(const Arguments& args)
{
	stringstream tb;
	ifstream tf("./savefiles/LastSaveTime.txt", ios::in);
	if (tf.is_open()){
		tb << tf.rdbuf();
	}
	ifstream file("./savefiles/Players " + tb.str() + ".txt", ios::in);
	if (file.is_open()){
		stringstream buffer;
		buffer << file.rdbuf();
		string str = buffer.str();
		unsigned long temp;
		int c = 0;
		int n = 0;
		while ((temp = str.find('{', c)) != string::npos){
			c = temp + 1;
			n = str.find(',', c);
			string name = str.substr(c, n-c);
			c = n + 1;
			n = str.find(',', c);
			string pw = str.substr(c, n-c);
			PlayersAll[name] = Player(name, pw);
		}
	}
	return Null();
}