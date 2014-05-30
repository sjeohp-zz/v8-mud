#include "room.h"
#include "player.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

static vector<Room> RoomsAll(100);

Room::Room()
{
	rnum_ = 0;
	name_ = "The Void";
	desc_ = "There is nothing here.";
	phead_ = 0;
	memset(exits_, 0, sizeof(Room*)*6);
};

void Room::addPlayer(Player* player_ptr) 
{ 
	if (!phead_){
		phead_ = player_ptr;
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
		RoomsAll.push_back(rm);
	}
	if (RoomsAll.size() < 2){
		Room rm = Room();
		rm.setRnum(1);
		rm.setName("Lobby");
		rm.setDesc("There is nothing here.");
		RoomsAll.push_back(rm);
	}
}

Room* Room::north() const { return roomAt(exits_[0]); }
Room* Room::east() const { return roomAt(exits_[1]); }
Room* Room::south() const { return roomAt(exits_[2]); }
Room* Room::west() const { return roomAt(exits_[3]); }
Room* Room::up() const { return roomAt(exits_[4]); }
Room* Room::down() const { return roomAt(exits_[5]); }

void Room::setExit(int dir, long rnum) 
{ 
	if (!roomAt(rnum)){
		RoomsAll.at(rnum) = Room();
	}
	exits_[dir] = rnum; 
}
