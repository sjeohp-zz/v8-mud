#include "room.h"
#include "player.h"

#include <fstream>
#include <sstream>
#include <ctime>
#include <string>
#include <vector>

using namespace std;

static vector<Room> RoomsAll(5);

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

string Room::Serialize()
{
	stringstream strstream;
	strstream << "{";
	strstream << "{" << rnum_ << "}";
	strstream << "{" << exits_[0] << "}";
	strstream << "{" << exits_[1] << "}";
	strstream << "{" << exits_[2] << "}";
	strstream << "{" << exits_[3] << "}";
	strstream << "{" << exits_[4] << "}";
	strstream << "{" << exits_[5] << "}";
	strstream << "{" << name_ << "}";
	strstream << "{" << desc_ << "}";
	strstream << "}";
	return strstream.str();
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
	stringstream tb;
	ifstream tf("./savefiles/RoomsSaveTime.txt", ios::in);
	if (tf.is_open()){
		tb << tf.rdbuf();
	}

	int roomCount = 0;
	ifstream file("./savefiles/Rooms " + tb.str() + ".txt", ios::in);
	if (file.is_open()){

		stringstream buffer;
		buffer << file.rdbuf();
		string str = buffer.str();
		unsigned long temp;
		int c = 1;
		int n = 0;
		
		while ((temp = str.find('{', c)) != string::npos){
			c = temp + 2;
			n = str.find('}', c);
			long rnum = atol(str.substr(c, n-c).c_str());
			c = n + 2;
			n = str.find('}', c);
			long north = atol(str.substr(c, n-c).c_str());
			c = n + 2;
			n = str.find('}', c);
			long east = atol(str.substr(c, n-c).c_str());
			c = n + 2;
			n = str.find('}', c);
			long south = atol(str.substr(c, n-c).c_str());
			c = n + 2;
			n = str.find('}', c);
			long west = atol(str.substr(c, n-c).c_str());
			c = n + 2;
			n = str.find('}', c);
			long up = atol(str.substr(c, n-c).c_str());
			c = n + 2;
			n = str.find('}', c);
			long down = atol(str.substr(c, n-c).c_str());
			c = n + 2;
			n = str.find('}', c);
			string name = str.substr(c, n-c);
			c = n + 2;
			n = str.find('}', c);
			string desc = str.substr(c, n-c);
			Room rm = Room(rnum);
			rm.setExit(0, north);
			rm.setExit(1, east);
			rm.setExit(2, south);
			rm.setExit(3, west);
			rm.setExit(4, up);
			rm.setExit(5, down);
			rm.setName(name);
			rm.setDesc(desc);
			RoomsAll.at(rnum) = rm;
			++roomCount;
		}
	}

	if (roomCount < 2){
		for (int i = 0; i < RoomsAll.size(); ++i){
			RoomsAll.at(i) = Room(i);
		}
		Room rm = Room(1);
		rm.setName("Lobby");
		rm.setDesc("There is nothing here.");
		RoomsAll.at(1) = rm;
	}
}

void saveRooms()
{
	time_t rawtime;
	struct tm * timeinfo;

	time (&rawtime);
	timeinfo = localtime(&rawtime);
	string timeStr = string(asctime(timeinfo));

	ofstream tf("./savefiles/RoomsSaveTime.txt", ios::out | ios::trunc);
	if (tf.is_open()){
		tf << timeStr;
	}

	ofstream file("./savefiles/Rooms " + timeStr + ".txt", ios::out | ios::app);
	if (file.is_open()){
		for (auto it = RoomsAll.begin(); it != RoomsAll.end(); ++it){
			file << it->Serialize() << ",";
		}
		file.close();
    }
}