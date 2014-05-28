#ifndef Mud_Trie_h
#define Mud_Trie_h

typedef struct Trie
{
	void* value;
	unsigned char abbrev_path;
	int abbrev_end;
	struct Trie* children['~' - '!'];
}	Trie;

Trie* addKeyForValue(Trie* node, const unsigned char* key, void* value, int abbreviates);
void* valueForKey(Trie* node, const unsigned char* key);

#endif