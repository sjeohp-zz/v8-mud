#include <iostream>
#include <unordered_map>

#include <node.h>
#include <v8.h>

#include "src/player.h"
#include "src/commands.h"
#include "src/sockets.h"
#include "src/trie.h"
#include "src/scrypt.h"

using namespace std;
using namespace v8;

static unordered_map<string, string> ActiveNames;
static unordered_map<string, string> UnconfirmedPasswords;

string processCommand(string sockuid, string str)
{
	size_t cmdEnd = str.find(' '); 
	const unsigned char* cmdKey = (const unsigned char*)str.substr(0, cmdEnd).c_str();
	CommandFunc cmdFunc = (CommandFunc)valueForKey(Commands(), cmdKey);
	string res;
	if (cmdFunc){
		res = cmdFunc(sockuid, str.substr(cmdEnd+1));
	} else {
		res = string(1, (char)INGAME) + "Eh?\n";
	}
	return res;
}

string processConnection(string sockuid, int state, string msg)
{
	string res;
	if (state == UNNAMED){
		ActiveNames[sockuid] = msg;
		if (checkPlayerExists(msg)){
			state = UNVERIFIED;
			res = "Password:\n";
		} else {
			state = NAME_CHOSEN;
			res = msg + "? Is that your name?\n";
		}
	} else if (state == UNVERIFIED){
		string name = ActiveNames[sockuid];
		if (verifyPlayer(name, msg)){
			connectSocketToPlayer(sockuid, name);
			state = INGAME;
			res = "Very well.\n";
		} else {
			res = "Wrong. Try again:\n";
		}
	} else if (state == NAME_CHOSEN){
		if (tolower(msg[0]) == 'y'){
			state = NAME_CONFIRMED;
			res = "Choose a password:\n";
		} else if (tolower(msg[0]) == 'n'){
			ActiveNames.erase(sockuid);
			state = UNNAMED;
			res = "Who are you then?\n";
		} else {
			res = "Answer yes or no. Is your name " + ActiveNames[sockuid] + "?\n";
		}
	} else if (state == NAME_CONFIRMED){
		string passwordHash = HashPassword(msg);
		UnconfirmedPasswords[sockuid] = passwordHash;
		state = PASSWORD_CHOSEN;
		res = "Confirm your password:\n";
	} else if (state == PASSWORD_CHOSEN){
		if (VerifyPassword(UnconfirmedPasswords[sockuid], msg)){
			connectSocketToNewPlayer(sockuid, Player(ActiveNames[sockuid], UnconfirmedPasswords[sockuid]));
			state = PASSWORD_CONFIRMED;
			res = "Very well.\n";
		} else {
			state = NAME_CONFIRMED;
			res = "Passwords didn't match. Choose a password:\n";
			UnconfirmedPasswords.erase(sockuid);
		}
	} else if (state == PASSWORD_CONFIRMED){
	// char creation?
		state = INGAME;
	}
	res.insert(0, 1, (char)state);
	return res;
}

Handle<Value> ProcessInput(const Arguments& args)
{
	
    HandleScope scope;

    if (args.Length() < 1) {
      ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
      return scope.Close(Undefined());
    }
    if (!args[0]->IsString()) {
      ThrowException(Exception::TypeError(String::New("Wrong arguments")));
      return scope.Close(Undefined());
	}

	string str = string(*String::Utf8Value(args[0]->ToString()));
	string sockuid = str.substr(0, 25);
	int state = str.at(25);
	string msg = str.substr(26);
	string 	res;
	
	if (state == INGAME){
		res = processCommand(sockuid, msg);
	} else {
		res = processConnection(sockuid, state, msg);
	}
	
    return scope.Close(String::New(res.c_str()));
}

void Init(Handle<Object> exports, Handle<Object> module) 
{
	exports->Set(String::NewSymbol("process"), FunctionTemplate::New(ProcessInput)->GetFunction());
	exports->Set(String::NewSymbol("setScrypt"), FunctionTemplate::New(SetScrypt)->GetFunction());
	exports->Set(String::NewSymbol("setBroadcast"), FunctionTemplate::New(SetBroadcast)->GetFunction());
	exports->Set(String::NewSymbol("setWrite"), FunctionTemplate::New(SetWrite)->GetFunction());
	exports->Set(String::NewSymbol("savePlayers"), FunctionTemplate::New(SavePlayers)->GetFunction());
	exports->Set(String::NewSymbol("loadPlayers"), FunctionTemplate::New(LoadPlayers)->GetFunction());
	exports->Set(String::NewSymbol("setDisconnect"), FunctionTemplate::New(SetDisconnect)->GetFunction());
	exports->Set(String::NewSymbol("removePlayerOnSocket"), FunctionTemplate::New(RemovePlayerOnSocket)->GetFunction());
	exports->Set(String::NewSymbol("runBenchmarks"), FunctionTemplate::New(run_command_benchmarks)->GetFunction());
}

NODE_MODULE(mud, Init)