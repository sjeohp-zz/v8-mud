#ifndef Mud_Trie_h
#define Mud_Trie_h

typedef struct Trie
{
	void* value;
	unsigned char abbrev_path;
	int abbrev_end;
	struct Trie* children['~' - '!'];
} Trie;

Trie* newTrie()
{
	Trie* node = (Trie*)calloc(1, sizeof(Trie));
	return node;
}

Trie* addKeyForValue(Trie* node, const unsigned char* key, void* value, int abbreviates)
{
	while (*key){
		if (!node->children[*key - '!']){
			node->children[*key - '!'] = newTrie();
		}
		node = node->children[*key++ - '!'];
		if (!node->abbrev_end && !node->abbrev_path && abbreviates){
			node->abbrev_path = *key;
			if (!*key){
				node->abbrev_end = 1;
			}
		}
	}
	node->value = value;
	return node;
}

void* valueForKey(Trie* node, const unsigned char* key)
{
	while (*key){
		node = node->children[*key++ - '!'];
		if (!node){
			return 0;
		}
	}
	if (!node->value){
		while (node->abbrev_path && !node->abbrev_end){
			node = node->children[node->abbrev_path - '!'];
		}
	}
	return node->value;
}

#endif