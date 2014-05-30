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

Room::Room(long rnum)
{
	rnum_ = rnum;
	name_ = "The Void";
	desc_ = "There is nothing here.";
	phead_ = 0;
	memset(exits_, 0, sizeof(Room*)*6);
}

void Room::addPlayer(Player* player_ptr) 
{ 
	if (!phead_){
		phead_ = player_ptr;
		phead_->setRoomPlayersNext(phead_);
		phead_->setRoomPlayersPrev(phead_);
		player_ptr->setRoom(this);
		return;
	}
	player_ptr->setRoomPlayersNext(phead_);
	player_ptr->setRoomPlayersPrev(phead_->roomPlayersPrev());
	phead_->roomPlayersPrev()->setRoomPlayersNext(player_ptr); 
	phead_->setRoomPlayersPrev(player_ptr);
	player_ptr->setRoom(this);
}

void Room::removePlayerWithName(string name)
{
	if (!phead_){
		return;
	}
	Player* curr = phead_;
	while (1){
		if (curr->name() == name){
			curr->roomPlayersNext()->setRoomPlayersPrev(curr->roomPlayersPrev());
			curr->roomPlayersPrev()->setRoomPlayersNext(curr->roomPlayersNext());
			curr->setRoomPlayersNext(0);
			curr->setRoomPlayersPrev(0);
			break;
		} else {
			curr = curr->roomPlayersNext();
			if (curr == phead_){
				break;
			}
		}
	}
	if (phead_->name() == name){
		phead_ = phead_->roomPlayersNext();
	}
}

string Room::exitstr() const 
{ 
	string str = "[ Exits: ";
	if (exits_[0]){
		str += "n ";
	}
	if (exits_[1]){
		str += "e ";
	}
	if (exits_[2]){
		str += "s ";
	}
	if (exits_[3]){
		str += "w ";
	}
	if (exits_[4]){
		str += "u ";
	}
	if (exits_[5]){
		str += "d ";
	}
	str += "]";
	if (str.size() < 11){
		str = "[ No exits ]";
	}
	return str;
}

Room* roomAt(long rnum)
{
	if (rnum >= RoomsAll.size() || rnum < 1){
		return 0;
	}
	Room* rm = &RoomsAll.at(rnum);
	return rm;
}

void loadRooms()
{
	for (int i = 0; i < 100; ++i){
		RoomsAll.at(i) = Room(i);
	}

	Room rm = Room(1);
	rm.setName("Lobby");
	rm.setDesc("There is nothing here.");
	RoomsAll.at(1) = rm;
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
		return;
	}
	exits_[dir] = rnum;
}
