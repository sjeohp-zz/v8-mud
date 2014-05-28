#ifndef Mud_Commands_h
#define Mud_Commands_h

#include <iostream>
#include <v8.h>

using namespace std;
using namespace v8;

struct Trie;

typedef string (*CommandFunc)(string, string);

struct Trie* Commands();

Handle<Value> run_command_benchmarks(const Arguments& args);

#endif