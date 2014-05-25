#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <utility>

#include <node.h>
#include <v8.h>
#include <stdio.h>
#include <stdlib.h>

#include "player.h"
#include "commands.h"
#include "sockets.h"
#include "trie.h"

using namespace std;
using namespace v8;

static Persistent<Object> Scrypt;

static unordered_map<string, string> ActiveNames;
static unordered_map<string, string> UnconfirmedPasswords;

Handle<Value> SetScrypt(const Arguments& args)
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

	Scrypt = Persistent<Object>::New(args[0]->ToObject());
	
	Handle<Object> hash = Scrypt->Get(String::New("hash"))->ToObject();
	Handle<Object> hashConfig = hash->Get(String::New("config"))->ToObject();
	hashConfig->Set(String::New("keyEncoding"), String::New("utf8"));
	hashConfig->Set(String::New("outputEncoding"), String::New("hex"));
	
	Handle<Object> verify = Scrypt->Get(String::New("verify"))->ToObject();
	Handle<Object> verifyConfig = verify->Get(String::New("config"))->ToObject();
	verifyConfig->Set(String::New("hashEncoding"), String::New("hex"));
	verifyConfig->Set(String::New("keyEncoding"), String::New("utf8"));
	
	return handleScope.Close(Null());
}

Handle<String> HashPassword(Handle<String> key)
{
	Local<Context> context = Context::GetCurrent();
	Handle<Object> global = context->Global();
		
	Handle<Object> hash = Scrypt->Get(String::New("hash"))->ToObject();
	Handle<Object> params = Scrypt->Get(String::New("params"))->ToObject();
	Handle<Value> paramsArgv[] = { Number::New(0.1) };
	Handle<Value> scryptParams = params->CallAsFunction(global, 1, paramsArgv);

	Handle<Value> hashArgv[] = { key, scryptParams };
	Handle<Value> result = hash->CallAsFunction(global, 2, hashArgv);

	return result->ToString();
}

bool VerifyPassword(Handle<String> hash, Handle<String> key)
{
	Local<Context> context = Context::GetCurrent();
	Handle<Object> global = context->Global();

	Handle<Object> verifyFunc = Scrypt->Get(String::New("verify"))->ToObject();
	Handle<Value> verifyArgv[] = { hash, key };
	Handle<Value> result = verifyFunc->CallAsFunction(global, 2, verifyArgv);

	return result->ToBoolean()->Value();
}

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
		if (PlayersAll.count(msg) > 0){
			state = UNVERIFIED;
			res = "Password:\n";
		} else {
			state = NAME_CHOSEN;
			res = msg + "? Is that your name?\n";
		}
	} else if (state == UNVERIFIED){
		Player player = PlayersAll[ActiveNames[sockuid]];
		Handle<String> hash = String::New(player.passwordHash().c_str());
		Handle<String> key = String::New(msg.c_str());
		if (VerifyPassword(hash, key)){
			state = INGAME;
			res = "Very well.\n";
			player.setSockuid(sockuid);
			PlayersInGame[sockuid] = player;
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
			PlayersInGame[sockuid] = player;
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