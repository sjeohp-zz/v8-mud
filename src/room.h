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
	long exits_[6];
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

	Room* north() const;
	Room* east() const;
	Room* south() const;
	Room* west() const;
	Room* up() const;
	Room* down() const;
	void setExit(int dir, long rnum);

	long* exits() { return exits_; }

	string exitstr() const 
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