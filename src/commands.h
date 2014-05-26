#ifndef Mud_Commands_h
#define Mud_Commands_h

#include <iostream>

using namespace std;

struct Trie;

typedef string (*CommandFunc)(string, string);

struct Trie* Commands();

#endif