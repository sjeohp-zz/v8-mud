#ifndef Mud_Room_h
#define Mud_Room_h

#include <string>
#include <vector>

#include "player.h"

using namespace std;

class Room
{
private:
	string rnum_;
	string name_;
	vector<Player*> players_;
public:
	Room();

	string rnum() const { return rnum_ }
	void setRnum(string rnum) { rnum_ = rnum; }
	string name() const { return name_; }
	void setName(string name) { name_ = name; }
	vector<Player*> players() const { return players_; }
	void addPlayer(Player* player_ptr) { players_.push_back(player_ptr); }
	void removePlayerByName(string playername) { 
		for (auto it = players_.begin(); it != players_.end(); ++it){
			if (it->name() == playername){ 
				players_.erase(it);
				return;
			}
		}
	}
};

#endif