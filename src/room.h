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
	Room(long rnum);

	long rnum() const { return rnum_; }
	void setRnum(long rnum) { rnum_ = rnum; }

	string name() const { return name_; }
	void setName(string name) { name_ = name; }

	string desc() const { return desc_; }
	void setDesc(string desc) { desc_ = desc; }

	Player* phead() const { return phead_; }

	void addPlayer(Player* player_ptr);
	void removePlayerWithName(string name);

	Room* north() const;
	Room* east() const;
	Room* south() const;
	Room* west() const;
	Room* up() const;
	Room* down() const;
	void setExit(int dir, long rnum);

	long* exits() { return exits_; }

	string exitstr() const;
};

Room* roomAt(long rnum);
void loadRooms();

#endif