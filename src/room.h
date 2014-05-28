#ifndef Mud_Room_h
#define Mud_Room_h

#include <string>
#include <vector>

#include "player.h"

using namespace std;

class PlayersIterator
{
private:
	Player* ptr_;
	Player* begin_;
	Player* end_;
public:
	PlayersIterator() {};
	PlayersIterator(Player* ptr) : ptr_(ptr), head_(ptr), end_(0) {};
	PlayersIterator operator++ () { ptr_ = ptr_->roomPlayersNext(); if (!end_) { end_ = begin_; } return *this; }
	PlayersIterator operator-- () { ptr_ = ptr_->roomPlayersPrev(); if (!end_) { end_ = begin_; } return *this; }
	Player& operator* () { return *ptr_; }
	Player* operator-> () { return ptr_; }
	Player* begin() { return begin_; }
	Player* end() { return end_; }
};

class Room
{
private:
	string rnum_;
	string name_;
	Player* players_head_;
public:
	Room();

	string rnum() const { return rnum_ }
	void setRnum(string rnum) { rnum_ = rnum; }

	string name() const { return name_; }
	void setName(string name) { name_ = name; }

	void addPlayer(Player* player_ptr) { 
		player_ptr->setRoomPlayersNext(players_head_);
		player_ptr->setRoomPlayersPrev(players_head_->roomPlayersPrev());
		players_head_->roomPlayersPrev()->setRoomPlayersNext(player_ptr); 
		players_head_->setRoomPlayersPrev(player_ptr);
	}

	Player* removePlayerWithName(string name) {
		Player* curr = players_;
		while (curr){
			if (curr->name() == name){
				curr->roomPlayersNext()->setRoomPlayersPrev(curr->roomPlayersPrev());
				curr->roomPlayersPrev()->setRoomPlayersNext(curr->roomPlayersNext()); 
			} else {
				curr = curr->roomPlayersNext();
				if (curr == players_){
					return 0;
				}
			}
		}
	}

	PlayersIterator playersIterator() { return PlayersIterator(players_head_); }
};

Room::Room() {};

#endif