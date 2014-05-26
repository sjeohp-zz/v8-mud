#ifndef Mud_Scrypt_h
#define Mud_Scrypt_h

#include <v8.h>

using namespace v8;

Handle<Value> SetScrypt(const Arguments& args);
Handle<String> HashPassword(Handle<String> key);
bool VerifyPassword(Handle<String> hash, Handle<String> key);

#endif