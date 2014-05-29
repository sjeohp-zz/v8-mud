#include "room.h"
#include "player.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

static vector<Room> RoomsAll;

Room::Room()
{
	phead_ = 0;
	memset(exits_, 0, sizeof(Room*)*6);
};

void Room::addPlayer(Player* player_ptr) 
{ 
	if (!phead_){
		player_ptr = phead_;
	}
	player_ptr->setRoomPlayersNext(phead_);
	player_ptr->setRoomPlayersPrev(phead_->roomPlayersPrev());
	phead_->roomPlayersPrev()->setRoomPlayersNext(player_ptr); 
	phead_->setRoomPlayersPrev(player_ptr);
	player_ptr->setRoom(this);
}

int Room::removePlayerWithName(string name)
{
	Player* curr = phead_;
	while (curr){
		if (curr->name() == name){
			curr->roomPlayersNext()->setRoomPlayersPrev(curr->roomPlayersPrev());
			curr->roomPlayersPrev()->setRoomPlayersNext(curr->roomPlayersNext()); 
			return 1;
		} else {
			curr = curr->roomPlayersNext();
			if (curr == phead_){
				return 0;
			}
		}
	}
	return 0;
}

Room* roomAt(long rnum)
{
	if (rnum >= RoomsAll.size()){
		return 0;
	}
	Room* rm = &RoomsAll.at(rnum);
	return rm;
}

void loadRooms()
{
	if (RoomsAll.empty()){
		Room rm = Room();
		rm.setRnum(0);
		rm.setName("Lobby");
		rm.setDesc("There is nothing here.");
		RoomsAll.push_back(rm);
	}
}