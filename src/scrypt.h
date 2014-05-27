#ifndef Mud_Scrypt_h
#define Mud_Scrypt_h

#include <iostream>

#include <v8.h>

using namespace v8;
using namespace std;

Handle<Value> SetScrypt(const Arguments& args);
string HashPassword(string key);
bool VerifyPassword(string hash, string key);

#endif