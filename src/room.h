#ifndef Mud_Room_h
#define Mud_Room_h

#include <string>
#include <vector>

using namespace std;

class Player;

class Room
{
private:
	long rnum_;
	string name_;
	string desc_;
	Player* phead_;
	Room* exits_[6];
public:
	Room();

	long rnum() const { return rnum_; }
	void setRnum(long rnum) { rnum_ = rnum; }

	string name() const { return name_; }
	void setName(string name) { name_ = name; }

	string desc() const { return desc_; }
	void setDesc(string desc) { desc_ = desc; }

	void addPlayer(Player* player_ptr);
	int removePlayerWithName(string name);

	Room* north() const { return exits_[0]; }
	void setNorth(Room* ptr) { exits_[0] = ptr; }
	Room* east() const { return exits_[1]; }
	void setEast(Room* ptr) { exits_[1] = ptr; }
	Room* south() const { return exits_[2]; }
	void setSouth(Room* ptr) { exits_[2] = ptr; }
	Room* west() const { return exits_[3]; }
	void setWest(Room* ptr) { exits_[3] = ptr; }
	Room* up() const { return exits_[4]; }
	void setUp(Room* ptr) { exits_[4] = ptr; }
	Room* down() const { return exits_[5]; }
	void setDown(Room* ptr) { exits_[5] = ptr; }

	string exits() const 
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
};

Room* roomAt(long rnum);
void loadRooms();

#endif