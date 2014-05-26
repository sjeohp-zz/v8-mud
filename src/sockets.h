#ifndef Mud_Sockets_h
#define Mud_Sockets_h

#include <iostream>
#include <v8.h>

using namespace std;
using namespace v8;

Handle<Value> SetBroadcast(const Arguments& args);
void Broadcast(string sockuid, string msg);
Handle<Value> AddSocket(const Arguments& args);
void Write(string sockuid, string msg);

#endif