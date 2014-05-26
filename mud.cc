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

string ProcessCommand(string sockuid, string msg)
{
	size_t cmdEnd = msg.find(' '); 
	const unsigned char* cmdKey = (const unsigned char*)msg.substr(0, cmdEnd).c_str();
	CommandFunc cmdFunc = (CommandFunc)valueForKey(Commands(), cmdKey);
	string res;
	
	if (cmdFunc){
		res = cmdFunc(sockuid, msg.substr(cmdEnd+1));
	} else {
		res = string("Eh?\n");
	}
	
	return res;
}

Handle<Value> ProcessInput(const Arguments& args)
{
	
    HandleScope handleScope;

    if (args.Length() < 1) {
      ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
      return handleScope.Close(Undefined());
    }
    if (!args[0]->IsString()) {
      ThrowException(Exception::TypeError(String::New("Wrong arguments")));
      return handleScope.Close(Undefined());
	}
	
	string 	str = string(*String::Utf8Value(args[0]->ToString()));
	string 	sockuid = str.substr(0, 25);
	int 	state = str.at(25);
	string 	msg = str.substr(26);
	string 	res;
	
	if (state == INGAME){
		res = ProcessCommand(sockuid, msg);
	} else if (state == UNNAMED){
		ActiveNames[sockuid] = msg;
		if (checkPlayerExists(msg)){
			state = UNVERIFIED;
			res = "Password:\n";
		} else {
			state = NAME_CHOSEN;
			res = msg + "? Is that your name?\n";
		}
	} else if (state == UNVERIFIED){
		Player player = playerForName(ActiveNames[sockuid]);
		Handle<String> hash = String::New(player.passwordHash().c_str());
		Handle<String> key = String::New(msg.c_str());
		if (VerifyPassword(hash, key)){
			state = INGAME;
			res = "Very well.\n";
			player.setSockuid(sockuid);
			setPlayerForSocket(sockuid, player);
		} else {
			res = "Wrong. Try again:\n";
		}
	} else if (state == NAME_CHOSEN){
		if (tolower(msg[0]) == 'y'){
			state = NAME_CONFIRMED;
			res = "Choose a password:\n";
		} else if (tolower(msg[0]) == 'n'){
			state = UNNAMED;
			res = "Who are you then?\n";
			ActiveNames.erase(sockuid);
		} else {
			res = "Answer yes or no. Is your name " + ActiveNames[sockuid] + "?\n";
		}
	} else if (state == NAME_CONFIRMED){
		string hash = string(*String::Utf8Value(HashPassword(String::New(msg.c_str()))));
		UnconfirmedPasswords[sockuid] = hash;
		state = PASSWORD_CHOSEN;
		res = "Confirm your password:\n";
	} else if (state == PASSWORD_CHOSEN){
		Handle<String> hash = String::New(UnconfirmedPasswords[sockuid].c_str());
		Handle<String> key = String::New(msg.c_str());
		if (VerifyPassword(hash, key)){
			state = PASSWORD_CONFIRMED;
			res = "Very well.\n";
			Player player = Player(ActiveNames[sockuid], string(*String::Utf8Value(hash)));
			player.setSockuid(sockuid);
			setPlayerForSocket(sockuid, player);
		} else {
			state = NAME_CONFIRMED;
			res = "Passwords didn't match. Choose a password:\n";
			UnconfirmedPasswords.erase(sockuid);
		}
	} else if (state == PASSWORD_CONFIRMED){
		// char creation?
		state = INGAME;
	}
	
	res = (char)state + res;
    return handleScope.Close(String::New(res.c_str()));
}

void Init(Handle<Object> exports, Handle<Object> module) 
{
	exports->Set(String::NewSymbol("setScrypt"), FunctionTemplate::New(SetScrypt)->GetFunction());
	exports->Set(String::NewSymbol("setBroadcast"), FunctionTemplate::New(SetBroadcast)->GetFunction());
	exports->Set(String::NewSymbol("addSocket"), FunctionTemplate::New(AddSocket)->GetFunction());
	exports->Set(String::NewSymbol("process"), FunctionTemplate::New(ProcessInput)->GetFunction());
}

NODE_MODULE(mud, Init)